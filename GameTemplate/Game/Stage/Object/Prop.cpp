#include "stdafx.h"
#include "Prop.h"
#include <cassert>

bool Prop::OnInit(
	const nlohmann::json& properties)
{

	m_physicsStaticObject.CreateFromModel(
		m_stageModel->GetModel(),
		m_stageModel->GetModel().GetWorldMatrix(),
		0.0f
	);

	////テスト用
	//// 最初はRunを再生。
	//bool result = PlayAnimation("Run");

	//assert(
	//	result &&
	//	"Run animation was not found."
	//);

	return true;
}

void Prop::OnUpdate()
{
	//m_animationTestTime +=
	//	g_gameTime->GetFrameDeltaTime();

	//// 3秒後にJumpへ切り替える。
	//if (!m_isJumpPlayed &&
	//	m_animationTestTime >= 3.0f)
	//{
	//	bool result =
	//		PlayAnimation(
	//			"Jump",
	//			0.2f
	//		);

	//	assert(
	//		result &&
	//		"Jump animation was not found."
	//	);

	//	m_isJumpPlayed = true;
	//}
}