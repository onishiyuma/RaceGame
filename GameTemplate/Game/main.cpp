#include "stdafx.h"
#include <cmath>
#include "system/system.h"
#include "Stage/StageLoader/StageDefinition.h"
#include "Stage/StageAssetDatabase.h"
#include "Stage/StageLoader/StageLoader.h"
#include"StageResourceLoader.h"
#include"Stage/Stage.h"
#include"Stage/StageBuilder.h"
#include"Scene/SceneManager.h"
#include"Scene/DebugScene.h"

// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	SceneManager* sceneManager = NewGO<SceneManager>(0);
	sceneManager->Init(std::make_unique<DebugScene>());

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		K2Engine::GetInstance()->Execute();
	}

	K2Engine::DeleteInstance();

	return 0;
}

