
#include "stdafx.h"
#include "CarPhysics.h"
#include "CarInput.h"
#include <algorithm>

namespace
{
	float Clamp(float value, float minValue, float maxValue)
	{
		if (value < minValue) return minValue;
		if (value > maxValue) return maxValue;
		return value;
	}

	constexpr float VEHICLE_SCALE = 50.0f;

	constexpr float BODY_WIDTH = 2.0f * VEHICLE_SCALE;                 // 車体の幅
	constexpr float BODY_HEIGHT = 1.0f * VEHICLE_SCALE;                // 車体の高さ
	constexpr float BODY_LENGTH = 3.0f * VEHICLE_SCALE;                // 車体の前後の長さ
	constexpr float BODY_MASS = 1000.0f;                // 車体の質量

	constexpr float SUSPENSION_STIFFNESS = 20.0f;      // サスペンションのばねの硬さ
	constexpr float SUSPENSION_COMPRESSION = 4.4f;     // サスペンションが縮むときの減衰
	constexpr float SUSPENSION_DAMPING = 2.3f;         // サスペンションが伸びるときの減衰
	constexpr float MAX_SUSPENSION_TRAVEL_CM = 20.0f * VEHICLE_SCALE;  // サスペンションの最大伸縮距離（cm）
	constexpr float FRICTION_SLIP = 10.5f;             // タイヤのグリップに影響する係数
	constexpr float MAX_SUSPENSION_FORCE = 6000.0f;   // サスペンションの最大支持力

	constexpr float WHEEL_RADIUS = 0.35f * VEHICLE_SCALE;              // タイヤの半径
	constexpr float SUSPENSION_REST_LENGTH = 0.4f * VEHICLE_SCALE;     // サスペンションの自然長

	constexpr float WHEEL_X = 0.75f * VEHICLE_SCALE;                   // 車体中心からタイヤまでの左右方向の距離
	constexpr float WHEEL_Y = -0.3f * VEHICLE_SCALE;                   // 車体中心からタイヤ取付位置までの高さ
	constexpr float FRONT_WHEEL_Z = 1.0f * VEHICLE_SCALE;              // 前輪の前後方向の位置
	constexpr float REAR_WHEEL_Z = -1.0f * VEHICLE_SCALE;              // 後輪の前後方向の位置

	constexpr float WHEEL_ROLL_INFLUENCE = 0.1f;       // タイヤの横方向の摩擦が車体の傾きに与える影響

	constexpr float ENGINE_FORCE = 25000.0f;          // 駆動輪1本あたりのエンジン駆動力
	constexpr float BRAKE_FORCE = 15.0f;           // タイヤ1本あたりのブレーキ強度
	constexpr float MAX_STEERING_ANGLE = 0.35f;    // 前輪の最大操舵角（ラジアン）

	constexpr int FRONT_LEFT_WHEEL = 0;           // 左前輪のインデックス
	constexpr int FRONT_RIGHT_WHEEL = 1;          // 右前輪のインデックス
	constexpr int REAR_LEFT_WHEEL = 2;            // 左後輪のインデックス
	constexpr int REAR_RIGHT_WHEEL = 3;           // 右後輪のインデックス
}

CarPhysics::~CarPhysics()
{
	Release();
}

void CarPhysics::SyncState()
{
	// Bulletの計算結果を車の物理状態に反映する。
	m_rigidBody.GetPositionAndRotation(
		m_state.position,
		m_state.rotation
	);
}

const CarPhysicsState& CarPhysics::GetState() const
{
	return m_state;
}

void CarPhysics::Release()
{
	// 車両を物理ワールドから登録解除する。
	if (m_vehicle)
	{
		auto* world =
			nsK2EngineLow::PhysicsWorld::GetInstance()
			->GetDynamicWorld();

		world->removeAction(m_vehicle.get());

		m_vehicle.reset();
	}

	m_raycaster.reset();

	// 車体の剛体を解放する。
	m_rigidBody.Release();
}
void CarPhysics::InitRigidBody(
	const Vector3& position,
	const Quaternion& rotation
)
{
	// 車体用のボックスコライダーを作成する。
	m_collider.Create({
		BODY_WIDTH,
		BODY_HEIGHT,
		BODY_LENGTH
		});

	nsK2EngineLow::RigidBodyInitData rbInfo;
	rbInfo.pos = position;
	rbInfo.rot = rotation;
	rbInfo.collider = &m_collider;
	rbInfo.mass = BODY_MASS;

	// 剛体の初期化。
	m_rigidBody.Init(rbInfo);
}

void CarPhysics::InitVehicle()
{
	auto* world =
		nsK2EngineLow::PhysicsWorld::GetInstance()
		->GetDynamicWorld();

	// 車両用のRaycasterを生成する。
	m_raycaster =
		std::make_unique<btDefaultVehicleRaycaster>(world);

	btRaycastVehicle::btVehicleTuning tuning;

	// サスペンションを設定する。
	tuning.m_suspensionStiffness = SUSPENSION_STIFFNESS;
	tuning.m_suspensionCompression = SUSPENSION_COMPRESSION;
	tuning.m_suspensionDamping = SUSPENSION_DAMPING;
	tuning.m_maxSuspensionTravelCm = MAX_SUSPENSION_TRAVEL_CM;
	tuning.m_frictionSlip = FRICTION_SLIP;
	tuning.m_maxSuspensionForce = MAX_SUSPENSION_FORCE;

	// 車体の剛体を使って車両を作成する。
	m_vehicle = std::make_unique<btRaycastVehicle>(
		tuning,
		m_rigidBody.GetBody(),
		m_raycaster.get()
	);

	// X軸が左右、Y軸が上下、Z軸が前後。
	m_vehicle->setCoordinateSystem(0, 1, 2);

	// タイヤを設定してから物理ワールドに登録する。
	InitWheels(tuning);

	world->addAction(m_vehicle.get());
}

void CarPhysics::InitWheels(
	const btRaycastVehicle::btVehicleTuning& tuning
)
{
	// タイヤのサスペンション方向と車軸方向。
	const btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
	const btVector3 wheelAxle(-1.0f, 0.0f, 0.0f);

	struct WheelSetting
	{
		btVector3 position;
		bool isFront;
	};

	// 車体中心を基準に4輪の位置を設定する。
	const WheelSetting wheels[] = {
		{ {-WHEEL_X, WHEEL_Y, FRONT_WHEEL_Z}, true  },
		{ { WHEEL_X, WHEEL_Y, FRONT_WHEEL_Z}, true  },
		{ {-WHEEL_X, WHEEL_Y, REAR_WHEEL_Z},  false },
		{ { WHEEL_X, WHEEL_Y, REAR_WHEEL_Z},  false }
	};

	for (const auto& wheel : wheels)
	{
		auto& wheelInfo = m_vehicle->addWheel(
			wheel.position,
			wheelDirection,
			wheelAxle,
			SUSPENSION_REST_LENGTH,
			WHEEL_RADIUS,
			tuning,
			wheel.isFront
		);

		wheelInfo.m_rollInfluence = WHEEL_ROLL_INFLUENCE;
	}
}

bool CarPhysics::Init(
	const Vector3& position,
	const Quaternion& rotation
)
{
	m_state = CarPhysicsState{};
	m_state.position = position;
	m_state.rotation = rotation;

	// 車体の剛体を初期化する。
	InitRigidBody(position, rotation);

	// 車両物理を初期化する。
	InitVehicle();

	return true;
}

void CarPhysics::ApplyInput(const CarInput& input)
{
	if (!m_vehicle)return;

	// 入力を有効範囲に制限する。
	const float accelerator = Clamp(input.accelerator, 0.0f, 1.0f);
	const float brake = Clamp(input.brake, 0.0f, 1.0f);
	const float steering = Clamp(input.steering, -1.0f, 1.0f);

	//エンジンの力。
	// ブレーキ中は駆動力を与えない。
	const float engineForce =
		brake > 0.0f ? 0.0f : accelerator * ENGINE_FORCE;

	//ブレーキの力。
	const float brakeForce = brake * BRAKE_FORCE;

	//タイヤの角度。
	const float steeringAngle = steering * MAX_STEERING_ANGLE;

	// 後輪にエンジンの駆動力を与える。
	m_vehicle->applyEngineForce(
		engineForce,
		REAR_LEFT_WHEEL
	);

	m_vehicle->applyEngineForce(
		engineForce,
		REAR_RIGHT_WHEEL
	);

	// 前輪の操舵角を設定する。
	m_vehicle->setSteeringValue(
		steeringAngle,
		FRONT_LEFT_WHEEL
	);

	m_vehicle->setSteeringValue(
		steeringAngle,
		FRONT_RIGHT_WHEEL
	);

	// 4輪にブレーキを設定する。
	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		m_vehicle->setBrake(brakeForce, i);
	}
}
