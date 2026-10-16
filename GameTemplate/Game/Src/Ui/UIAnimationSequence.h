#pragma once
#include "UIAnimation.h"
/**
 * @file UIAnimationSequence.h。
 * @brief UIアニメーションシーケンスクラス。
 * @author Tanimoto。
 * @date 2026/10/07。
 */

/*UIアニメーションシーケンス基底クラス。*/
class UIAnimationSequenceBase
{
public:
	virtual ~UIAnimationSequenceBase() = default;
	virtual void Update() = 0;
	virtual void Reset() = 0;

public:

	/**
	 * @brief 再生するアニメーションの追加。
	 * @param anim 再生するアニメーション。
	 * @return このクラス自身。
	 */
	UIAnimationSequenceBase& Add(std::shared_ptr<UIAnimationBase> anim)
	{
		m_uiAnimations.push_back(anim);;
		return *this;
	}

	/**
	 * @brief 再生終了？
	 * @return trueなら再生終了している。
	 */
	bool IsEnd() const { return m_isEnd; }

protected:
	std::vector<std::shared_ptr<UIAnimationBase>> m_uiAnimations;
	bool m_isEnd = false;
};

/*UIアニメーションシーケンス(並列)*/
class UIAnimationSequenceParallel : public UIAnimationSequenceBase
{
public:
	void Update() override;/*更新処理。*/
	void Reset() override;/*リセット処理。*/
};

/*UIアニメーションシーケンス(直列)*/
class UIAnimationSequenceSerial : public UIAnimationSequenceBase
{
public:
	void Update() override;/*更新処理。*/
	void Reset() override;/*リセット処理。*/

private:
	int m_currentPlayNumber = 0;/*現在アニメーションを再生している番号*/
};

