#include "stdafx.h"
#include"CarDefinition.h"
#include "CarPerformance.h"
#include "Car.h"
#include "ModelAssetDefinition.h"

bool Car::Init(
	const CarDefinition& definition,
	const CarPerformance& performance,
	const ModelAssetDefinition& modelAsset,
	const Vector3& position,
	const Quaternion& rotation)
{
	// 使用する車種定義が不正な場合は初期化しない。
	if (definition.id.empty() || definition.modelAssetId.empty())
	{
		K2_ASSERT(false, "車種定義が不正です。");
		return false;
	}

	m_physicsState = CarPhysicsState{};
	m_physicsState.position = position;
	m_physicsState.rotation = rotation;

	m_performance = performance;
	m_speed = 0.0f;

	// 車の描画モデルを初期化する。
	if (!m_visual.Init(modelAsset))return false;

	// 初回の描画に備えて姿勢を反映する。
	m_visual.Update(
		m_physicsState.position,
		m_physicsState.rotation
	);

	return true;
}

void Car::Update(const CarInput& input)
{
	//TODO:物理演算処理の更新を書く

	m_visual.Update(m_physicsState.position, m_physicsState.rotation);
}

void Car::Render(RenderContext& renderContext)
{
	m_visual.Render(renderContext);
}

void Car::UpdateSpeed(const CarInput& input, float deltaTime)
{}

void Car::UpdateSteering(const CarInput& input, float deltaTime)
{}

void Car::UpdatePosition(float deltaTime)
{}
