#include "k2EnginePreCompile.h"
#include "Tone.h"

namespace nsK2Engine 
{
	//初期化
	void Tone::Init(RenderTarget& mainRt ,RenderTarget& depthRt, RenderTarget& normalRt)
	{	
		float clearColor[] = { 0.0f,0.0f,0.0f,1.0f };
		m_toneRenderTarget.Create(
			mainRt.GetWidth(),
			mainRt.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		SpriteInitData initData;
		initData.m_width = mainRt.GetWidth();
		initData.m_height = mainRt.GetHeight();
		initData.m_textures[0] = &mainRt.GetRenderTargetTexture();
		initData.m_textures[1] = &depthRt.GetRenderTargetTexture();
		initData.m_textures[2] = &normalRt.GetRenderTargetTexture();
		initData.m_colorBufferFormat[0] = mainRt.GetColorBufferFormat();
		initData.m_colorBufferFormat[1] = depthRt.GetColorBufferFormat();
		initData.m_colorBufferFormat[2] = normalRt.GetColorBufferFormat();
		initData.m_fxFilePath = "Assets/shader/tonemap.fx";

		m_toneMap.Init(initData);

		SpriteInitData finalSpriteInitData;
		finalSpriteInitData.m_textures[0] = &m_toneRenderTarget.GetRenderTargetTexture();
		finalSpriteInitData.m_width = mainRt.GetWidth();
		finalSpriteInitData.m_height = mainRt.GetHeight();
		finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		finalSpriteInitData.m_vsEntryPointFunc = "VSMain";
		finalSpriteInitData.m_psEntryPoinFunc = "PSMain";
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_finalSprite.Init(finalSpriteInitData);
	}

	//描画処理を実行
	void Tone::Execute(RenderContext& rc, RenderTarget& rt)
	{
		//ターゲットをメインに戻す
		rc.WaitUntilToPossibleSetRenderTarget(m_toneRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_toneRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_toneRenderTarget);

		//トゥーン用スプライトの描画
		m_toneMap.Draw(rc);

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_toneRenderTarget);
		//ターゲットをメインに戻す
		rc.WaitUntilToPossibleSetRenderTarget(rt);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(rt);

		//最終合成用スプライトの描画
		m_finalSprite.Draw(rc);

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(rt);
	}
}