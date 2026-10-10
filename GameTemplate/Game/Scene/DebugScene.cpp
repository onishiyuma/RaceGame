
#include "stdafx.h"
#include "DebugScene.h"

#include <cmath>
#include "system/system.h"
#include "Stage/StageLoader/StageLoader.h"
#include "StageResourceLoader.h"
#include "Stage/StageBuilder.h"

bool DebugScene::OnLoad()
{
	// アセットIDとアセットの対応表を読み込む。
	m_assetDatabase.Init(
		"Assets/assetsData/Course01Assets.json"
	);

	// ステージ配置情報を読み込む。
	LoadStageDefinition(
		"Assets/stage/Course01.json",
		m_stageDefinition
	);

	// ステージで使用するリソースを事前ロードする。
	StageResourceLoader resourceLoader;
	resourceLoader.Load(
		m_stageDefinition,
		m_assetDatabase
	);

	return true;
}

bool DebugScene::OnInit()
{
	auto stage = std::make_unique<Stage>();
	StageBuilder stageBuilder;

	// ロードした定義から実際のStageを構築する。
	stageBuilder.Build(
		m_stageDefinition,
		m_assetDatabase,
		*stage
	);

	m_stage = std::move(stage);

	return true;
}

void DebugScene::OnActivate()
{
	// デバッグカメラの遠クリップ距離を設定する。
	constexpr float debugFarClip = 50000.0f;
	g_camera3D->SetFar(debugFarClip);
}

void DebugScene::OnUpdate()
{
	UpdateDebugCamera();

	// ステージ上のオブジェクトを更新する。
	if (m_stage)
	{
		m_stage->Update();
	}
}

void DebugScene::OnRender(RenderContext& renderContext)
{
	if (m_stage)
	{
		m_stage->Render(renderContext);
	}
}

void DebugScene::OnUnload()
{
	// シーンが所有しているステージを破棄する。
	m_stage.reset();
}

void DebugScene::UpdateDebugCamera()
{
	// ウィンドウがアクティブでなければ操作しない。
	if (GetForegroundWindow() != g_hWnd)return;

	const auto isKeyDown = [](int key)
		{
			return (GetAsyncKeyState(key) & 0x8000) != 0;
		};

	const float deltaTime = g_gameTime->GetFrameDeltaTime();
	const float cameraDeltaTime =
		deltaTime < 0.05f ? deltaTime : 0.05f;

	// 矢印キーによる視点の回転。
	const float yawInput =
		(isKeyDown(VK_RIGHT) ? 1.0f : 0.0f)
		- (isKeyDown(VK_LEFT) ? 1.0f : 0.0f);

	const float pitchInput =
		(isKeyDown(VK_UP) ? 1.0f : 0.0f)
		- (isKeyDown(VK_DOWN) ? 1.0f : 0.0f);

	if (yawInput != 0.0f || pitchInput != 0.0f)
	{
		const Vector3 look =
			g_camera3D->GetTarget() - g_camera3D->GetPosition();

		const float lookDistance = look.Length();

		if (lookDistance > 0.000001f)
		{
			const float turnSpeed = Math::DegToRad(90.0f);

			const float yaw =
				std::atan2(look.x, look.z)
				+ yawInput * turnSpeed * cameraDeltaTime;

			float pitch =
				std::atan2(
					look.y,
					std::sqrt(look.x * look.x + look.z * look.z)
				)
				+ pitchInput * turnSpeed * cameraDeltaTime;

			const float pitchLimit = Math::DegToRad(85.0f);

			if (pitch > pitchLimit) pitch = pitchLimit;
			if (pitch < -pitchLimit) pitch = -pitchLimit;

			const Vector3 direction(
				std::sin(yaw) * std::cos(pitch),
				std::sin(pitch),
				std::cos(yaw) * std::cos(pitch)
			);

			g_camera3D->SetTarget(
				g_camera3D->GetPosition()
				+ direction * lookDistance
			);

			// 回転後の移動方向を更新する。
			g_camera3D->Update();
		}
	}

	// WASD・QEによるカメラ移動。
	Vector3 movement = Vector3::Zero;

	if (isKeyDown('W')) movement += g_camera3D->GetForward();
	if (isKeyDown('S')) movement -= g_camera3D->GetForward();
	if (isKeyDown('D')) movement += g_camera3D->GetRight();
	if (isKeyDown('A')) movement -= g_camera3D->GetRight();
	if (isKeyDown('E')) movement.y += 1.0f;
	if (isKeyDown('Q')) movement.y -= 1.0f;

	if (movement.LengthSq() > 0.000001f)
	{
		const float moveSpeed =
			isKeyDown(VK_SHIFT) ? 500.0f : 100.0f;

		movement.Normalize();
		movement *= moveSpeed * cameraDeltaTime;

		g_camera3D->SetPosition(
			g_camera3D->GetPosition() + movement
		);

		g_camera3D->SetTarget(
			g_camera3D->GetTarget() + movement
		);
	}
}
