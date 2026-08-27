#include "k2EnginePreCompile.h"
#include "SpriteRender.h"

namespace nsK2Engine
{
	//初期化
	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
	{
		//スプライトの初期化
		SpriteInitData spriteInitData;
		//ddsファイルのファイルパスの指定
		spriteInitData.m_ddsFilePath[0] = filePath;
		//シェーダーファイルのファイルパスの指定
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		//幅と高さの設定
		spriteInitData.m_width = w;
		spriteInitData.m_height = h;
		//描画モードを指定
		spriteInitData.m_alphaBlendMode = alphaBlendMode;
		//初期化
		m_sprite.Init(spriteInitData);
	}

	//更新処理
	void SpriteRender::Update()
	{
		//スプライトの更新処理
		m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
	}

	//描画処理
	void SpriteRender::Draw(RenderContext& rc)
	{
		//レンダリングオブジェクトの追加
		g_renderingEngine->AddRenderObject(this);
	}
}