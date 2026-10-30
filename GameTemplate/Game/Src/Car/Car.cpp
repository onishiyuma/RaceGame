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

	m_performance = performance;

	// 車の描画モデルを初期化する。
	if (!m_visual.Init(modelAsset))return false;

	// 座標を取得する前に車体・車両物理を初期化する。
	if (!m_physics.Init(position, rotation))return false;

	// 初回の描画に備えて姿勢を反映する。
	const auto& state = m_physics.GetState();

	// 初回の描画に備えて姿勢を反映する。
	m_visual.Update(
		state.position,
		state.rotation
	);

	return true;
}

void Car::Update(const CarInput& input, float deltaTime)
{
	// 操作入力を物理オブジェクトに反映する。
	m_physics.ApplyInput(input);
}

void Car::PostPhysicsUpdate()
{
	// Bulletの計算結果を取得する。
	m_physics.SyncState();

	const auto& state = m_physics.GetState();

	// 物理計算後の位置・回転を描画に反映する。
	m_visual.Update(
		state.position,
		state.rotation
	);
}

void Car::Render(RenderContext& renderContext)
{
	m_visual.Render(renderContext);
}