#include "stdafx.h"
#include "UIAnimation.h"

/*UIアニメーションの更新処理。*/
void UIAnimationBase::Update()
{
	/*逆再生の状態が切り替わってもスムーズに動くように判定をリセットする。*/
	if (m_isReverse != m_isCurrentReverseState)
	{
		m_isCurrentReverseState = m_isReverse;

		m_isCompleted = false;
		m_isEnd = false;
	}

	/*開始前の遅延時間があるか？*/
	/*ある場合は、遅延時間を減らしていく。*/
	if (m_startDelayTime > 0.0f)
	{
		m_startDelayTime -= g_gameTime->GetFrameDeltaTime();
		return;
	}

	/*繰り返し実行するか？*/
	if (!m_isLoop && m_isCompleted)
	{
		if (!IsReverse()) LarpProcess(1.0f);
		else LarpProcess(0.0f);
		m_render->Update();

		/*終了後の遅延時間があるか？*/
		/*ある場合は、遅延時間を減らしていく。*/
		if (m_endDelayTime > 0.0f)
		{
			m_endDelayTime -= g_gameTime->GetFrameDeltaTime();
			return;
		}

		m_isEnd = true;
		return;
	}

	/*経過時間の取得。*/
	const float deltaTime = g_gameTime->GetFrameDeltaTime();

	/*現在のアニメーション経過時間のパーセント。*/
	float computePercent = m_elapsedTime / m_targetTime;
	computePercent = m_easingFunc(computePercent);

	/*位置の設定。*/
	LarpProcess(computePercent);
	m_render->Update();

	/*経過時間の更新。*/
	if (!m_isReverse)
	{
		m_elapsedTime += deltaTime * m_playSpeed;
		if (m_elapsedTime > m_targetTime)
		{
			if (m_isLoop)
			{
				m_currentStep = m_currentStep == enAnimationStep_Max ? enAnimationStep_Min : enAnimationStep_Max;
				m_elapsedTime = 0.0f;
			}
			else
			{
				m_elapsedTime = 1.0f;
			}

			/*完了。*/
			m_isCompleted = true;
		}
	}
	else if(m_isReverse)
	{
		m_elapsedTime -= deltaTime * m_playSpeed;
		if (m_elapsedTime < 0.0f)
		{
			if (m_isLoop)
			{
				m_currentStep = m_currentStep == enAnimationStep_Min ? enAnimationStep_Max : enAnimationStep_Min;
				m_elapsedTime = 1.0f;
			}
			else
			{
				m_elapsedTime = 0.0f;
			}

			/*完了。*/
			m_isCompleted = true;
		}
	}
}

/*線形補間処理。*/
void PositionUIAnimation::LarpProcess(float percent)
{
	Vector3 targetPosition = Vector3::Zero;
	Vector3 basePosition = Vector3::Zero;

	/*ステップの切り替え処理。*/
	switch (m_currentStep)
	{
	case UIAnimationBase::enAnimationStep_Min:/*下限。*/
		targetPosition = m_targetPosition;
		basePosition = m_basePosition;
		break;
	case UIAnimationBase::enAnimationStep_Max:/*上限。*/
		targetPosition = m_basePosition;
		basePosition = m_targetPosition;
		break;
	default:
		break;
	}

	/*初期値から目標値までをなめらかに変化。*/
	Vector3 computePosition = basePosition;
	computePosition.Lerp(percent, basePosition, targetPosition);

	/*位置の設定。*/
	m_render->SetPosition(computePosition);
}

/*線形補間処理。*/
void RotationUIAnimation::LarpProcess(float percent)
{
	Quaternion targetRotation = Quaternion::Identity;
	Quaternion baseRotation = Quaternion::Identity;

	/*ステップの切り替え処理。*/
	switch (m_currentStep)
	{
	case UIAnimationBase::enAnimationStep_Min:/*下限。*/
		targetRotation = m_targetRotation;
		baseRotation = m_baseRotation;
		break;
	case UIAnimationBase::enAnimationStep_Max:/*上限。*/
		targetRotation = m_baseRotation;
		baseRotation = m_targetRotation;
		break;
	}

	/*初期値から目標値までをなめらかに変化。*/
	Quaternion computeRotation = baseRotation;
	computeRotation.Slerp(percent, baseRotation, targetRotation);

	/*回転の設定。*/
	m_render->SetRotation(computeRotation);
}

/*線形補間処理。*/
void ScaleUIAnimation::LarpProcess(float percent)
{
	Vector2 targetScale = Vector2::Zero;
	Vector2 baseScale = Vector2::Zero;

	/*ステップの切り替え処理。*/
	switch (m_currentStep)
	{
	case UIAnimationBase::enAnimationStep_Min:/*下限。*/
		targetScale = m_targetScale;
		baseScale = m_baseScale;
		break;
	case UIAnimationBase::enAnimationStep_Max:/*上限。*/
		targetScale = m_baseScale;
		baseScale = m_targetScale;
		break;
	}

	/*初期値から目標値までをなめらかに変化。*/
	Vector2 computeScale = baseScale;
	computeScale.Lerp(percent, baseScale, targetScale);

	/*大きさの設定。*/
	m_render->SetScale(Vector3(computeScale.x, computeScale.y, 1.0f));
}

/*線形補間処理。*/
void ColorUIAnimation::LarpProcess(float percent)
{
	Vector4 targetColor = Vector4::White;
	Vector4 baseColor = Vector4::White;

	/*ステップの切り替え処理。*/
	switch (m_currentStep)
	{
	case UIAnimationBase::enAnimationStep_Min:/*下限。*/
		targetColor = m_targetColor;
		baseColor = m_baseColor;
		break;
	case UIAnimationBase::enAnimationStep_Max:/*上限。*/
		targetColor = m_baseColor;
		baseColor = m_targetColor;
		break;
	}

	/*初期値から目標値までをなめらかに変化。*/
	Vector4 computeColor = nsK2EngineLow::Math::Lerp<Vector4>(percent, baseColor, targetColor);

	/*色の設定。*/
	m_render->SetMulColor(computeColor);
}

/*線形補間処理。*/
void AlphaUIAnimation::LarpProcess(float percent)
{
	float targetAlpha = 0.0f;
	float baseAlpha = 0.0f;

	/*ステップの切り替え処理。*/
	switch (m_currentStep)
	{
	case UIAnimationBase::enAnimationStep_Min:/*下限。*/
		targetAlpha = m_targetAlpha;
		baseAlpha = m_baseAlpha;
		break;
	case UIAnimationBase::enAnimationStep_Max:/*上限。*/
		targetAlpha = m_baseAlpha;
		baseAlpha = m_targetAlpha;
		break;
	}

	/*初期値から目標値までをなめらかに変化。*/
	float computeAlpha = nsK2EngineLow::Math::Lerp<float>(percent, baseAlpha, targetAlpha);

	/*透明度の設定。*/
	m_render->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, computeAlpha));
}

/*リセット処理。*/
void UIAnimationBase::Reset()
{
	m_elapsedTime = 0.0f;
	m_currentStep = enAnimationStep_Min;
	m_isCompleted = false;
	m_isEnd = false;
	m_isReverse = false;
	m_isCurrentReverseState = false;
}