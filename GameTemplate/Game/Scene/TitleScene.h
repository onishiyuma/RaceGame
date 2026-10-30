#pragma once
#include "Scene/IScene.h"
/**
 * @file TitleScene.h。
 * @brief タイトルシーンクラス。
 * @author Tanimoto。
 * @date 2026/10/13。
 */

class TitleScene : public IScene
{
public:
	TitleScene() {};/*コンストラクタ。*/
	~TitleScene();/*デストラクタ。*/

public:
	bool OnLoad() override;
	bool OnInit() override;
	void OnUpdate(float deltaTime) override;
	void OnRender(RenderContext& rc) override {};
	void OnUnload() override {};

public:/*列挙型。*/

	/*タイトル画面でのシーン。*/
	enum EnScene
	{
		enScene_Title,/*タイトル。*/
		enScene_Select,/*選択。*/
		enScene_Num/*シーン数。*/
	};

	/*選択する内容。*/
	enum EnSelect
	{
		enSelect_Stage1,/*ステージ1。*/
		enSelect_Stage2,/*ステージ2。*/
		enSelect_Num/*選択する数。*/
	};

private:/*メンバ関数。*/
	void InitUI();/*UIの初期化。*/

	void InitTitleBackGround();/*タイトル背景の初期化。*/

	void InitTitleLogo();/*タイトルロゴの初期化。*/

	void InitSelectBackGround();/*選択背景の初期化。*/

	void InitStageIcon();/*ステージアイコンの初期化。*/

	void InitFrame();/*選択枠の初期化。。*/

	void InitAbutton();/*Aボタンの初期化。*/

	void InitBButton();/*Bボタンの初期化。*/

	void Title();/*タイトルシーン。*/

	void Select();/*選択シーン。*/

public:/*メンバ関数。*/
	
	/**
	 * @brief 現在選択している内容の取得。
	 * @return 現在選択している内容の取得。
	 */
	inline EnSelect GetSelect() const
	{
		return m_currentSelect;
	}

private:/*メンバ変数。*/
	SpriteRender m_titleBackGround;/*タイトル背景。*/
	SpriteRender m_titleLogo;/*タイトルロゴ。*/
	SpriteRender m_selectBackGround;/*選択背景。*/
	SpriteRender m_stageIcon[enSelect_Num];/*ステージアイコン。*/
	SpriteRender m_frame;/*選択枠。*/
	SpriteRender m_aButton;/*Aボタン。*/
	SpriteRender m_bButton;/*Bボタン。*/
	EnScene m_currentScene = enScene_Title;/*現在のシーン。*/
	EnSelect m_currentSelect = enSelect_Stage1;/*現在の選択。*/
	DelayTime m_delayTime;/*遅延時間。*/
};

