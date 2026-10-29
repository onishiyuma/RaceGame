#pragma once
#include "CarInput.h"
#include "CarPerformance.h"
#include"CarPhysicsState.h"
#include "CarVisual.h"

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

	void Update(const CarInput& input);

	float GetSpeed() const { return m_speed; }

	void Render(RenderContext& renderContext);

private:
	void UpdateSpeed(const CarInput& input, float deltaTime);

	void UpdateSteering(const CarInput& input, float deltaTime);

	void UpdatePosition(float deltaTime);

	float m_speed = 0.0f;

private:
	CarPhysicsState m_physicsState;
	CarPerformance m_performance;
	CarVisual m_visual;
};