#include "stdafx.h"
#include "SoundOption.h"

namespace {
	const float KNOB_MOVE_X_MIN = -384.0f;/*つまみがX軸に動かせる最小値。*/

	const float KNOB_MOVE_X_MAX = 384.0f;/*つまみがX軸に動かせる最大値。*/

	const int SOUND_INIT_VOLUME = 50;/*初期音量。*/

	const int SOUND_VOLUME_MAX = 100;/*最大音量。*/

	const int SOUND_VOLUME_MIN = 0;/*最小音量。*/
}

SoundOption::~SoundOption()
{
	UIManager::GetInstance()->DeleteUI(&m_soundBackGround);

	for (int i = 0; i < enSoundType_Num; i++)
	{
		UIManager::GetInstance()->DeleteUI(&m_bar[i]);

		UIManager::GetInstance()->DeleteUI(&m_gauge[i]);

		UIManager::GetInstance()->DeleteUI(&m_soundOptionText[i]);

		UIManager::GetInstance()->DeleteUI(&m_knob[i]);

		for (int j = 0; j < enSoundIconType_Num; j++)
		{
			UIManager::GetInstance()->DeleteUI(&m_soundIcon[i][j]);
		}

		for (int k = 0; k < enPlace_Num; k++)
		{
			for (int l = 0; l < enNumber_Num; l++)
			{
				if (k == enPlace_Hundred && l >= enNumber_Two) { continue; }

				UIManager::GetInstance()->DeleteUI(&m_soundVolumeText[i][k][l]);
			}
		}
	}

	UIManager::GetInstance()->DeleteUI(&m_soundSettingText);

	UIManager::GetInstance()->DeleteUI(&m_returnBButtonText);

	UIManager::GetInstance()->DeleteUI(&m_knobLight);
}

/*開始処理。*/
bool SoundOption::Start()
{
	/*UIの初期化。*/
	InitUI();

	/*アニメーションの初期化。*/
	InitAnimation();

	/*サウンド音量の初期化。*/
	InitSoundVolume();

	/*更新処理の無効化。*/
	DisableUpdate();

	return true;
}

/*更新処理。*/
void SoundOption::Update()
{
	/*プレイヤー側の操作。*/
	Operation();

	/*このクラスがアクティブ状態ではなければ処理しない。*/
	if (!IsActive()) { return; }

	/*各音量のテキストを表示するための更新処理。*/
	UpdateDisplaySoundVolumeText();

	for (int i = 0; i < enSoundType_Num; i++)
	{
		/*音量を設定したら各サウンドごとに反映させる。*/
		GameSoundEngine::GetInstance()->SetVolumeRate((GameSoundType)i, CalcVolumeRate((EnSoundType)i));
	}

	/*スライドしたら。*/
	if (m_isSlide)
	{
		/*SEの音量プレビュー。*/
		PreviewSeSoundVolume();
	}

	/*アニメーションの更新処理。*/
	UpdateAnimation();
}

/*UIの初期化。*/
void SoundOption::InitUI()
{
	/*背景の初期化。*/
	InitSoundBackGround();

	for (int i = 0; i < enSoundType_Num; i++)
	{
		/*バーの初期化。*/
		InitBar((EnSoundType)i, i);

		/*ゲージの初期化。*/
		InitGauge((EnSoundType)i, i);

		/*各サウンドを設定する項目テキストの初期化。*/
		InitSoundOptionText((EnSoundType)i, i);

		/*つまみの初期化。*/
		InitKnob((EnSoundType)i, i);

		for (int j = 0; j < enSoundIconType_Num; j++)
		{
			/*サウンドアイコンの初期化。*/
			InitSoundIcon((EnSoundType)i, (EnSoundIconType)j, i, j);
		}

		for (int k = 0; k < enPlace_Num; k++)
		{
			for (int l = 0; l < enNumber_Num; l++)
			{
				if (k == enPlace_Hundred && l >= enNumber_Two) { continue; }

				/*各音量のテキストの初期化。*/
				InitSoundVolumeText((EnSoundType)i, (EnPlace)k, (EnNumber)l, i, k);
			}
		}
	}

	/*サウンド設定テキストの初期化。*/
	InitSoundSettingText();

	/*Bボタンで戻るテキストの初期化。*/
	InitReturnBButtonText();

	/*光るつまみの初期化。*/
	InitKnobLight();
}

/*背景の初期化。*/
void SoundOption::InitSoundBackGround()
{
	m_soundBackGround.Init("Assets/UI/sound/soundBack.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	m_soundBackGround.Update();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_soundBackGround);
}

/*バーの初期化。*/
void SoundOption::InitBar(EnSoundType soundType, int index)
{
	m_bar[soundType].Init("Assets/UI/sound/sound_whiteBar.dds", 1920, 1080);
	m_bar[soundType].SetPosition({ 0.0f,150.0f - (150.0f * index),0.0f });
	m_bar[soundType].SetScale({ 0.4f,0.03f,1.0f });
	m_bar[soundType].Update();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_bar[soundType]);
}

/*ゲージの初期化。*/
void SoundOption::InitGauge(EnSoundType soundType, int index)
{
	m_gauge[soundType].Init("Assets/UI/sound/sound_blueGauge.dds", 1920, 1080);
	m_gauge[soundType].SetPosition({ -384.0f,150.0f - (150.0f * index),0.0f });
	m_gauge[soundType].SetScale({ 0.4f,0.03f,1.0f });
	m_gauge[soundType].SetPivot({ 0.0f,0.5f });
	m_gauge[soundType].Update();

	m_gaugeBaseScale[soundType] = m_gauge[soundType].GetScale();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_gauge[soundType]);
}

/*各サウンドを設定する項目テキストの初期化。*/
void SoundOption::InitSoundOptionText(EnSoundType soundType, int index)
{
	m_soundOptionText[soundType].Init(m_soundOptionTextFilePath[soundType].c_str(), 1920, 1080);
	m_soundOptionText[soundType].SetPosition({ -650.0f,150.0f - (150.0f * index),0.0f });
	m_soundOptionText[soundType].SetScale({ 0.4f,0.4f,1.0f });
	m_soundOptionText[soundType].Update();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_soundOptionText[soundType]);
}

/*つまみの初期化。*/
void SoundOption::InitKnob(EnSoundType soundType, int index)
{
	m_knob[soundType].Init("Assets/UI/sound/sound_knob.dds", 1920, 1080);
	m_knob[soundType].SetPosition({ 0.0f,137.5f - (150.0f * index),0.0f });
	m_knob[soundType].SetScale({ 0.8f,0.8f,1.0f });
	m_knob[soundType].Update();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_knob[soundType], 2);
}

/*サウンドアイコンの初期化。*/
void SoundOption::InitSoundIcon(EnSoundType soundType, EnSoundIconType soundIconType, int soundIndex ,int soundIconIndex)
{
	m_soundIcon[soundType][soundIconType].Init(m_soundIconFilePath[soundIconType].c_str(), 1920, 1080);
	m_soundIcon[soundType][soundIconType].SetPosition({ -450.0f + (950.0f * soundIconIndex),150.0f - (150.0f * soundIndex),0.0f });
	m_soundIcon[soundType][soundIconType].SetScale({ 0.2f,0.2f,1.0f });
	m_soundIcon[soundType][soundIconType].Update();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_soundIcon[soundType][soundIconType]);
}

/*各音量のテキストの初期化。*/
void SoundOption::InitSoundVolumeText(EnSoundType soundType, EnPlace place, EnNumber number, int soundIndex, int placeIndex)
{
	m_soundVolumeText[soundType][place][number].Init(m_soundVolumeTextFilePath[number].c_str(), 1920, 1080);
	m_soundVolumeText[soundType][place][number].SetPosition({ 600.0f + (60.0f * placeIndex),150.0f - (150.0f * soundIndex),0.0f });
	m_soundVolumeText[soundType][place][number].SetScale({ 0.5f,0.5f,1.0f });
	m_soundVolumeText[soundType][place][number].DisableDraw();
	m_soundVolumeText[soundType][place][number].Update();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_soundVolumeText[soundType][place][number]);
}

/*サウンド設定テキストの初期化。*/
void SoundOption::InitSoundSettingText()
{
	m_soundSettingText.Init("Assets/UI/sound/word/soundSettings.dds", 1920, 1080);
	m_soundSettingText.SetPosition({ -455.0f,325.0f,0.0f });
	m_soundSettingText.SetScale({ 0.5f,0.5f,1.0f });
	m_soundSettingText.Update();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_soundSettingText);
}

/*Bボタンで戻るテキストの初期化。*/
void SoundOption::InitReturnBButtonText()
{
	m_returnBButtonText.Init("Assets/UI/sound/word/sound_Bbutton.dds", 1920, 1080);
	m_returnBButtonText.SetPosition({ 625.0f,-325.0f,0.0f });
	m_returnBButtonText.SetScale({ 0.6f,0.6f,1.0f });
	m_returnBButtonText.Update();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_returnBButtonText);
}

/*光るつまみの初期化。*/
void SoundOption::InitKnobLight()
{
	m_knobLight.Init("Assets/UI/sound/sound_knobLight.dds", 1920, 1080);
	m_knobLight.SetPosition({ 0.0f,137.0f,0.0f });
	m_knobLight.SetScale({ 1.2f,0.9f,1.0f });
	m_knobLight.SetMulColor({ 1.0f,1.0f,1.0f,0.8f });
	m_knobLight.Update();

	/*UIマネージャーに追加。*/
	UIManager::GetInstance()->AddUI(&m_knobLight, 1);
}

/*アニメーションの初期化。*/
void SoundOption::InitAnimation()
{
	/*透明度を変えるアニメーションの初期化。*/
	InitAlphaAnimation();
}

/*透明度を変えるアニメーションの初期化。*/
void SoundOption::InitAlphaAnimation()
{
	m_alphaAnimation = std::make_unique<AlphaUIAnimation>();
	m_alphaAnimation->SetRender(&m_knobLight).SetPlaySpeed(1.5f).SetLoop(true).SetRelativeAlpha(-0.6f);
}

/*サウンド音量の初期化。*/
void SoundOption::InitSoundVolume()
{
	for (int i = 0; i < enSoundType_Num; i++)
	{
		m_soundVolumes[i] = GameSoundEngine::GetInstance()->GetVolumeRate((GameSoundType)i) * 100.0f;

		/*ゲージとつまみ関連の更新処理。*/
		UpdateGaugeAndKnob((EnSoundType)i);

		/*音量を表示するためのデータの計算。*/
		CalcVolumeNumverDisplayData((EnSoundType)i, CalcVolumeRate((EnSoundType)i));
	}
}

/*音量を表示するためのデータの計算。*/
void SoundOption::CalcVolumeNumverDisplayData(EnSoundType soundType, float volumeRate)
{
	m_volumeNumverDisplayManager[soundType].clear(); /*音量の数字UIのデータを管理するクラスをリセット。*/
	volumeRate *= 100.0f;							 /*音量の割合を100倍する。*/

	/*百の位を算出するための計算。*/
	int hundred = (int)volumeRate / 100;

	/*十の位を算出するための計算。*/
	int ten = ((int)volumeRate % 100) / 10;

	/*一の位を算出するための計算。*/
	int one = (int)volumeRate % 10;

	/*音量の数字UIのデータの格納。*/
	m_volumeNumverDisplayManager[soundType].push_back((EnNumber)hundred);
	m_volumeNumverDisplayManager[soundType].push_back((EnNumber)ten);
	m_volumeNumverDisplayManager[soundType].push_back((EnNumber)one);
}

/*プレイヤー側の操作。*/
void SoundOption::Operation()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		/*SEを流す。*/
		GameSoundEngine::GetInstance()->PlaySE(SEList_Cancel, 4.0f, true);

		/*更新処理の無効化。*/
		DisableUpdate();
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonX))
	{
		/*サウンド音量のリセット。*/
		ResetSoundVolume();
		return;
	}

	if (g_pad[0]->IsPress(enButtonLeft) || g_pad[0]->IsPress(enButtonRight))
	{
		/*スライド。*/
		Slide();
		return; 
	}
	else 
	{
		/*選択。*/
		Select();
	}
}

/*サウンド音量のリセット。*/
void SoundOption::ResetSoundVolume()
{
	for (int i = 0; i < enSoundType_Num; i++)
	{
		/*各サウンドの音量を初期音量に戻す。*/
		m_soundVolumes[i] = SOUND_INIT_VOLUME;

		/*ゲージとつまみ関連の更新処理。*/
		UpdateGaugeAndKnob((EnSoundType)i);

		/*音量を表示するためのデータの計算。*/
		CalcVolumeNumverDisplayData((EnSoundType)i, CalcVolumeRate((EnSoundType)i));
	}
}

/*選択。*/
void SoundOption::Select()
{
	/*上に選択。*/
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_alphaAnimation->Reset();
		if (m_currentSelectSoundType == enSoundType_Master)
		{
			m_currentSelectSoundType = enSoundType_SE;
			return;
		}
		m_currentSelectSoundType = static_cast<EnSoundType>(static_cast<int>(m_currentSelectSoundType) - 1);
	}
	/*下に選択。*/
	else if (g_pad[0]->IsTrigger(enButtonDown))
	{
		m_alphaAnimation->Reset();
		if (m_currentSelectSoundType == enSoundType_SE)
		{
			m_currentSelectSoundType = enSoundType_Master;
			return;
		}
		m_currentSelectSoundType = static_cast<EnSoundType>(static_cast<int>(m_currentSelectSoundType) + 1);
	}

	/*現在の選択を表すつまみの位置を変える。*/
	m_knobLight.SetPosition({ m_knob[m_currentSelectSoundType].GetPosition().x, m_knob[m_currentSelectSoundType].GetPosition().y - 0.5f, m_knob[m_currentSelectSoundType].GetPosition().z});
}

/*スライド。*/
void SoundOption::Slide()
{
	/*最小音量まで到達していない場合。*/
	if (m_soundVolumes[m_currentSelectSoundType] > SOUND_VOLUME_MIN)
	{
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			m_soundVolumes[m_currentSelectSoundType]--;
			m_isSlide = true;
		}
	}

	/*最大音量まで到達していない場合。*/
	if (m_soundVolumes[m_currentSelectSoundType] < SOUND_VOLUME_MAX)
	{
		if (g_pad[0]->IsPress(enButtonRight))
		{
			m_soundVolumes[m_currentSelectSoundType]++;
			m_isSlide = true;
		}
	}

	/*ゲージとつまみ関連の更新処理。*/
	UpdateGaugeAndKnob(m_currentSelectSoundType);

	/*音量を表示するためのデータの計算。*/
	CalcVolumeNumverDisplayData(m_currentSelectSoundType, CalcVolumeRate(m_currentSelectSoundType));
}

/*ゲージとつまみ関連の更新処理。*/
void SoundOption::UpdateGaugeAndKnob(EnSoundType soundType)
{
	/*音量の割合の計算。*/
	float volumeRate = CalcVolumeRate(soundType);

	/*ゲージの大きさ設定。*/
	Vector3 gaugeScale = m_gaugeBaseScale[soundType];
	gaugeScale.x *= volumeRate;
	m_gauge[soundType].SetScale(gaugeScale);
	m_gauge[soundType].Update();

	/*つまみを移動させるための計算。*/
	float maxLength = KNOB_MOVE_X_MAX - KNOB_MOVE_X_MIN;
	float moveLength = maxLength * (1.0f - volumeRate);

	/*つまみの位置設定。*/
	Vector3 knobPosition = m_knob[soundType].GetPosition();
	knobPosition.x = KNOB_MOVE_X_MAX - moveLength;
	m_knob[soundType].SetPosition(knobPosition);
	m_knob[soundType].Update();

	/*光るつまみの位置設定。*/
	Vector3 knobLightPosition = m_knobLight.GetPosition();
	knobLightPosition.x = KNOB_MOVE_X_MAX - moveLength;
	m_knobLight.SetPosition(knobLightPosition);
	m_knobLight.Update();
}

/*SEの音量プレビュー。*/
void SoundOption::PreviewSeSoundVolume()
{
	/*まだスライドし続けていたら処理をしない。*/
	if (g_pad[0]->IsPress(enButtonLeft) || g_pad[0]->IsPress(enButtonRight)) { return; }

	if (m_currentSelectSoundType == enSoundType_SE)
	{
		/*SEの音量確認、*/
		GameSoundEngine::GetInstance()->PlaySE(SEList_Decision, 4.0f, true);
	}

	/*スライドできていない状態に戻す。*/
	m_isSlide = false;
}

/*各音量のテキストを表示するための更新処理。*/
void SoundOption::UpdateDisplaySoundVolumeText()
{
	for (int i = 0; i < enSoundType_Num; i++)
	{
		for (int j = 0; j < enPlace_Num; j++)
		{
			for (int k = 0; k < enNumber_Num; k++)
			{
				if (j == enPlace_Hundred && k >= enNumber_Two) { continue; }

				if (m_volumeNumverDisplayManager[i][j] == k)
				{
					m_soundVolumeText[i][j][k].EnableDraw();
				}
				else
				{
					m_soundVolumeText[i][j][k].DisableDraw();
				}
			}
		}
	}
}

/*アニメーションの更新処理。*/
void SoundOption::UpdateAnimation()
{
	m_alphaAnimation->Update();
}

/*UI描画の有効化。*/
void SoundOption::EnableDrawUI()
{
	UIManager::GetInstance()->EnableDrawUI(&m_soundBackGround);

	for (int i = 0; i < enSoundType_Num; i++)
	{
		UIManager::GetInstance()->EnableDrawUI(&m_bar[i]);

		UIManager::GetInstance()->EnableDrawUI(&m_gauge[i]);

		UIManager::GetInstance()->EnableDrawUI(&m_soundOptionText[i]);

		UIManager::GetInstance()->EnableDrawUI(&m_knob[i]);

		for (int j = 0; j < enSoundIconType_Num; j++)
		{
			UIManager::GetInstance()->EnableDrawUI(&m_soundIcon[i][j]);
		}

		for (int k = 0; k < enPlace_Num; k++)
		{
			for (int l = 0; l < enNumber_Num; l++)
			{
				UIManager::GetInstance()->EnableDrawUI(&m_soundVolumeText[i][k][l]);
			}
		}
	}

	UIManager::GetInstance()->EnableDrawUI(&m_soundSettingText);

	UIManager::GetInstance()->EnableDrawUI(&m_returnBButtonText);

	UIManager::GetInstance()->EnableDrawUI(&m_knobLight);
}

/*UI描画の無効化。*/
void SoundOption::DisableDrawUI()
{
	UIManager::GetInstance()->DisableDrawUI(&m_soundBackGround);

	for (int i = 0; i < enSoundType_Num; i++)
	{
		UIManager::GetInstance()->DisableDrawUI(&m_bar[i]);

		UIManager::GetInstance()->DisableDrawUI(&m_gauge[i]);

		UIManager::GetInstance()->DisableDrawUI(&m_soundOptionText[i]);

		UIManager::GetInstance()->DisableDrawUI(&m_knob[i]);

		for (int j = 0; j < enSoundIconType_Num; j++)
		{
			UIManager::GetInstance()->DisableDrawUI(&m_soundIcon[i][j]);
		}

		for (int k = 0; k < enPlace_Num; k++)
		{
			for (int l = 0; l < enNumber_Num; l++)
			{
				if (k == enPlace_Hundred && l >= enNumber_Two) { continue; }

				UIManager::GetInstance()->DisableDrawUI(&m_soundVolumeText[i][k][l]);
			}
		}
	}

	UIManager::GetInstance()->DisableDrawUI(&m_soundSettingText);

	UIManager::GetInstance()->DisableDrawUI(&m_returnBButtonText);

	UIManager::GetInstance()->DisableDrawUI(&m_knobLight);
}
