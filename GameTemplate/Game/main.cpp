#include "stdafx.h"
#include "system/system.h"
#include "Stage/StageLoader/StageDefinition.h"
#include "Stage/StageAssetDatabase.h"
#include "Stage/StageLoader/StageLoader.h"
#include"StageResourceLoader.h"

// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//テスト↓

	StageDefinition stageDefinition;

	// ステージJSONを読み込む。
	if (!LoadStage(
		"Assets/CourseData/TestStage.json",
		stageDefinition))
	{
		MessageBoxA(
			nullptr,
			"Stage JSONの読み込みに失敗しました。",
			"Error",
			MB_OK
		);

		return 0;
	}

	StageAssetDatabase stageAssetDatabase;

	// アセット定義JSONを読み込む。
	if (!stageAssetDatabase.Init(
		"Assets/AssetsData/TestStageAssets.json"))
	{
		MessageBoxA(
			nullptr,
			"TestStageAssets.jsonの読み込みに失敗しました。",
			"Error",
			MB_OK
		);

		return 0;
	}

	StageResourceLoader stageResourceLoader;

	// ステージで使用するリソースをロード。
	if (!stageResourceLoader.Load(
		stageDefinition,
		stageAssetDatabase))
	{
		MessageBoxA(
			nullptr,
			"ステージリソースのロードに失敗しました。",
			"Error",
			MB_OK
		);

		return 0;
	}

	MessageBoxA(
		nullptr,
		"ステージリソースのロードに成功しました。",
		"Success",
		MB_OK
	);

	// =============================
	// ロード済みTKMを取得
	// =============================

	TkmFile* tkmFile =
		g_resourceManager->GetTkm(
			"Assets/modelData/unityChan.tkm"
		);

	if (tkmFile == nullptr)
	{
		MessageBoxA(
			nullptr,
			"unityChan.tkmを取得できませんでした。",
			"Error",
			MB_OK
		);

		return 0;
	}

	//tksをロード
	TksFile* tksFile =
		g_resourceManager->GetTks(
			"Assets/modelData/unityChan.tks"
		);

	if (tksFile == nullptr)
	{
		MessageBoxA(
			nullptr,
			"unityChan.tksを取得できませんでした。",
			"Error",
			MB_OK
		);
		return 0;
	}


	// =============================
	// モデルを生成
	// =============================

	ModelRender modelRender;

	modelRender.Init(
		tkmFile,
		tksFile,						// アニメーションクリップ
		nullptr,
		0,								// アニメーション数
		enModelUpAxisY,					// ←ここは今のenum名に合わせる
		true,							// ShadowCaster
		true,							// ShadowReceiver
		AlphaBlendMode_None				// ←ここも今のenum名に合わせる
	);



	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		K2Engine::GetInstance()->Execute();
		modelRender.Draw(g_graphicsEngine->GetRenderContext());

	}

	K2Engine::DeleteInstance();


	return 0;
}

