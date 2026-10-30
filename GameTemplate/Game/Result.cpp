#include "stdafx.h"
#include "Result.h"
#include "Scene/TitleScene.h"
#include "Scene/SceneManager.h"

Result::~Result()
{
	for (auto& frame : m_frames)
	{
		UIManager::GetInstance()->DeleteUI(frame.get());
	}

	for (auto& playerIcon : m_playerIcons)
	{
		UIManager::GetInstance()->DeleteUI(playerIcon.get());
	}

	for (auto& rankIcon : m_rankIcons)
	{
		UIManager::GetInstance()->DeleteUI(rankIcon.get());
	}

	UIManager::GetInstance()->DeleteUI(&m_resultBackGround);

	for (int i = 0; i < enSelect_Num; i++)
	{
		UIManager::GetInstance()->DeleteUI(&m_resultOption[i]);
	}

	UIManager::GetInstance()->DeleteUI(&m_resultFrame);
}

bool Result::OnLoad()
{
	/*下記のPushPlayerDataはデバッグ用なので後で消す。*/
	PushPlayerData(Result::enPlayerIcon_1P, 1);
	PushPlayerData(Result::enPlayerIcon_2P, 2);
	PushPlayerData(Result::enPlayerIcon_CPU1, 3);
	PushPlayerData(Result::enPlayerIcon_CPU2, 4);
	PushPlayerData(Result::enPlayerIcon_CPU3, 5);

	/*UIの初期化。*/
	InitUI();

	/*アニメーションの初期化。*/
	InitAnimation();

	return true;
}

void Result::OnUpdate()
{
	/*ロードしている場合は処理しない。*/
	if (LoadingManager::GetInstance()->IsLoading())
	{
		if (FadeManager::GetInstance()->IsEnd(FadeManager::enFade_Out))
		{
			if (m_delayTime.Execute(1.5f))
			{
				auto sceneManager = GetSceneManager();
				if (sceneManager) {
					/*選択してる内容で遷移する。*/
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
		}
		return;
	}

	/*演出が終わっていなければ処理する。*/
	if (!m_isEndDirection)
	{
		/*現在の演出。*/
		switch (m_currentDirection)
		{
		case enDirection_Ranking:/*ランキング。*/
			UpdateSlide();
			UpdateAlpha();

			/*演出が終わったら。*/
			if (m_slideAnimations[m_slideAnimations.size() - 1]->IsEnd())
			{
				/*Aボタンを押されていたらランキング用の演出が2回目のため選択用の演出に切り替える。*/
				if (m_isTriggerButtonA)
				{
					m_currentDirection = enDirection_Select;
					m_currentScene = enScene_Select;

					m_alphaAnimations.clear();
					
					auto alphaAnimation = std::make_unique<AlphaUIAnimation>();
					alphaAnimation->SetRender(&m_resultBackGround).SetPlaySpeed(2.0f).SetRelativeAlpha(1.0f);

					m_alphaAnimations.push_back(std::move(alphaAnimation));
					UIManager::GetInstance()->EnableDrawUI(&m_resultBackGround);

					m_isTriggerButtonA = false;
				}
				/*プレイヤー側の操作ができるようにする。*/
				else
				{
					m_isEndDirection = true;
				}
			}

			break;

		case enDirection_Select:/*選択。*/
			UpdateAlpha();

			/*演出が終わったら選択用のUIを表示するかつプレイヤー側の操作ができるようにする。*/
			if (m_alphaAnimations[m_alphaAnimations.size() - 1]->IsEnd())
			{
				m_isEndDirection = true;

				for (int i=0; i<enSelect_Num; i++)
				{
					UIManager::GetInstance()->EnableDrawUI(&m_resultOption[i]);
				}
				UIManager::GetInstance()->EnableDrawUI(&m_resultFrame);
			}
			break;
		default:
			break;
		}
		return;
	}

	/*プレイヤー側の操作。*/
	Operation();
}

/*UIの初期化。*/
void Result::InitUI()
{
	for (const auto playerData : m_playerDatas)
	{
		/*枠の初期化。*/
		InitFrame(playerData.rank);

		/*プレイヤーアイコンの初期化。*/
		InitPlayerIcon(playerData);

		/*ランクアイコンの初期化。*/
		InitRankIcon(playerData.rank);
	}

	/*UIマネージャーに追加。*/
	for (auto& frame : m_frames)
	{
		UIManager::GetInstance()->AddUI(frame.get());
	}

	for (auto& playerIcon : m_playerIcons)
	{
		UIManager::GetInstance()->AddUI(playerIcon.get());
	}

	for (auto& rankIcon :m_rankIcons)
	{
		UIManager::GetInstance()->AddUI(rankIcon.get());
	}

	/*リザルト後の選択用の背景の初期化。*/
	InitReslutBackGround();

	for (int i = 0; i < enSelect_Num; i++)
	{
		/*リザルト後の選択用の選択肢の初期化。*/
		InitResultOption(i);
	}

	/*リザルト後の選択用の枠の初期化。*/
	InitResultFrame();
}

/*プレイヤーアイコンの初期化。*/
void Result::InitFrame(int rank)
{
	auto frame = std::make_unique<SpriteRender>();
	frame->Init(m_frameFilePath.c_str(), 1920, 1080);
	frame->SetPosition({ 900.0f,300.0f - (125.0f * (rank - 1)),0.0f });
	frame->SetScale({ 1.0f,1.0f,1.0f });
	frame->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	frame->Update();

	m_frames.push_back(std::move(frame));
}

/*プレイヤーアイコンの初期化。*/
void Result::InitPlayerIcon(PlayerData playerData)
{
	auto playerIcon = std::make_unique<SpriteRender>();
	playerIcon->Init(m_playerIconFilePath[playerData.playerType].c_str(), 1920, 1080);
	playerIcon->SetPosition({ 965.0f,300.0f - (125.0f * (playerData.rank - 1)),0.0f });
	playerIcon->SetScale({ 0.6f,0.6f,1.0f });
	playerIcon->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	playerIcon->Update();

	m_playerIcons.push_back(std::move(playerIcon));
}

/*ランクアイコンの初期化。*/
void Result::InitRankIcon(int rank)
{
	auto rankIcon = std::make_unique<SpriteRender>();
	rankIcon->Init(m_rankIconFilePath[rank - 1].c_str(), 1920, 1080);
	rankIcon->SetPosition({ 670.0f,300.0f - (125.0f * (rank - 1)),0.0f });
	rankIcon->SetScale({ 0.6f,0.6f,1.0f });
	rankIcon->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	rankIcon->Update();
	
	m_rankIcons.push_back(std::move(rankIcon));
}

/*リザルト後の選択用の背景の初期化。*/
void Result::InitReslutBackGround()
{
	m_resultBackGround.Init("Assets/UI/result/select/resultBack.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	m_resultBackGround.SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	m_resultBackGround.DisableDraw();
	m_resultBackGround.Update();

	UIManager::GetInstance()->AddUI(&m_resultBackGround);
}

/*リザルト後の選択用の選択肢の初期化。*/
void Result::InitResultOption(int index)
{
	m_resultOption[index].Init(m_resultOptionFilePath[index].c_str(), 1920, 1080);
	m_resultOption[index].SetPosition({0.0f,275.0f - (175.0f * index),0.0f});
	m_resultOption[index].SetScale({ 0.5f,0.5f,1.0f });
	m_resultOption[index].DisableDraw();
	m_resultOption[index].Update();

	UIManager::GetInstance()->AddUI(&m_resultOption[index]);
}

/*リザルト後の選択用の枠の初期化。*/
void Result::InitResultFrame()
{
	m_resultFrame.Init("Assets/UI/result/select/resultFrame.dds", 1920, 1080);
	m_resultFrame.SetPosition({ 0.0f,240.0f,0.0f });
	m_resultFrame.SetScale({0.5f,0.5f,1.0f});
	m_resultFrame.DisableDraw();
	m_resultFrame.Update();

	UIManager::GetInstance()->AddUI(&m_resultFrame);
}

/*アニメーションの初期化。*/
void Result::InitAnimation()
{
	/*スライドアニメーションの初期化。*/
	/*透明度を変えるアニメーションの初期化。*/
	for (int i = 0; i < m_frames.size(); i++)
	{
		InitSlideAnimation(m_frames[i], i);
		InitAlphaAnimation(m_frames[i], i);
	}

	for (int i = 0; i < m_playerIcons.size(); i++)
	{
		InitSlideAnimation(m_playerIcons[i], i);
		InitAlphaAnimation(m_playerIcons[i], i);
	}

	for (int i = 0; i < m_rankIcons.size(); i++)
	{
		InitSlideAnimation(m_rankIcons[i], i);
		InitAlphaAnimation(m_rankIcons[i], i);
	}
}

/*スライドアニメーションの初期化。*/
void Result::InitSlideAnimation(std::unique_ptr<SpriteRender>& targetSprite, int index)
{
	auto slideAnimation = std::make_unique<PositionUIAnimation>();
	slideAnimation->SetRender(targetSprite.get()).SetPlaySpeed(4.5f).SetStartDelayTime(0.1f * index).SetEndDelayTime(0.5f).SetRelativePositionX(-600.0f);

	m_slideAnimations.push_back(std::move(slideAnimation));
}

/*透明度を変えるアニメーションの初期化。*/
void Result::InitAlphaAnimation(std::unique_ptr<SpriteRender>& targetSprite, int index)
{
	auto alphaAnimation = std::make_unique<AlphaUIAnimation>();
	alphaAnimation->SetRender(targetSprite.get()).SetPlaySpeed(5.5f).SetStartDelayTime(0.1f * index).SetEndDelayTime(0.5f).SetRelativeAlpha(1.0f);

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
		(*it)->SetStartDelayTime(0.1f * index).SetEndDelayTime(0.1f).SetRelativePositionX(-600.0f);
		(*it)->Reset();
	}
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

/*プレイヤー側の操作。*/
void Result::Operation()
{
	if (!m_isTriggerButtonA)
	{
		switch (m_currentScene)
		{
		case Result::enScene_Ranking:
			/*Aボタンが押されたら2回目のランキング用の演出を流す。*/
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				m_isTriggerButtonA = true;

				for (int i = 0; i < m_frames.size(); i++)
				{
					ChangeSlideAnimationData(m_frames[i], i);
					ChangeAlphaAnimationData(m_frames[i], i);
				}

				for (int i = 0; i < m_playerIcons.size(); i++)
				{
					ChangeSlideAnimationData(m_playerIcons[i], i);
					ChangeAlphaAnimationData(m_playerIcons[i], i);
				}

				for (int i = 0; i < m_rankIcons.size(); i++)
				{
					ChangeSlideAnimationData(m_rankIcons[i], i);
					ChangeAlphaAnimationData(m_rankIcons[i], i);
				}

				m_isEndDirection = false;
			}
			break;
		case Result::enScene_Select:
			/*上に選択。*/
			if (g_pad[0]->IsTrigger(enButtonUp))
			{
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
				if (m_currentSelect == enSelect_Title)
				{
					m_currentSelect = enSelect_PlayGame;
					return;
				}
				m_currentSelect = static_cast<EnSelect>(static_cast<int>(m_currentSelect) + 1);
			}

			/*現在の選択を表す枠の位置を変える。*/
			m_resultFrame.SetPosition({ m_resultFrame.GetPosition().x, m_resultOption[m_currentSelect].GetPosition().y - 35.0f, m_resultFrame.GetPosition().z });

			/*Aボタンを押したらタイトルシーンに切り替えるためロードする。*/
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				FadeManager::GetInstance()->ChangeFade(FadeManager::enFade_Out);
				FadeManager::GetInstance()->SetTargetFede(LoadingManager::GetInstance()->GetBackGround());
				FadeManager::GetInstance()->SetTargetFede(LoadingManager::GetInstance()->GetLoadingTire());
			}
			break;
		default:
			break;
		}
	}
}