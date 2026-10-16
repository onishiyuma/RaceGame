#include "stdafx.h"
#include "UIManager.h"

UIManager* UIManager::m_instance = nullptr; /*シングルトンインスタンスの初期化。*/

/*更新処理。*/
void UIManager::Update()
{
	/*UIの更新処理。*/
	for (auto& uiList: m_uiList)
	{
		for (auto& ui : uiList)
		{
			ui->Update();
		}
	}

	/*UIアニメーションシーケンスの更新処理。*/
	for (auto& anim : m_uiAnimationSequenceParallels)
	{
		anim->Update();
	}

	for (auto& anim : m_uiAnimationSequenceSerials)
	{
		anim->Update();
	}
}

/*描画処理。*/
void UIManager::Render(RenderContext& rc)
{
	for (auto& uiList : m_uiList)
	{
		for (auto& ui : uiList)
		{
			ui->Draw(rc);
		}
	}
}