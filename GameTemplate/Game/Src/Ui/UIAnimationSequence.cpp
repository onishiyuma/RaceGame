#include "stdafx.h"
#include "UIAnimationSequence.h"

/*更新処理。*/
void UIAnimationSequenceParallel::Update()
{
	m_isEnd = true;
	for (auto& anim : m_uiAnimations)
	{
		if (!anim->IsEnd())
		{
			anim->Update();
			m_isEnd = false;
		}
	}
}
void UIAnimationSequenceSerial::Update()
{
	m_isEnd = true;
	if (m_currentPlayNumber != m_uiAnimations.size())
	{
		m_isEnd = false;
		if (m_uiAnimations[m_currentPlayNumber]->IsEnd())
		{
			m_currentPlayNumber++;
			return;
		}
		m_uiAnimations[m_currentPlayNumber]->Update();
	}
}

/*リセット処理。*/
void UIAnimationSequenceParallel::Reset()
{
	m_isEnd = false;
	for (auto& anim : m_uiAnimations)
	{
		anim->Reset();
	}
}
void UIAnimationSequenceSerial::Reset()
{
	m_isEnd = false;
	m_currentPlayNumber = 0;
	for (auto& anim : m_uiAnimations)
	{
		anim->Reset();
	}
}