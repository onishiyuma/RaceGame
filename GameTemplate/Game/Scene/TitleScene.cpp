#include "stdafx.h"
#include "TitleScene.h"
#include "Scene/SceneManager.h"
#include "Scene/DebugScene.h"

TitleScene::~TitleScene()
{
	UIManager::GetInstance()->DeleteUI(&m_titleBackGround);
	UIManager::GetInstance()->DeleteUI(&m_titleLogo);
	UIManager::GetInstance()->DeleteUI(&m_selectBackGround);
	UIManager::GetInstance()->DeleteUI(&m_stageIcon[enSelect_Stage1]);
	UIManager::GetInstance()->DeleteUI(&m_stageIcon[enSelect_Stage2]);
	UIManager::GetInstance()->DeleteUI(&m_frame);
	UIManager::GetInstance()->DeleteUI(&m_aButton);
	UIManager::GetInstance()->DeleteUI(&m_bButton);
}

bool TitleScene::OnLoad()
{
	/*UIの初期化。*/
	InitUI();

	return true;
}

bool TitleScene::OnInit()
{
	FadeManager::GetInstance()->ChangeFade(FadeManager::enFade_In, 2.0f);

	return true;
}

void TitleScene::OnUpdate()
{
	/*ロードしている場合は処理しない。*/
	if (LoadingManager::GetInstance()->IsLoading())
	{
		if (FadeManager::GetInstance()->IsEnd(FadeManager::enFade_Out))
		{
			if (m_delayTime.Execute(3.0f))
			{
				auto sceneManager = GetSceneManager();
				if (sceneManager) {
					sceneManager->ChangeScene(std::make_unique<DebugScene>());
				}
			}
		}
		return;
	}

	/*現在のシーン。*/
	switch (m_currentScene)
	{
	case enScene_Title:/*タイトル。*/
		Title();
		break;
	case enScene_Select:/*選択。*/
		Select();
		break;
	default:
		break;
	}
}

/*UIの初期化。*/
void TitleScene::InitUI()
{
	/*タイトル背景の初期化。*/
	InitTitleBackGround();

	/*タイトルロゴの初期化。*/
	InitTitleLogo();

	/*選択背景の初期化。*/
	InitSelectBackGround();

	/*ステージアイコンの初期化。*/
	InitStageIcon();

	/*選択枠の初期化。*/
	InitFrame();

	/*Aボタンの初期化。*/
	InitAbutton();
	
	/*Bボタンの初期化。*/
	InitBButton();
}

/*タイトル背景の初期化。*/
void TitleScene::InitTitleBackGround()
{
	/*初期化。*/
	m_titleBackGround.Init("Assets/UI/Title/titleBack.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	/*更新処理。*/
	m_titleBackGround.Update();
	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_titleBackGround);
}

/*タイトルロゴの初期化。*/
void TitleScene::InitTitleLogo()
{
	/*初期化。*/
	m_titleLogo.Init("Assets/UI/Title/titleLogo.dds", 1920, 1080);
	/*位置設定。*/
	m_titleLogo.SetPosition({-425.0f,-100.0f,0.0f});
	/*大きさ設定。*/
	m_titleLogo.SetScale({0.7f,0.7f,1.0f});
	/*更新処理。*/
	m_titleLogo.Update();
	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_titleLogo);
}


/*選択背景の初期化。*/
void TitleScene::InitSelectBackGround()
{
	/*初期化。*/
	m_selectBackGround.Init("Assets/UI/outGame/back.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	/*描画無効化。*/
	m_selectBackGround.DisableDraw();
	/*更新処理。*/
	m_selectBackGround.Update();
	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_selectBackGround);
}

/*ステージアイコンの初期化。*/
void TitleScene::InitStageIcon()
{
	/*初期化。*/
	m_stageIcon[enSelect_Stage1].Init("Assets/UI/outGame/iconTrack_1.dds", 1920, 1080);
	/*位置設定。*/
	m_stageIcon[enSelect_Stage1].SetPosition({ -300.0f,0.0f,0.0f });
	/*大きさ設定。*/
	m_stageIcon[enSelect_Stage1].SetScale({ 1.0f,1.0f,1.0f });
	/*描画無効化。*/
	m_stageIcon[enSelect_Stage1].DisableDraw();
	/*更新処理。*/
	m_stageIcon[enSelect_Stage1].Update();
	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_stageIcon[enSelect_Stage1]);

	/*初期化。*/
	m_stageIcon[enSelect_Stage2].Init("Assets/UI/outGame/iconTrack_2.dds", 1920, 1080);
	/*位置設定。*/
	m_stageIcon[enSelect_Stage2].SetPosition({ 300.0f,0.0f,0.0f });
	/*大きさ設定。*/
	m_stageIcon[enSelect_Stage2].SetScale({ 1.0f,1.0f,1.0f });
	/*描画無効化。*/
	m_stageIcon[enSelect_Stage2].DisableDraw();
	/*更新処理。*/
	m_stageIcon[enSelect_Stage2].Update();
	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_stageIcon[enSelect_Stage2]);
}

/*選択枠の初期化。*/
void TitleScene::InitFrame()
{
	/*初期化。*/
	m_frame.Init("Assets/UI/outGame/frame.dds", 1920, 1080);
	/*位置設定。*/
	m_frame.SetPosition({ -300.0f,40.0f,0.0f });
	/*大きさ設定。*/
	m_frame.SetScale({ 1.0f,1.0f,1.0f });
	/*描画無効化。*/
	m_frame.DisableDraw();
	/*更新処理。*/
	m_frame.Update();
	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_frame);
}

/*Aボタンの初期化。*/
void TitleScene::InitAbutton()
{
	/*初期化。*/
	m_aButton.Init("Assets/UI/outGame/aButton.dds", 1920, 1080);
	/*位置設定。*/
	m_aButton.SetPosition({ 750.0f,-375.0f,0.0f });
	/*大きさ設定。*/
	m_aButton.SetScale({ 0.7f,0.7f,1.0f });
	/*描画無効化。*/
	m_aButton.DisableDraw();
	/*更新処理。*/
	m_aButton.Update();
	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_aButton);
}

/*Bボタンの初期化。*/
void TitleScene::InitBButton()
{
	/*初期化。*/
	m_bButton.Init("Assets/UI/outGame/bButton.dds", 1920, 1080);
	/*位置設定。*/
	m_bButton.SetPosition({ -720.0f,-375.0f,0.0f });
	/*大きさ設定。*/
	m_bButton.SetScale({ 0.7f,0.7f,1.0f });
	/*描画無効化。*/
	m_bButton.DisableDraw();
	/*更新処理。*/
	m_bButton.Update();
	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_bButton);
}

/*タイトルシーン。*/
void TitleScene::Title()
{
	/*Aボタンを押したら選択シーンに切り替える。*/
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		/*タイトルシーンにあるUIを非表示にする。*/
		UIManager::GetInstance()->DisableDrawUI(&m_titleBackGround);
		UIManager::GetInstance()->DisableDrawUI(&m_titleLogo);

		/*選択シーンにあるUIを表示する。*/
		UIManager::GetInstance()->EnableDrawUI(&m_selectBackGround);
		UIManager::GetInstance()->EnableDrawUI(&m_stageIcon[enSelect_Stage1]);
		UIManager::GetInstance()->EnableDrawUI(&m_stageIcon[enSelect_Stage2]);
		UIManager::GetInstance()->EnableDrawUI(&m_frame);
		UIManager::GetInstance()->EnableDrawUI(&m_aButton);
		UIManager::GetInstance()->EnableDrawUI(&m_bButton);

		/*シーンの切り替え。*/
		m_currentScene = enScene_Select;
	}
}

/*選択シーン。*/
void TitleScene::Select()
{
	/*左に選択。*/
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		/*選択肢が左端の場合は右端を選択する。*/
		if (m_currentSelect == enSelect_Stage1)
		{
			m_currentSelect = enSelect_Stage2;
			return;
		}
		m_currentSelect = static_cast<EnSelect>(static_cast<int>(m_currentSelect) - 1);
	}
	/*右に選択。*/
	else if (g_pad[0]->IsTrigger(enButtonRight))
	{
		/*選択肢が右端の場合は左端を選択する。*/
		if (m_currentSelect == enSelect_Stage2)
		{
			m_currentSelect = enSelect_Stage1;
			return;
		}
		m_currentSelect = static_cast<EnSelect>(static_cast<int>(m_currentSelect) + 1);
	}

	/*選択を表す枠の位置を変える。*/
	m_frame.SetPosition({ m_stageIcon[m_currentSelect].GetPosition().x, m_frame.GetPosition().y , m_frame.GetPosition().z });

	/*Aボタンを押したらインゲームに切り替えるためにロードする。*/
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		FadeManager::GetInstance()->ChangeFade(FadeManager::enFade_Out);
		FadeManager::GetInstance()->SetTargetFede(LoadingManager::GetInstance()->GetBackGround());
		FadeManager::GetInstance()->SetTargetFede(LoadingManager::GetInstance()->GetLoadingTire());
		return;
	}

	/*Bボタンを押したらタイトルシーンに戻る。*/
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		UIManager::GetInstance()->EnableDrawUI(&m_titleBackGround);
		UIManager::GetInstance()->EnableDrawUI(&m_titleLogo);

		UIManager::GetInstance()->DisableDrawUI(&m_selectBackGround);
		UIManager::GetInstance()->DisableDrawUI(&m_stageIcon[enSelect_Stage1]);
		UIManager::GetInstance()->DisableDrawUI(&m_stageIcon[enSelect_Stage2]);
		UIManager::GetInstance()->DisableDrawUI(&m_frame);
		UIManager::GetInstance()->DisableDrawUI(&m_aButton);
		UIManager::GetInstance()->DisableDrawUI(&m_bButton);

		m_currentScene = enScene_Title;
	}
}
