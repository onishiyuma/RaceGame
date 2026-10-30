#pragma once
#include "CarInput.h"
#include "CarPerformance.h"
#include"CarPhysicsState.h"
#include "CarVisual.h"
#include "CarPhysics.h"

class ModelAssetDefinition;
struct CarDefinition;
class Car
{
public:
	Car() = default;
	~Car() = default;

	bool Init(
		const CarDefinition& definition,
		const CarPerformance& performance,
		const ModelAssetDefinition& modelAsset,
		const Vector3& position,
		const Quaternion& rotation
	);

	void Update(const CarInput& input, float deltaTime);

	// 物理計算後の状態を描画に反映する。
	void PostPhysicsUpdate();

	void Render(RenderContext& renderContext);

	const Vector3& GetPosition() const { return m_physics.GetState().position; }

	float GetSpeed() const { return m_speed; }


private:
	float m_speed = 0.0f;

private:
	CarPhysics m_physics;
	CarPerformance m_performance;
	CarVisual m_visual;
};