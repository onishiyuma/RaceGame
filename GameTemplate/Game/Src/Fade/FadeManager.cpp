#include "stdafx.h"
#include "FadeManager.h"

namespace
{
	const float ALPHA_MAX = 1.0f;/*透明度の最大値。*/
	
	const float ALPHA_MIN = 0.0f;/*透明度の最小値。*/
}

FadeManager* FadeManager::m_instance = nullptr;/*シングルトンインスタンスの初期化。*/

/*更新処理。*/
void FadeManager::Update()
{
	/*フェードさせるスプライトがなければ処理しない。*/
	if (m_targetFades.empty()) { return; }

	/*フェードの状態が切り替わったらフェードさせるスプライトの乗算色を変更する。*/
	if ((m_currentFade != m_previousFade && m_currentFade != enFade_None))
	{
		ChangeTargetFadesAlpha();
		return;
	}

	/*フェードの更新処理。*/
	UpdateFade();
	
	/*フェードの終了するかの判断処理。*/
	DecisionEndFade();
}

/*フェードさせるスプライトの透明度を切り替える処理。*/
void FadeManager::ChangeTargetFadesAlpha()
{
	for (auto& fade : m_targetFades)
	{
		switch (m_currentFade)
		{
		case FadeManager::enFade_In:
			fade->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
			break;
		case FadeManager::enFade_Out:
			fade->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
			break;
		case FadeManager::enFade_None:
		default:
			break;
		}
		fade->Update();
	}
	m_previousFade = m_currentFade;
}

/*フェードの更新処理。*/
void FadeManager::UpdateFade()
{
	if (m_delay > 0.0f)
	{
		m_delay -= g_gameTime->GetFrameDeltaTime();
		return;
	}

	for (auto& fade : m_targetFades)
	{
		m_fadeAlpha = fade->GetMulColor().a;

		switch (m_currentFade)
		{
		case FadeManager::enFade_In:
			m_fadeAlpha -= g_gameTime->GetFrameDeltaTime();
			break;
		case FadeManager::enFade_Out:
			m_fadeAlpha += g_gameTime->GetFrameDeltaTime();
			break;
		case FadeManager::enFade_None:
		default:
			break;
		}

		fade->SetMulColor({ 1.0f,1.0f,1.0f,m_fadeAlpha });
		fade->Update();
	}
}

/*フェードの終了するかの判断処理。*/
void FadeManager::DecisionEndFade()
{
	switch (m_currentFade)
	{
	case FadeManager::enFade_In:
		if (m_fadeAlpha < ALPHA_MIN)
		{
			m_fadeAlpha = ALPHA_MIN;
			m_currentFade = enFade_None;
		}
		break;
	case FadeManager::enFade_Out:
		if (m_fadeAlpha > ALPHA_MAX)
		{
			m_fadeAlpha = ALPHA_MAX;
			m_currentFade = enFade_None;
		}
		break;
	case FadeManager::enFade_None:
	default:
		break;
	}
}