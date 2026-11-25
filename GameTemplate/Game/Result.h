#pragma once
#include <optional>
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
	void OnUpdate(float deltaTime) override;
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

	void InitFrame(PlayerData playerData);/*枠の初期化。*/

	void InitPlayerIcon(PlayerData playerData);/*プレイヤーアイコンの初期化。*/

	void InitRankIcon(PlayerData playerData);/*ランクアイコンの初期化。*/

	void InitSelectBackGround();/*リザルト後の選択用の背景の初期化。*/

	void InitSelectOption(int index);/*リザルト後の選択用の選択肢の初期化。*/

	void InitSelectFrame();/*リザルト後の選択用の枠の初期化。*/

	void InitAnimation();/*アニメーションの初期化。*/

	void InitSlideAnimation(std::unique_ptr<SpriteRender>& targetSprite, int index);/*スライドアニメーションの初期化。*/

	void InitAlphaAnimation(std::unique_ptr<SpriteRender>& targetSprite, int index);/*透明度を変えるアニメーションの初期化。*/

	void InitScaleAnimation();/*拡大縮小させるアニメーションの初期化。*/

	void InitDecisionAnimation();/*決定アニメーションの初期化。*/

	void ChangeSlideAnimationData(std::unique_ptr<SpriteRender>& targetSprite, int index);/*スライドアニメーションに登録されている情報の切り替え。*/

	void ChangeAlphaAnimationData(std::unique_ptr<SpriteRender>& targetSprite, int index);/*透明度を変えるアニメーションに登録されている情報の切り替え。*/

	void DisplayPlayerOrNormalUI();/*プレイヤー側のUIと通常のUIの表示切り替え。*/

	void UpdateSlide();/*スライドアニメーションの更新処理。*/

	void UpdateAlpha();/*透明度を変えるアニメーションの更新処理。*/

	void TransitionScene();/*シーンの切り替え。*/

	void UpdateDirection();/*演出の更新処理。*/

	void UpdateRankingDirection();/*ランキング用の演出の更新処理。*/

	void UpdateSelectDirection();/*選択用の演出の更新処理。*/

	void UpdateScaleAnimation();/*拡大縮小させるアニメーションの更新処理。*/

	void UpdateDecisionAnimation();/*決定アニメーションの更新処理。*/

	void Operation();/*プレイヤー側の操作。*/

	void TriggerButtonA();/*Aボタンを押す。*/

	void Select();/*選択。*/

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
	std::vector<std::unique_ptr<SpriteRender>> m_normalFrames;/*通常枠。*/
	std::vector<std::unique_ptr<SpriteRender>> m_playerFrames;/*プレイヤー側の枠。*/
	std::vector<std::unique_ptr<SpriteRender>> m_normalIcons;/*通常アイコン。*/
	std::vector<std::unique_ptr<SpriteRender>> m_playerIcons;/*プレイヤーアイコン。*/
	std::vector<std::unique_ptr<SpriteRender>> m_normalRankIcons;/*通常ランクアイコン。*/
	std::vector<std::unique_ptr<SpriteRender>> m_playerRankIcons;/*プレイヤーランクアイコン。*/
	std::vector<std::unique_ptr<PositionUIAnimation>> m_slideAnimations;/*スライドさせるアニメーション。*/
	std::vector<std::unique_ptr<AlphaUIAnimation>> m_alphaAnimations;/*透明度を変えるアニメーション。*/
	std::unique_ptr<ScaleUIAnimation> m_scaleAnimation;/*拡大縮小させるアニメーション。*/
	std::unique_ptr<ScaleUIAnimation> m_decisionAnimation;/*決定アニメーション。*/
	SpriteRender m_selectBackGround;/*リザルト後の選択用の背景。*/
	SpriteRender m_selectOption[enSelect_Num];/*選択肢。*/
	SpriteRender m_selectFrame;/*リザルト後の選択用の枠。*/
	bool m_isEndDirection = false;/*演出が終了したか？*/
	bool m_isTriggerButtonA = false;/*Aボタンが押されたか？*/
	EnDirection m_currentDirection = enDirection_Ranking;/*現在の演出。*/
	EnScene m_currentScene = enScene_Ranking;/*現在のシーン。*/
	EnSelect m_currentSelect = enSelect_PlayGame;/*現在の内容。*/
	DelayTime m_delayTime;/*遅延時間。*/

private:/*スプライトのファイルパス。*/
	std::string m_frameFilePath = "Assets/UI/result/ranking/rankNormalFrame.dds";/*枠のファイルパス。*/
	std::string m_playerFrameFilePath = "Assets/UI/result/ranking/rankYellowFrame.dds";/*プレイヤー側の枠のファイルパス。*/

	std::string m_normalIconFilePath[enPlayerIcon_Num] = {
		"Assets/UI/result/name/result1P.dds",
		"Assets/UI/result/name/result2P.dds",
		"Assets/UI/result/name/resultCPU1.dds",
		"Assets/UI/result/name/resultCPU2.dds",
		"Assets/UI/result/name/resultCPU3.dds",
		"Assets/UI/result/name/resultCPU4.dds",
		"Assets/UI/result/name/resultCPU5.dds",
		"Assets/UI/result/name/resultCPU6.dds"
	};/*通常アイコンのファイルパス。*/
	
	std::string m_playerIconFilePath[enPlayerIcon_Num] = { 
		"Assets/UI/result/name/result1P_Black.dds",
		"Assets/UI/result/name/result2P_Black.dds"
	};/*プレイヤーアイコンのファイルパス。*/

	std::string m_normalRankIconFilePath[enRankIcon_Num] = { 
		"Assets/UI/result/number/rankOne.dds",
		"Assets/UI/result/number/rankTwo.dds",
		"Assets/UI/result/number/rankThree.dds",
		"Assets/UI/result/number/rankFour.dds",
		"Assets/UI/result/number/rankFive.dds",
		"Assets/UI/result/number/rankSix.dds",
		"Assets/UI/result/number/rankSeven.dds",
		"Assets/UI/result/number/rankEight.dds",
		"Assets/UI/result/number/rankNine.dds"
	};/*ランクアイコンのファイルパス。*/

	std::string m_playerRankIconFilePath[enRankIcon_Num] = { 
		"Assets/UI/result/number/rankOneBlack.dds",
		"Assets/UI/result/number/rankTwoBlack.dds",
		"Assets/UI/result/number/rankThreeBlack.dds",
		"Assets/UI/result/number/rankFourBlack.dds",
		"Assets/UI/result/number/rankFiveBlack.dds",
		"Assets/UI/result/number/rankSixBlack.dds",
		"Assets/UI/result/number/rankSevenBlack.dds",
		"Assets/UI/result/number/rankEightBlack.dds",
		"Assets/UI/result/number/rankNineBlack.dds"
	};/*プレイヤーランクアイコンのファイルパス。*/

	std::string m_resultOptionFilePath[enSelect_Num] = {
		"Assets/UI/result/select/resultPlayStart.dds",
		"Assets/UI/result/select/resultSticker.dds",
		"Assets/UI/result/select/resultStage.dds",
		"Assets/UI/result/select/resultTitle.dds"
	};/*選択肢のファイルパス。*/
};

