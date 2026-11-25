#pragma once
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
/**
 * @file GameSoundEngine.h。
 * @brief ゲームサウンドエンジンクラス。
 * @author Tanimoto。
 * @date 2026/11/13。
 */

/*サウンドの種類。*/
enum GameSoundType
{
	GameSoundType_Master,/*マスター。*/
	GameSoundType_BGM,/*BGM。*/
	GameSoundType_SE,/*SE。*/
	GameSoundType_Num/*サウンドの種類の数。*/
};

/*BGMリスト。*/
enum BGMList
{
	BGMList_Title,/*タイトル。*/
	BGMList_Num/*BGMリスト数。*/
};

/*SEリスト。*/
enum SEList
{
	SEList_Decision = 100,/*決定音。*/
	SEList_Select = 101,/*選択音。*/
	SEList_Cancel = 102,/*キャンセル音。*/
	SEList_Num/*SEリスト数。*/
};;

/*ゲームサウンドエンジン。*/
class GameSoundEngine : public Noncopyable
{
public:/*メンバ関数。*/

	/**
	 * @brief 初期化。
	 */
	void Init();

	/**
	 * @brief 更新処理。
	 */
	void Update();

	/**
	 * @brief BGMを再生。
	 * @param BGMList BGMリスト。
	 * @param soundVolume サウンドの音量。
	 */
	void PlayBGM(BGMList bgm, float soundVolume);

	/**
	 * @brief SEを再生。
	 * @param SEList SEリスト。
	 * @param soundVolume サウンドの音量。
	 * @param isSoundOption サウンド設定用？ 基本的に使わないからfalseで。
	 */
	void PlaySE(SEList se, float soundVolume, bool isSoundOption = false);

	/**
	 * @brief サウンドの音量の設定。
	 * @param BGMList BGMリスト。
	 * @param soundVolume サウンドの音量。
	 */
	void SetVolume(BGMList bgm, float soundVolume)
	{
		m_bgmSound[bgm]->SetVolume(soundVolume);
	}

	/**
	 * @brief サウンドの音量の設定。
	 * @param SEList SEリスト。
	 * @param soundVolume サウンドの音量。
	 */
	void SetVolume(SEList se, float soundVolume)
	{
		m_seSound[se]->SetVolume(soundVolume);
	}

	/**
	 * @brief サウンドの音量を取得。
	 * @param BGMList BGMリスト。
	 * @return サウンドの音量。
	 */
	float GetVolume(BGMList bgm)
	{
		return m_bgmSound[bgm]->GetVolume();
	}
	/**
	 * @brief サウンドの音量を取得。
	 * @param SEList SEリスト。
	 * @return サウンドの音量。
	 */
	float GetVolume(SEList se)
	{
		return m_seSound[se]->GetVolume();
	}

	/**
	 * @brief 音量の割合の計算。
	 * @param soundType 音量の種類。
	 * @param rate 音量の割合。
	 */
	void SetVolumeRate(GameSoundType soundType, float rate)
	{
		m_volumeRate[soundType] = rate;
	}

	/**
	 * @brief 音量の割合の取得。
	 * @param soundType 音量の種類。
	 * @return 音量の割合。
	 */
	float GetVolumeRate(GameSoundType soundType)
	{
		return m_volumeRate[soundType];
	}

	/**
	 * @brief BGM再生中？
	 * @param BGMList ゲームサウンドリスト。
	 * @return trueなら再生中。
	 */
	bool IsPlayingBGM(BGMList bgm)
	{
		return m_isPlayingBGM[bgm];
	}

	/**
	 * @brief SE再生中？
	 * @param SEList SEリスト。
	 * @return trueなら再生中。
	 */
	bool IsPlayingSE(SEList se)
	{
		return m_isPlayingSE[se];
	}

	/**
	 * @brief サウンドの停止。
	 * @param BGMList BGMリスト。
	 */
	void StopSound(BGMList bgm)
	{
		m_bgmSound[bgm]->Stop();
	}

	/**
	 * @brief サウンドの停止。
	 * @param SEList SEリスト。
	 */
	void StopSound(SEList se)
	{
		m_seSound[se]->Stop();
	}

	/**
	 * @brief サウンドの一時停止。
	 * @param BGMList ゲームサウンドリスト。
	 */
	void PauseSound(BGMList bgm)
	{
		m_bgmSound[bgm]->Pause();
	}

	/**
	 * @brief サウンドの一時停止。
	 * @param SEList ゲームサウンドリスト。
	 */
	void PauseSound(SEList se)
	{
		m_seSound[se]->Pause();
	}

	/**
	 * @brief サウンド用インスタンスを取得。
	 * @param BGMList ゲームサウンドリスト。
	 * @return サウンド用インスタンス。
	 */
	SoundSource* GetSoundInstance(BGMList bgm)
	{
		return m_bgmSound[bgm];
	}

	/**
	 * @brief サウンド用インスタンスを取得。
	 * @param SEList ゲームサウンドリスト。
	 * @return サウンド用インスタンス。
	 */
	SoundSource* GetSoundInstance(SEList se)
	{
		return m_seSound[se];
	}

private:/*メンバ関数。*/

	/*BGMの初期化*/
	inline void InitBGM(BGMList bgm, const char* bgmFilePath) {
		g_soundEngine->ResistWaveFileBank(bgm, bgmFilePath);
		m_isPlayingBGM[bgm] = false;
		m_bgmSound[bgm] = nullptr;
	}

	/*SEの初期化。*/
	inline void InitSE(SEList se, const char* seFliePath) {
		g_soundEngine->ResistWaveFileBank(se, seFliePath);
		m_isPlayingSE[se] = false;
		m_seSound[se] = nullptr;
	}

public:/*シングルトン用のメンバ関数。*/

	/*シングルトンインスタンスの生成。*/
	static void CreateInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new GameSoundEngine();
		}
	}

	/*シングルトンインスタンスの取得。*/
	static GameSoundEngine* GetInstance()
	{
		return m_instance;
	}

	/*シングルトンインスタンスの削除。*/
	static void DeleteInstance()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

private:
	float m_baseBGMVolume[BGMList_Num] = { 0.0f };/*基準となるBGMの音量。*/
	float m_baseSEVolume[SEList_Num] = { 0.0f };/*基準となるSEの音量。*/
	float m_volumeRate[GameSoundType_Num] = { 0.5f,0.5f,0.5f };/*音量の割合。*/
	bool m_isPlayingBGM[BGMList_Num] = { false };/*サウンドが再生されているか？*/
	bool m_isPlayingSE[SEList_Num] = { false };/*サウンドが再生されているか？*/
	SoundSource* m_bgmSound[BGMList_Num];/*BGM再生用インスタンス。*/
	SoundSource* m_seSound[SEList_Num];/*SE再生用インスタンス。*/

private:/*シングルトン用のメンバ変数。*/
	static GameSoundEngine* m_instance;/*シングルトン用インスタンス。*/
};

