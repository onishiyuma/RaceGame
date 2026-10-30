#include "stdafx.h"
#include "LoadingUI.h"

namespace
{
	const float LOADING_TIRE_ROTATION_SPEED = 5.0f;/*ローディング用のタイヤの回転速度。*/

	const float VIBRATION_ANIMATION_PLAY_SPEED = 3.0f;/*振動アニメーションの再生速度。*/

	const float VIBRATION_ANIMATION_END_TIME = 0.1f;/*振動アニメーションの終了遅延時間。*/
}

LoadingUI* LoadingUI::m_instance = nullptr;/*シングルトンインスタンスの初期化。*/

/*開始処理。*/
bool LoadingUI::Start()
{
	/*背景の初期化。*/
	m_backGround.Init("Assets/UI/Load/loadingBack.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	m_backGround.SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	m_backGround.Update();

	/*ローディング用のタイヤの初期化。*/
	m_loadingTire.Init("Assets/UI/Load/LoadTire.dds", 1920, 1080);
	m_loadingTire.SetPosition({ 625.0f,-325.0f,0.0f });
	m_loadingTire.SetScale({ 0.8f,0.8f,1.0f });
	m_loadingTire.SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	m_loadingTire.Update();

	/*振動アニメーションの初期化。*/
	m_vibrationAnimation = std::make_unique<PositionUIAnimation>();
	m_vibrationAnimation->SetRender(&m_loadingTire).SetPlaySpeed(VIBRATION_ANIMATION_PLAY_SPEED).SetLoop(true).SetEndDelayTime(VIBRATION_ANIMATION_END_TIME).SetRelativePositionY(-5.0f);

	return true;
}

/*更新処理。*/
void LoadingUI::Update()
{
	/*スプライトが透明のときは処理しない。*/
	if (m_loadingTire.GetMulColor().a <= 0.0f) { return; }

	m_tireRotation.AddRotationDegZ(LOADING_TIRE_ROTATION_SPEED);
	m_loadingTire.SetRotation(m_tireRotation);
	m_loadingTire.Update();

	m_vibrationAnimation->Update();
}

/*描画処理。*/
void LoadingUI::Render(RenderContext& rc)
{
	/*スプライトが透明のときは描画しない。*/
	if (m_loadingTire.GetMulColor().a <= 0.0f) { return; }

	m_backGround.Draw(rc);
	m_loadingTire.Draw(rc);
}