#pragma once
#include"Src/Car/CarPhysicsState.h"
struct CarInput;
class CarPhysics : public Noncopyable
{
public:
	CarPhysics() = default;
	~CarPhysics();

	bool Init(
		const Vector3& position,
		const Quaternion& rotation
	);

	// 操作入力を車両の駆動・操舵に反映する。
	void ApplyInput(const CarInput& input);

	// 物理更新後の状態を反映する。
	void SyncState();

	const CarPhysicsState& GetState() const;

	void Release();

private:

	/// <summary>
	/// 車体の剛体を初期化する。
	/// </summary>
	/// <param name="position"></param>
	/// <param name="rotation"></param>
	void InitRigidBody(
		const Vector3& position,
		const Quaternion& rotation
	);
	/// <summary>
	/// 車両物理を初期化する。
	/// </summary>
	void InitVehicle();

	/// <summary>
	/// 車輪を初期化。
	/// </summary>
	/// <param name="tuning"></param>
	void InitWheels(
		const btRaycastVehicle::btVehicleTuning& tuning
	);

private:
	nsK2EngineLow::BoxCollider m_collider;
	nsK2EngineLow::RigidBody m_rigidBody;

	std::unique_ptr<btDefaultVehicleRaycaster> m_raycaster;	//車輪の設置判定
	std::unique_ptr<btRaycastVehicle> m_vehicle;			//車体と車輪の物理挙動を管理する。

	CarPhysicsState m_state;
};