#pragma once

#include "Stage/StageObject.h"

class Prop : public StageObject
{
protected:
	bool OnInit(
		const nlohmann::json& properties
	) override;

	void OnUpdate() override;

private:
	float m_animationTestTime = 0.0f;
	bool m_isJumpPlayed = false;
	PhysicsStaticObject m_physicsStaticObject;
};