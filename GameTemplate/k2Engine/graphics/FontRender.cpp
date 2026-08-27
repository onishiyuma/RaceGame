#include "k2EnginePreCompile.h"
#include "FontRender.h"

namespace nsK2Engine {
	//描画処理
	void FontRender::Draw(RenderContext& rc)
	{
		//テキストの内容が入力されていなかったら処理しない
		if (m_text == nullptr)
		{
			return;
		}
		//レンダリングオブジェクトの追加
		g_renderingEngine->AddRenderObject(this);
	}
}