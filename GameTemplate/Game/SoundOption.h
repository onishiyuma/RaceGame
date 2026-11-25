#pragma once
/**
 * @file SoundOption.h。
 * @brief サウンドオプションクラス。
 * @author Tanimoto。
 * @date 2026/11/06。
 */

/*サウンドを設定するクラス。*/
class SoundOption : public IGameObject
{
public:
	SoundOption() {};/*コンストラクタ。*/
	~SoundOption();/*デストラクタ。*/

public:
	bool Start() override;/*開始処理。*/
	void Update() override; /*更新処理。*/

public:/*列挙型。*/

	/*サウンドの種類。*/
	enum EnSoundType
	{
		enSoundType_Master,/*マスター。*/
		enSoundType_BGM,/*BGM。*/
		enSoundType_SE,/*SE。*/
		enSoundType_Num/*サウンドの種類の数。*/
	};

	/*サウンドアイコンの種類。*/
	enum EnSoundIconType
	{
		enSoundIconType_Small,/*小さい。*/
		enSoundIconType_Big,/*大きい。*/
		enSoundIconType_Num/*サウンドアイコンの種類。*/
	};

	/*〇の位。*/
	enum EnPlace
	{
		enPlace_Hundred,/*百の位。*/
		enPlace_Ten,/*十の位。*/
		enPlace_One,/*一の位。*/
		enPlace_Num/*位の数*/
	};

	/*数字。*/
	enum EnNumber
	{
		enNumber_Zero,/*0。*/
		enNumber_One,/*1。*/
		enNumber_Two,/*2。*/
		enNumber_Three,/*3。*/
		enNumber_Four,/*4。*/
		enNumber_Five,/*5。*/
		enNumber_Six,/*6。*/
		enNumber_Seven,/*7。*/
		enNumber_Eight,/*8。*/
		enNumber_Nine,/*9。*/
		enNumber_Num/*数字の数。*/
	};

private:/*メンバ変数。*/

	void InitUI();/*サウンドの初期化。*/

	void InitSoundBackGround();/*背景の初期化。*/

	void InitBar(EnSoundType soundType, int index);/*バーの初期化。*/

	void InitGauge(EnSoundType soundType, int index);/*バーの初期化。*/

	void InitSoundOptionText(EnSoundType soundType, int index);/*各サウンドを設定する項目テキストの初期化。*/

	void InitKnob(EnSoundType soundType, int index);/*つまみの初期化。*/

	void InitSoundIcon(EnSoundType soundType, EnSoundIconType soundIconType ,int soundIndex ,int soundIconIndex);/*サウンドアイコンの初期化。*/

	void InitSoundVolumeText(EnSoundType soundType, EnPlace place, EnNumber number, int soundIndex, int placeIndex);/*各音量のテキストの初期化。*/

	void InitSoundSettingText();/*サウンド設定テキストの初期化。*/

	void InitReturnBButtonText();/*Bボタンで戻るテキストの初期化。*/

	void InitKnobLight();/*光るつまみの初期化。*/

	void InitAnimation();/*アニメーションの初期化。*/

	void InitAlphaAnimation();/*透明度を変えるアニメーションの初期化。*/

	void InitSoundVolume();/*サウンド音量の初期化。*/

	inline float CalcVolumeRate(EnSoundType soundType) 
	{
		return (float)m_soundVolumes[soundType] / (float)m_soundMaxVolume;
	}/*音量の割合の計算。*/

	void CalcVolumeNumverDisplayData(EnSoundType soundType, float volumeRate);/*音量を表示するためのデータの計算。*/

	void Operation();/*プレイヤー側の操作。*/

	void ResetSoundVolume();/*サウンド音量のリセット。*/

	void Slide();/*スライド。*/

	void Select();/*選択。*/

	void PreviewSeSoundVolume();/*SEの音量プレビュー。*/

	void UpdateGaugeAndKnob(EnSoundType soundType);/*ゲージとつまみ関連の更新処理。*/

	void UpdateDisplaySoundVolumeText();/*各音量のテキストを表示するための更新処理。*/

	void UpdateAnimation();/*アニメーションの更新処理。*/

	void EnableDrawUI();/*UI描画の有効化。*/

	void DisableDrawUI();/*UI描画の無効化。*/

public:/*メンバ関数。*/

	/**
	 * @brief 更新処理の有効化。
	 */
	void EnableUpdate()
	{
		EnableDrawUI();
		Activate();
	}

	/**
	 * @brief 更新処理の無効化。
	 */
	void DisableUpdate()
	{
		DisableDrawUI();
		Deactivate();
	}

	/**
	 * @brief 更新処理しているか？
	 * @return trueなら更新処理している。
	 */
	bool IsUpdate()
	{
		return IsActive();
	}

private:/*メンバ変数。*/
	SpriteRender m_soundBackGround;/*サウンド背景。*/
	SpriteRender m_bar[enSoundType_Num];/*バー。*/
	SpriteRender m_gauge[enSoundType_Num];/*ゲージ。*/
	SpriteRender m_soundOptionText[enSoundType_Num];/*各サウンドを設定する項目テキスト。*/
	SpriteRender m_knob[enSoundType_Num];/*つまみ。*/
	SpriteRender m_soundIcon[enSoundType_Num][enSoundIconType_Num];/*サウンドアイコン。*/
	SpriteRender m_soundVolumeText[enSoundType_Num][enPlace_Num][enNumber_Num];/*各音量のテキスト。*/
	SpriteRender m_soundSettingText;/*サウンド設定テキスト。*/
	SpriteRender m_returnBButtonText;/*Bボタンで戻るテキスト。*/
	SpriteRender m_knobLight;/*光るつまみ。*/
	std::unique_ptr<AlphaUIAnimation> m_alphaAnimation;/*透明度を変えるアニメーション。*/
	std::vector<EnNumber> m_volumeNumverDisplayManager[enSoundType_Num];/*音量の数字UIを管理する変数。*/
	Vector3 m_gaugeBaseScale[enSoundType_Num];/*ゲージの基準となる大きさ。*/
	int m_soundVolumes[enSoundType_Num] = { 0 };/*各サウンドの音量。*/
	int m_soundMaxVolume = 100;/*最大音量。*/
	bool m_isSlide = false;/*スライドしているか？*/
	EnSoundType m_currentSelectSoundType = enSoundType_Master;/*現在選択しているサウンドの種類。*/

private:/*スプライトのファイルパス。*/

	std::string m_soundOptionTextFilePath[enSoundType_Num] = {
		"Assets/UI/sound/word/soundMaster.dds",
		"Assets/UI/sound/word/soundBGM.dds",
		"Assets/UI/sound/word/soundSE.dds"
	};/*各サウンドを設定する項目テキストのファイルパス。*/

	std::string m_soundIconFilePath[enSoundIconType_Num] = {
		"Assets/UI/sound/soundIcon_Small.dds",
		"Assets/UI/sound/soundIcon_Big.dds"
	};/*サウンドアイコンのファイルパス。*/

	std::string m_soundVolumeTextFilePath[enNumber_Num] = {
		"Assets/UI/sound/number/sound_Zero.dds",
		"Assets/UI/sound/number/sound_One.dds",
		"Assets/UI/sound/number/sound_Two.dds",
		"Assets/UI/sound/number/sound_Three.dds",
		"Assets/UI/sound/number/sound_Four.dds",
		"Assets/UI/sound/number/sound_Five.dds",
		"Assets/UI/sound/number/sound_Six.dds",
		"Assets/UI/sound/number/sound_Seven.dds",
		"Assets/UI/sound/number/sound_Eight.dds",
		"Assets/UI/sound/number/sound_Nine.dds",
	};/*各音量のテキストのファイルパス。*/
};

