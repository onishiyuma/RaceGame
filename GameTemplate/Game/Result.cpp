#include "stdafx.h"
#include "Result.h"
#include "Scene/TitleScene.h"
#include "Scene/SceneManager.h"

namespace {
	Vector4 SELECT_ICON_APLHA = { 1.0f,1.0f,1.0f,1.0f };/*選択されているアイコンの透明度。*/

	Vector4 NOT_SELECT_ICON_APLHA = { 1.0f,1.0f,1.0f,0.7f };/*選択されていないアイコンの透明度。*/
}

Result::~Result()
{
	UIManager::GetInstance()->DeleteUIs(m_normalFrames);
	UIManager::GetInstance()->DeleteUIs(m_playerFrames);
	UIManager::GetInstance()->DeleteUIs(m_normalIcons);
	UIManager::GetInstance()->DeleteUIs(m_playerIcons);
	UIManager::GetInstance()->DeleteUIs(m_normalRankIcons);
	UIManager::GetInstance()->DeleteUIs(m_playerRankIcons);

	UIManager::GetInstance()->DeleteUI(&m_selectBackGround);

	for (int i = 0; i < enSelect_Num; i++)
	{
		UIManager::GetInstance()->DeleteUI(&m_selectOption[i]);
	}

	UIManager::GetInstance()->DeleteUI(&m_selectFrame);
}

bool Result::OnLoad()
{
	/*下記のPushPlayerDataはデバッグ用なので後で消す。*/
	PushPlayerData(Result::enPlayerIcon_1P, 8);
	PushPlayerData(Result::enPlayerIcon_2P, 7);
	PushPlayerData(Result::enPlayerIcon_CPU1, 6);
	PushPlayerData(Result::enPlayerIcon_CPU2, 5);
	PushPlayerData(Result::enPlayerIcon_CPU3, 4);
	PushPlayerData(Result::enPlayerIcon_CPU4, 3);
	PushPlayerData(Result::enPlayerIcon_CPU5, 2);
	PushPlayerData(Result::enPlayerIcon_CPU6, 1);

	/*プレイヤーのデータを順位が高い順にソートする。*/
	std::sort(m_playerDatas.begin(), m_playerDatas.end(), [](const PlayerData& a, const PlayerData& b) {
		return a.rank < b.rank;
	});

	/*UIの初期化。*/
	InitUI();

	/*アニメーションの初期化。*/
	InitAnimation();

	/*プレイヤー側のUIと通常のUIの表示切り替え。*/
	DisplayPlayerOrNormalUI();

	return true;
}

void Result::OnUpdate(float deltaTime)
{
	/*ロードしている場合は処理しない。*/
	if (LoadingUI::GetInstance()->IsLoading())
	{
		/*フェードアウトが終了したら処理する。*/
		if (FadeManager::GetInstance()->IsEnd(FadeManager::enFade_Out))
		{
			/*1.5秒経過したら処理する。*/
			if (m_delayTime.Execute(1.5f))
			{
				/*シーンの切り替え。*/
				TransitionScene();
			}
		}
		return;
	}

	/*演出が終わっていなければ処理する。*/
	if (!m_isEndDirection)
	{
		/*演出の更新処理。*/
		UpdateDirection();
		return;
	}

	/*Aボタンが押されていなければ。*/
	if (!m_isTriggerButtonA)
	{
		/*プレイヤー側の操作。*/
		Operation();
	}
	/*選択シーンのときにAボタンが押されたら。*/
	else if (m_isTriggerButtonA && m_currentScene == enScene_Select)
	{
		/*決定アニメーションの更新処理。*/
		UpdateDecisionAnimation();
	}
}

/*UIの初期化。*/
void Result::InitUI()
{
	for (const auto playerData : m_playerDatas)
	{
		/*枠の初期化。*/
		InitFrame(playerData);

		/*プレイヤーランクの初期化。*/
		InitPlayerIcon(playerData);

		/*ランクアイコンの初期化。*/
		InitRankIcon(playerData);
	}

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUIs(m_normalFrames);
	UIManager::GetInstance()->AddUIs(m_playerFrames);
	UIManager::GetInstance()->AddUIs(m_normalIcons);
	UIManager::GetInstance()->AddUIs(m_playerIcons);
	UIManager::GetInstance()->AddUIs(m_normalRankIcons);
	UIManager::GetInstance()->AddUIs(m_playerRankIcons);

	/*リザルト後の選択用の背景の初期化。*/
	InitSelectBackGround();

	for (int i = 0; i < enSelect_Num; i++)
	{
		/*リザルト後の選択用の選択肢の初期化。*/
		InitSelectOption(i);
	}

	/*リザルト後の選択用の枠の初期化。*/
	InitSelectFrame();
}

/*枠の初期化。*/
void Result::InitFrame(PlayerData playerData)
{
	auto normalFrame = std::make_unique<SpriteRender>();
	normalFrame->Init(m_frameFilePath.c_str(), 1920, 1080);
	normalFrame->SetPosition({ 1050.0f,350.0f - (100.0f * (playerData.rank - 1)),0.0f });
	normalFrame->SetScale({ 0.8f,0.8f,1.0f });
	normalFrame->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	normalFrame->Update();
	m_normalFrames.push_back(std::move(normalFrame));

	if (playerData.playerType == enPlayerIcon_1P || playerData.playerType == enPlayerIcon_2P)
	{
		auto playerFrame = std::make_unique<SpriteRender>();
		playerFrame->Init(m_playerFrameFilePath.c_str(), 1920, 1080);
		playerFrame->SetPosition({ 1050.0f,350.0f - (100.0f * (playerData.rank - 1)),0.0f });
		playerFrame->SetScale({ 0.8f,0.8f,1.0f });
		playerFrame->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		playerFrame->DisableDraw();
		playerFrame->Update();

		m_playerFrames.push_back(std::move(playerFrame));
	}
	else
	{
		auto dummy = std::make_unique<SpriteRender>();
		dummy->Init("", 1920, 1080);

		m_playerFrames.push_back(std::move(dummy));
	}
}

/*プレイヤーアイコンの初期化。*/
void Result::InitPlayerIcon(PlayerData playerData)
{
	auto normalIcon = std::make_unique<SpriteRender>();
	normalIcon->Init(m_normalIconFilePath[playerData.playerType].c_str(), 1920, 1080);
	normalIcon->SetPosition({ 1095.0f,350.0f - (100.0f * (playerData.rank - 1)),0.0f });
	normalIcon->SetScale({ 0.4f,0.4f,1.0f });
	normalIcon->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	normalIcon->Update();

	m_normalIcons.push_back(std::move(normalIcon));

	if (playerData.playerType == enPlayerIcon_1P || playerData.playerType == enPlayerIcon_2P)
	{
		auto playerIcon = std::make_unique<SpriteRender>();
		playerIcon->Init(m_playerIconFilePath[playerData.playerType].c_str(), 1920, 1080);
		playerIcon->SetPosition({ 1095.0f,350.0f - (100.0f * (playerData.rank - 1)),0.0f });
		playerIcon->SetScale({ 0.4f,0.4f,1.0f });
		playerIcon->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		playerIcon->DisableDraw();
		playerIcon->Update();

		m_playerIcons.push_back(std::move(playerIcon));
	}
	else
	{
		auto dummy = std::make_unique<SpriteRender>();
		dummy->Init("", 1920, 1080);

		m_playerIcons.push_back(std::move(dummy));
	}
}

/*ランクアイコンの初期化。*/
void Result::InitRankIcon(PlayerData playerData)
{
	auto normaRankIcon = std::make_unique<SpriteRender>();
	normaRankIcon->Init(m_normalRankIconFilePath[playerData.rank - 1].c_str(), 1920, 1080);
	normaRankIcon->SetPosition({ 865.0f,350.0f - (100.0f * (playerData.rank - 1)),0.0f });
	normaRankIcon->SetScale({ 0.4f,0.4f,1.0f });
	normaRankIcon->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	normaRankIcon->Update();
	
	m_normalRankIcons.push_back(std::move(normaRankIcon));

	if (playerData.playerType == enPlayerIcon_1P || playerData.playerType == enPlayerIcon_2P)
	{
		auto playerRankIcon = std::make_unique<SpriteRender>();
		playerRankIcon->Init(m_playerRankIconFilePath[playerData.rank - 1].c_str(), 1920, 1080);
		playerRankIcon->SetPosition({ 865.0f,350.0f - (100.0f * (playerData.rank - 1)),0.0f });
		playerRankIcon->SetScale({ 0.4f,0.4f,1.0f });
		playerRankIcon->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		playerRankIcon->DisableDraw();
		playerRankIcon->Update();

		m_playerRankIcons.push_back(std::move(playerRankIcon));
	}
	else
	{
		auto dummy = std::make_unique<SpriteRender>();
		dummy->Init("", 1920, 1080);

		m_playerRankIcons.push_back(std::move(dummy));
	}
}

/*リザルト後の選択用の背景の初期化。*/
void Result::InitSelectBackGround()
{
	m_selectBackGround.Init("Assets/UI/result/select/resultBack.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	m_selectBackGround.SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	m_selectBackGround.DisableDraw();
	m_selectBackGround.Update();

	UIManager::GetInstance()->AddUI(&m_selectBackGround);
}

/*リザルト後の選択用の選択肢の初期化。*/
void Result::InitSelectOption(int index)
{
	m_selectOption[index].Init(m_resultOptionFilePath[index].c_str(), 1920, 1080);
	m_selectOption[index].SetPosition({0.0f,275.0f - (175.0f * index),0.0f});
	m_selectOption[index].SetScale({ 0.5f,0.5f,1.0f });
	m_selectOption[index].DisableDraw();
	m_selectOption[index].Update();

	UIManager::GetInstance()->AddUI(&m_selectOption[index]);
}

/*リザルト後の選択用の枠の初期化。*/
void Result::InitSelectFrame()
{
	m_selectFrame.Init("Assets/UI/result/select/resultFrame.dds", 1920, 1080);
	m_selectFrame.SetPosition({ 5.0f,240.0f,0.0f });
	m_selectFrame.SetScale({0.5f,0.5f,1.0f});
	m_selectFrame.DisableDraw();
	m_selectFrame.Update();

	UIManager::GetInstance()->AddUI(&m_selectFrame);
}

/*アニメーションの初期化。*/
void Result::InitAnimation()
{
	/*スライドアニメーションの初期化。*/
	/*透明度を変えるアニメーションの初期化。*/
	for (int i = 0; i < m_normalFrames.size(); i++)
	{
		InitSlideAnimation(m_normalFrames[i], i);
		InitAlphaAnimation(m_normalFrames[i], i);

		if (m_playerDatas[i].playerType == enPlayerIcon_1P || m_playerDatas[i].playerType == enPlayerIcon_2P)
		{
			InitSlideAnimation(m_playerFrames[i], i);
			InitAlphaAnimation(m_playerFrames[i], i);
		}
	}

	for (int i = 0; i < m_normalIcons.size(); i++)
	{
		InitSlideAnimation(m_normalIcons[i], i);
		InitAlphaAnimation(m_normalIcons[i], i);

		if (m_playerDatas[i].playerType == enPlayerIcon_1P || m_playerDatas[i].playerType == enPlayerIcon_2P)
		{
			InitSlideAnimation(m_playerIcons[i], i);
			InitAlphaAnimation(m_playerIcons[i], i);
		}
	}

	for (int i = 0; i < m_normalRankIcons.size(); i++)
	{
		InitSlideAnimation(m_normalRankIcons[i], i);
		InitAlphaAnimation(m_normalRankIcons[i], i);

		if (m_playerDatas[i].playerType == enPlayerIcon_1P || m_playerDatas[i].playerType == enPlayerIcon_2P)
		{
			InitSlideAnimation(m_playerRankIcons[i], i);
			InitAlphaAnimation(m_playerRankIcons[i], i);
		}
	}

	/*拡大縮小させるアニメーションの初期化。*/
	InitScaleAnimation();

	/*決定アニメーションの初期化。*/
	InitDecisionAnimation();
}

/*スライドアニメーションの初期化。*/
void Result::InitSlideAnimation(std::unique_ptr<SpriteRender>& targetSprite, int index)
{
	auto slideAnimation = std::make_unique<PositionUIAnimation>();
	slideAnimation->SetRender(targetSprite.get()).SetPlaySpeed(6.5f).SetStartDelayTime(0.1f * index).SetEndDelayTime(0.5f).SetRelativePositionX(-600.0f);

	m_slideAnimations.push_back(std::move(slideAnimation));
}

/*透明度を変えるアニメーションの初期化。*/
void Result::InitAlphaAnimation(std::unique_ptr<SpriteRender>& targetSprite, int index)
{
	auto alphaAnimation = std::make_unique<AlphaUIAnimation>();
	alphaAnimation->SetRender(targetSprite.get()).SetPlaySpeed(8.5f).SetStartDelayTime(0.1f * index).SetEndDelayTime(0.5f).SetRelativeAlpha(1.0f);

	m_alphaAnimations.push_back(std::move(alphaAnimation));
}

/*スライドアニメーションに登録されている情報の切り替え。*/
void Result::ChangeSlideAnimationData(std::unique_ptr<SpriteRender>& targetSprite, int index)
{
	auto it = std::find_if(
		m_slideAnimations.begin(),
		m_slideAnimations.end(),
		[&](const std::unique_ptr<PositionUIAnimation>& anim) {
			return anim->GetRender() == targetSprite.get();
		}
	);
	if (it != m_slideAnimations.end())
	{
		(*it)->SetStartDelayTime(0.1f * index).SetEndDelayTime(0.1f).SetRelativePositionX(-450.0f);
		(*it)->Reset();
	}
}

/*拡大縮小させるアニメーションの初期化。*/
void Result::InitScaleAnimation()
{
	m_scaleAnimation = std::make_unique<ScaleUIAnimation>();
	m_scaleAnimation->SetRender(&m_selectFrame).SetPlaySpeed(2.7f).SetLoop(true).SetBaseScale({ m_selectFrame.GetScale().x,m_selectFrame.GetScale().y }).SetTargetScale({ 0.485f,0.485f });
}

/*決定アニメーションの初期化。*/
void Result::InitDecisionAnimation()
{
	m_decisionAnimation = std::make_unique<ScaleUIAnimation>();
	m_decisionAnimation->SetRender(&m_selectOption[enSelect_PlayGame]).SetPlaySpeed(5.0f).SetRelativeScale(-0.05f);
}

/*透明度を変えるアニメーションに登録されている情報の切り替え。*/
void Result::ChangeAlphaAnimationData(std::unique_ptr<SpriteRender>& targetSprite, int index)
{
	auto it = std::find_if(
		m_alphaAnimations.begin(),
		m_alphaAnimations.end(),
		[&](const std::unique_ptr<AlphaUIAnimation>& anim) {
			return anim->GetRender() == targetSprite.get();
		}
	);
	if (it != m_alphaAnimations.end())
	{
		(*it)->SetStartDelayTime(0.1f * index).SetEndDelayTime(0.1f).SetRelativeAlpha(-1.0f);
		(*it)->Reset();
	}
}

/*プレイヤー側のUIと通常のUIの表示切り替え。*/
void Result::DisplayPlayerOrNormalUI()
{
	GamePad gamePad;
	for (int i = 0; i < m_playerDatas.size(); i++)
	{
		if (EnPlayerIcon(m_playerDatas[i].playerType) == enPlayerIcon_1P)
		{
			m_playerFrames[i]->EnableDraw();
			m_normalFrames[i]->DisableDraw();

			m_playerIcons[i]->EnableDraw();
			m_normalIcons[i]->DisableDraw();

			m_playerRankIcons[i]->EnableDraw();
			m_normalRankIcons[i]->DisableDraw();
		}
		else if (EnPlayerIcon(m_playerDatas[i].playerType) == enPlayerIcon_2P)
		{
			if (gamePad.GetPadState(enGamePad_2P) == GamePad::EnXInputPadState::Connect)
			{
				m_playerFrames[i]->EnableDraw();
				m_normalFrames[i]->DisableDraw();

				m_playerIcons[i]->EnableDraw();
				m_normalIcons[i]->DisableDraw();

				m_playerRankIcons[i]->EnableDraw();
				m_normalRankIcons[i]->DisableDraw();
			}
			else
			{
				m_normalFrames[i]->EnableDraw();

				m_normalIcons[i]->EnableDraw();

				m_normalRankIcons[i]->EnableDraw();
			}
		}
	}
}

/*スライドアニメーションの更新処理。*/
void Result::UpdateSlide()
{
	for (auto& slideAnimation : m_slideAnimations)
	{
		slideAnimation->Update();
	}
}

/*透明度を変えるアニメーションの更新処理。*/
void Result::UpdateAlpha()
{
	for (auto& alphaAnimation : m_alphaAnimations)
	{
		alphaAnimation->Update();
	}
}

/*シーンの切り替え。*/
void Result::TransitionScene()
{
	auto sceneManager = GetSceneManager();
	if (sceneManager) {
		/*選択している内容で遷移する。*/
		switch (m_currentSelect)
		{
		case Result::enSelect_PlayGame:/*もういちど遊ぶ。*/
			break;
		case Result::enSelect_Sticker:/*ステッカー選択へ。*/
			break;
		case Result::enSelect_Stage:/*ステージ選択へ。*/
			break;
		case Result::enSelect_Title:/*タイトルへ。*/
			sceneManager->ChangeScene(std::make_unique<TitleScene>());
			break;
		default:
			break;
		}
	}
}

/*演出の更新処理。*/
void Result::UpdateDirection()
{
	/*現在の演出。*/
	switch (m_currentDirection)
	{
	case enDirection_Ranking:/*ランキング。*/
		UpdateRankingDirection();
		break;

	case enDirection_Select:/*選択。*/
		UpdateSelectDirection();
		break;
	default:
		break;
	}
}

/*ランキング用の演出の更新処理。*/
void Result::UpdateRankingDirection()
{
	UpdateSlide();
	UpdateAlpha();

	/*演出が終わったら。*/
	if (m_slideAnimations[m_slideAnimations.size() - 1]->IsEnd())
	{
		/*Aボタンを押されていたらランキング用の演出が2回目のため選択用の演出に切り替える。*/
		if (m_isTriggerButtonA)
		{
			/*現在の演出とシーンの切り替え。*/
			m_currentDirection = enDirection_Select;
			m_currentScene = enScene_Select;

			/*透明度を変えるアニメーションの再設定。*/
			m_alphaAnimations.clear();

			auto alphaAnimation = std::make_unique<AlphaUIAnimation>();
			alphaAnimation->SetRender(&m_selectBackGround).SetPlaySpeed(2.0f).SetRelativeAlpha(1.0f);

			m_alphaAnimations.push_back(std::move(alphaAnimation));
			UIManager::GetInstance()->EnableDrawUI(&m_selectBackGround);

			/*もう一度同じ演出を流す。*/
			m_isTriggerButtonA = false;
		}
		/*プレイヤー側の操作ができるようにする。*/
		else
		{
			/*演出終了。*/
			m_isEndDirection = true;
		}
	}
}

/*選択用の演出の更新処理。*/
void Result::UpdateSelectDirection()
{
	UpdateAlpha();

	/*演出が終わったら選択用のUIを表示するかつプレイヤー側の操作ができるようにする。*/
	if (m_alphaAnimations[m_alphaAnimations.size() - 1]->IsEnd())
	{
		m_isEndDirection = true;

		for (int i = 0; i < enSelect_Num; i++)
		{
			UIManager::GetInstance()->EnableDrawUI(&m_selectOption[i]);
		}
		UIManager::GetInstance()->EnableDrawUI(&m_selectFrame);
	}
}

/*拡大縮小させるアニメーションの更新処理。*/
void Result::UpdateScaleAnimation()
{
	m_scaleAnimation->Update();
}

/*決定アニメーションの更新処理。*/
void Result::UpdateDecisionAnimation()
{
	m_decisionAnimation->SetRender(&m_selectOption[m_currentSelect]);
	m_decisionAnimation->Update();

	/*アニメーションを最後まで再生したか？*/
	if (m_decisionAnimation->IsEnd())
	{
		/*このアニメーションは逆再生していないか？*/
		if (!m_decisionAnimation->IsReverse())
		{
			/*逆再生する。*/
			m_decisionAnimation->EnableReverse();
		}
		/*このアニメーションは逆再生しているか？*/
		else
		{
			/*フェードアウトに切り替える。*/
			FadeManager::GetInstance()->ChangeFade(FadeManager::enFade_Out);
		}
	}
}

/*プレイヤー側の操作。*/
void Result::Operation()
{
	/*現在のシーン。*/
	switch (m_currentScene)
	{
	case Result::enScene_Ranking:/*ランキング。*/
		TriggerButtonA();
		break;
	case Result::enScene_Select:/*選択。*/
		Select();
		break;
	default:
		break;
	}
}

/*Aボタンを押す。*/
void Result::TriggerButtonA()
{
	/*Aボタンが押されたら2回目のランキング用の演出を流す。*/
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_isTriggerButtonA = true;

		for (int i = 0; i < m_normalFrames.size(); i++)
		{
			ChangeSlideAnimationData(m_normalFrames[i], i);
			ChangeAlphaAnimationData(m_normalFrames[i], i);

			if (m_playerDatas[i].playerType == enPlayerIcon_1P || m_playerDatas[i].playerType == enPlayerIcon_2P)
			{
				ChangeSlideAnimationData(m_playerFrames[i], i);
				ChangeAlphaAnimationData(m_playerFrames[i], i);
			}
		}

		for (int i = 0; i < m_normalIcons.size(); i++)
		{
			ChangeSlideAnimationData(m_normalIcons[i], i);
			ChangeAlphaAnimationData(m_normalIcons[i], i);

			if (m_playerDatas[i].playerType == enPlayerIcon_1P || m_playerDatas[i].playerType == enPlayerIcon_2P)
			{
				ChangeSlideAnimationData(m_playerIcons[i], i);
				ChangeAlphaAnimationData(m_playerIcons[i], i);
			}
		}

		for (int i = 0; i < m_normalRankIcons.size(); i++)
		{
			ChangeSlideAnimationData(m_normalRankIcons[i], i);
			ChangeAlphaAnimationData(m_normalRankIcons[i], i);

			if (m_playerDatas[i].playerType == enPlayerIcon_1P || m_playerDatas[i].playerType == enPlayerIcon_2P)
			{
				ChangeSlideAnimationData(m_playerRankIcons[i], i);
				ChangeAlphaAnimationData(m_playerRankIcons[i], i);
			}
		}

		m_isEndDirection = false;
	}
}

/*選択。*/
void Result::Select()
{
	m_scaleAnimation->Update();

	/*上に選択。*/
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		/*SEを流す。*/
		GameSoundEngine::GetInstance()->PlaySE(SEList_Select, 4.0f, true);

		m_scaleAnimation->Reset();
		if (m_currentSelect == enSelect_PlayGame)
		{
			m_currentSelect = enSelect_Title;
			return;
		}
		m_currentSelect = static_cast<EnSelect>(static_cast<int>(m_currentSelect) - 1);
	}
	/*下に選択。*/
	else if (g_pad[0]->IsTrigger(enButtonDown))
	{
		/*SEを流す。*/
		GameSoundEngine::GetInstance()->PlaySE(SEList_Select, 4.0f, true);

		m_scaleAnimation->Reset();
		if (m_currentSelect == enSelect_Title)
		{
			m_currentSelect = enSelect_PlayGame;
			return;
		}
		m_currentSelect = static_cast<EnSelect>(static_cast<int>(m_currentSelect) + 1);
	}

	/*現在の選択を表す枠の位置を変える。*/
	m_selectFrame.SetPosition({ m_selectFrame.GetPosition().x, m_selectOption[m_currentSelect].GetPosition().y - 35.0f, m_selectFrame.GetPosition().z });

	for (int i = 0; i < enSelect_Num; i++)
	{
		if (m_currentSelect == i)
		{
			/*選択している状態。*/
			m_selectOption[i].SetMulColor(SELECT_ICON_APLHA);
		}
		else
		{
			/*選択していない状態。*/
			m_selectOption[i].SetMulColor(NOT_SELECT_ICON_APLHA);
		}
	}

	/*Aボタンを押したらタイトルシーンに切り替えるためロードする。*/
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_isTriggerButtonA = true;

		/*SEを流す。*/
		GameSoundEngine::GetInstance()->PlaySE(SEList_Decision, 4.0f, true);
	}
}