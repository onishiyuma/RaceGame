#pragma once
#include "Scene/IScene.h"
/**
 * @file Result.h。
 * @brief リザルトクラス。
 * @author Tanimoto。
 * @date 2026/10/13。
 */

/*リザルトクラス。*/
class Result : public IScene
{
public:
	Result() {};/*コンストラクタ。*/
	~Result();/*デストラクタ。*/

public:
	bool OnLoad() override;
	bool OnInit() override { return true; };
	void OnUpdate() override;
	void OnRender(RenderContext& rc) override {};
	void OnUnload() override {};

private:/*構造体。*/

	/*リザルトの表示に必要なプレイヤーのデータ。*/
	struct PlayerData
	{
		int playerType;/*プレイヤーの種類。*/
		int rank;/*順位。*/
	};

private:/*列挙型。*/

	/*演出一覧。*/
	enum EnDirection
	{
		enDirection_Ranking,/*ランキング。*/
		enDirection_Select,/*選択。*/
		enDirection_Num/*演出数。*/
	};

	/*ランク一覧。*/
	enum EnRank
	{
		enRankIcon_1st,/*1位。*/
		enRankIcon_2nd,/*2位。*/
		enRankIcon_3rd,/*3位。*/
		enRankIcon_4th,/*4位。*/
		enRankIcon_5th,/*5位。*/
		enRankIcon_6th,/*6位。*/
		enRankIcon_7th,/*7位。*/
		enRankIcon_8th,/*8位。*/
		enRankIcon_9th,/*9位。*/
		enRankIcon_Num/*ランク数。*/
	};

public:/*列挙型。/

	/*プレイヤーアイコン一覧。*/
	enum EnPlayerIcon
	{
		enPlayerIcon_1P,/*プレイヤー1。*/
		enPlayerIcon_2P,/*プレイヤー2。*/
		enPlayerIcon_CPU1,/*CPU1。*/
		enPlayerIcon_CPU2,/*CPU2。*/
		enPlayerIcon_CPU3,/*CPU3。*/
		enPlayerIcon_CPU4,/*CPU4。*/
		enPlayerIcon_CPU5,/*CPU5。*/
		enPlayerIcon_CPU6,/*CPU6。*/
		enPlayerIcon_Num/*プレイヤーアイコン数。。*/
	};

	/*リザルト画面でのシーン。*/
	enum EnScene
	{
		enScene_Ranking,/*ランキング。*/
		enScene_Select,/*選択。*/
		enScene_Num/*シーン数。*/
	};

	/*選択する内容。*/
	enum EnSelect
	{
		enSelect_PlayGame,/*もういちどプレイ。*/
		enSelect_Sticker,/*ステッカー選択へ。*/
		enSelect_Stage,/*ステージ選択へ。*/
		enSelect_Title,/*タイトルへ。*/
		enSelect_Num/*選択する数。*/
	};

private:/*メンバ関数。*/
	void InitUI();/*UIの初期化。*/

	void InitFrame(int rank);/*枠の初期化。*/

	void InitPlayerIcon(PlayerData PlayerData);/*プレイヤーアイコンの初期化。*/

	void InitRankIcon(int rank);/*ランクUIの初期化。*/

	void InitReslutBackGround();/*リザルト後の選択用の背景の初期化。*/

	void InitResultOption(int index);/*リザルト後の選択用の選択肢の初期化。*/

	void InitResultFrame();/*リザルト後の選択用の枠の初期化。*/

	void InitAnimation();/*アニメーションの初期化。*/

	void InitSlideAnimation(std::unique_ptr<SpriteRender>& targetSprite, int index);/*スライドアニメーションの初期化。*/

	void InitAlphaAnimation(std::unique_ptr<SpriteRender>& targetSprite, int index);/*透明度を変えるアニメーションの初期化。*/

	void ChangeSlideAnimationData(std::unique_ptr<SpriteRender>& targetSprite, int index);/*スライドアニメーションに登録されている情報の切り替え。*/

	void ChangeAlphaAnimationData(std::unique_ptr<SpriteRender>& targetSprite, int index);/*透明度を変えるアニメーションに登録されている情報の切り替え。*/

	void UpdateSlide();/*スライドアニメーションの更新処理。*/

	void UpdateAlpha();/*透明度を変えるアニメーションの更新処理。*/

	void Operation();/*プレイヤー側の操作。*/

public:/*メンバ関数。*/

	/**
	 * @brief リザルトの表示に必要なプレイヤーのデータの格納。
	 * @param playerType プレイヤーの種類。
	 * @param rank 順位。
	 */
	inline void PushPlayerData(int playerType, int rank)
	{
		m_playerDatas.push_back({ playerType, rank });
	}

	/**
	 * @brief 現在選択している内容の取得。
	 * @return 現在選択している内容の取得。
	 */
	inline EnSelect GetSelect() const
	{
		return m_currentSelect;
	}

private:/*メンバ変数。*/
	std::vector<PlayerData> m_playerDatas;/*リザルトの表示に必要なプレイヤーのデータ。*/
	std::vector<std::unique_ptr<SpriteRender>> m_frames;/*枠。*/
	std::vector<std::unique_ptr<SpriteRender>> m_playerIcons;/*プレイヤーアイコン。*/
	std::vector<std::unique_ptr<SpriteRender>> m_rankIcons;/*ランクアイコン。*/
	std::vector<std::unique_ptr<PositionUIAnimation>> m_slideAnimations;/*スライドさせるアニメーション。*/
	std::vector<std::unique_ptr<AlphaUIAnimation>> m_alphaAnimations;/*透明度を変えるアニメーション。*/
	SpriteRender m_resultBackGround;/*リザルト後の選択用の背景。*/
	SpriteRender m_resultOption[enSelect_Num];/*選択肢。*/
	SpriteRender m_resultFrame;/*リザルト後の選択用の枠。*/
	bool m_isEndDirection = false;/*演出が終了したか？*/
	bool m_isTriggerButtonA = false;/*Aボタンが押されたか？*/
	EnDirection m_currentDirection = enDirection_Ranking;/*現在の演出。*/
	EnScene m_currentScene = enScene_Ranking;/*現在のシーン。*/
	EnSelect m_currentSelect = enSelect_PlayGame;/*現在の内容。*/
	DelayTime m_delayTime;/*遅延時間。*/

private:/*スプライトのファイルパス。*/
	std::string m_frameFilePath = "Assets/UI/result/ranking/rankNormalFrame.dds";/*枠。*/
	std::string m_playerIconFilePath[enPlayerIcon_Num] = { 
		"Assets/UI/result/ranking/result1P.dds",
		"Assets/UI/result/ranking/result2P.dds",
		"Assets/UI/result/ranking/resultCPU1.dds",
		"Assets/UI/result/ranking/resultCPU2.dds",
		"Assets/UI/result/ranking/resultCPU3.dds",
		"Assets/UI/result/ranking/resultCPU4.dds",
		"Assets/UI/result/ranking/resultCPU5.dds",
		"Assets/UI/result/ranking/resultCPU6.dds",
	};/*プレイヤーアイコンのファイルパス。*/
	std::string m_rankIconFilePath[enRankIcon_Num] = { 
		"Assets/UI/result/ranking/rankOne.dds",
		"Assets/UI/result/ranking/rankTwo.dds",
		"Assets/UI/result/ranking/rankThree.dds",
		"Assets/UI/result/ranking/rankFour.dds",
		"Assets/UI/result/ranking/rankFive.dds",
		"Assets/UI/result/ranking/rankSix.dds",
		"Assets/UI/result/ranking/rankSevem.dds",
		"Assets/UI/result/ranking/rankEight.dds",
		"Assets/UI/result/ranking/rankNine.dds"
	};/*ランクアイコンのファイルパス。*/
	std::string m_resultOptionFilePath[enSelect_Num] = {
		"Assets/UI/result/select/resultPlayStart.dds",
		"Assets/UI/result/select/resultSticker.dds",
		"Assets/UI/result/select/resultStage.dds",
		"Assets/UI/result/select/resultTitle.dds"
	};
};

