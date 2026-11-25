#include "stdafx.h"
#include "GameSoundEngine.h"
namespace
{
	const int SE_START_ID = 100;/*最初のSEのID。*/
}
GameSoundEngine* GameSoundEngine::m_instance = nullptr;

/*初期化。*/
void GameSoundEngine::Init()
{
	/*BGM。*/
	/*0.タイトル画面BGM。*/
	InitBGM(BGMList_Title, "Assets/sound/bgm/title.wav");

	/*SE。*/
	/*0.決定音SE。*/
	InitSE(SEList_Decision, "Assets/sound/se/decision.wav");
	/*1.選択音SE。*/
	InitSE(SEList_Select, "Assets/sound/se/select.wav");
	/*2.キャンセル音SE。*/
	InitSE(SEList_Cancel, "Assets/sound/se/cancel.wav");
}

/*BGMを再生。*/
void GameSoundEngine::PlayBGM(BGMList bgm, float soundVolume)
{
	if (!IsPlayingBGM(bgm))
	{
		m_bgmSound[bgm] = NewGO<SoundSource>(0);
		m_bgmSound[bgm]->Init(bgm);
		m_bgmSound[bgm]->Play(true);
		m_bgmSound[bgm]->SetVolume(soundVolume);
		m_baseBGMVolume[bgm] = m_bgmSound[bgm]->GetVolume();
		m_bgmSound[bgm]->SetVolume(m_baseBGMVolume[bgm] * m_volumeRate[GameSoundType_BGM] * m_volumeRate[GameSoundType_Master]);
		m_isPlayingBGM[bgm] = true;
	}
}

/*SEを再生。*/
void GameSoundEngine::PlaySE(SEList se, float soundVolume, bool isSoundOption)
{
	if (isSoundOption)
	{
		if (IsPlayingSE(se))
		{
			m_seSound[se]->Stop();
			m_seSound[se]->Play(false);
			m_seSound[se]->SetVolume(m_baseSEVolume[se] * m_volumeRate[GameSoundType_SE] * m_volumeRate[GameSoundType_Master]);
			return;
		}
	}

	m_seSound[se] = NewGO<SoundSource>(0);
	m_seSound[se]->Init(se);
	m_seSound[se]->Play(false);
	m_seSound[se]->SetVolume(soundVolume);
	m_baseSEVolume[se] = m_seSound[se]->GetVolume();
	m_seSound[se]->SetVolume(m_baseSEVolume[se] * m_volumeRate[GameSoundType_SE] * m_volumeRate[GameSoundType_Master]);
	m_isPlayingSE[se] = true;
}

/*更新処理。*/
void GameSoundEngine::Update()
{
	/*BGMを再生しているときに処理する。*/
	for (int i = 0; i < BGMList_Num; i++)
	{
		/*BGMのサウンドインスタンスがnullptrだったら処理をしない。*/
		if (m_bgmSound[i] == nullptr)
		{
			continue;
		}

		/*今再生しているサウンドインスタンスが削除したら。*/
		if (m_bgmSound[i]->IsDead())
		{
			/*サウンドが再生しているかを判定するフラグをfalseにする。*/
			m_isPlayingBGM[i] = false;
			m_baseBGMVolume[i] = 0.0f;
			m_bgmSound[i] = nullptr;
		}
	}

	/*SEのみ処理をする。*/
	for (int i = SE_START_ID; i < SEList_Num; i++)
	{
		/*SEのサウンドインスタンスがnullptrだったら処理をしない。*/
		if (m_seSound[i] == nullptr)
		{
			continue;
		}

		/*今再生しているサウンドインスタンスが削除したら。*/
		if (m_seSound[i]->IsDead())
		{
			/*サウンドが再生しているかを判定するフラグをfalseにする。*/
			m_isPlayingSE[i] = false;
			m_baseSEVolume[i] = 0.0f;
			m_seSound[i] = nullptr;
		}
	}

	/*音量設定。*/
	for (int i = 0; i < BGMList_Num; i++)
	{
		if (m_bgmSound[i] == nullptr)
		{
			continue;
		}

		m_bgmSound[i]->SetVolume(m_baseBGMVolume[i] * m_volumeRate[GameSoundType_BGM] * m_volumeRate[GameSoundType_Master]);
	}

	for (int i = SE_START_ID; i < SEList_Num; i++)
	{
		if (m_seSound[i] == nullptr)
		{
			continue;
		}

		m_seSound[i]->SetVolume(m_baseSEVolume[i] * m_volumeRate[GameSoundType_SE] * m_volumeRate[GameSoundType_Master]);
	}
}