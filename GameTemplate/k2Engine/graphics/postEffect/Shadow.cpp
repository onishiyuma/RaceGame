#include "k2EnginePreCompile.h"
#include "Shadow.h"

namespace nsK2Engine
{
	//初期化
	void Shadow::Init()
	{
		//シャドウマップ用のレンダリングターゲットの作成
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowMap.Create(
			2048,
			2048,
			1,
			1,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		//シャドウマップをぼかすためのオブジェクトの初期化。
		m_shadowBlur.Init(&m_shadowMap.GetRenderTargetTexture());
	}

	//描画処理を実行
	void Shadow::Execute(RenderContext& rc, std::vector<IRenderer*>& ro)
	{
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_shadowMap);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_shadowMap);

		for (auto& renderObj : ro)
		{
			//シャドウマップの描画
			renderObj->OnRenderShadowMap(rc, g_renderingEngine->GetLightCamera());
		}

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

		m_shadowBlur.ExecuteOnGPU(rc, 5.0f);
	}
}