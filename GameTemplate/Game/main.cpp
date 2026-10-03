#include "stdafx.h"
#include "system/system.h"
#include "Stage/StageLoader/StageDefinition.h"
#include "Stage/StageAssetDatabase.h"
#include "Stage/StageLoader/StageLoader.h"
#include"StageResourceLoader.h"
#include"Stage/Stage.h"
#include"Stage/StageBuilder.h"

// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	PhysicsWorld::GetInstance()->DebubDrawWorld(g_graphicsEngine->GetRenderContext());

	///////ステージのロード処理///////

	//アセットIDとアセットの対応表
	StageAssetDatabase assetDatabase;
	// Jsonから読み込む。
	assetDatabase.Init("Assets/assetsData/TestStageAssets.json");

	//ステージの定義（中にオブジェクトの定義の配列が入っている）
	StageDefinition stageDefinition;
	// ステージ配置情報を読み込む。
	LoadStageDefinition("Assets/stage/TestStage.json", stageDefinition);


	StageResourceLoader resourceLoader;
	// ステージで使用するリソースを
	// ResourceManagerへ事前ロードする。
	resourceLoader.Load(stageDefinition, assetDatabase);


	/////ステージの構築/////

	Stage stage;
	StageBuilder stageBuilder;

	// StageDefinitionから実際のStageを作る。
	stageBuilder.Build(stageDefinition, assetDatabase, stage);


	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		// ステージ上のオブジェクトを更新する。
		stage.Update();
		stage.Draw(g_graphicsEngine->GetRenderContext());
		K2Engine::GetInstance()->Execute();


	}

	K2Engine::DeleteInstance();

	return 0;
}

