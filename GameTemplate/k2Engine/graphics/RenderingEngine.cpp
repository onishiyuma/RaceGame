#include "k2EnginePreCompile.h"
#include "RenderingEngine.h"

namespace {
	const float FRAME_BUFFER_W_HALF = FRAME_BUFFER_W / 2;
	const float FRAME_BUFFER_H_HALF = FRAME_BUFFER_H / 2;
}

namespace nsK2Engine
{
	//初期化
	void RenderingEngine::Init()
	{
		//メインレンダリングターゲットの初期化
		InitMainRenderTarget();

		//深度値用レンダリングターゲットの初期化
		InitDepthRenderTarget();

		//法線用レンダリングターゲットの初期化
		InitNormalRenderTarget();

		//2D(フォントとスプライト)の初期化
		Init2DSprite();

		//メインレンダリングターゲットのカラーバッファの内容を
		//フレームバッファにコピーするためのスプライトを初期化する
		InitCopyMainRenderTargetToFrameBuffer();

		//ブルームの初期化
		m_bloom.Init(m_mainRenderTarget);

		//シャドウの初期化
		m_shadow.Init();

		//トゥーンの初期化
		m_tone.Init(m_mainRenderTarget, m_depthRenderTarget, m_normalRenderTarget);

		//シーンライトの初期化
		m_sceneLight.Init();
	}

	//メインレンダリングターゲットの初期化
	void RenderingEngine::InitMainRenderTarget()
	{
		//メインレンダリングターゲットの作成
		float clearColor[4] = { 0.5f,0.5f,0.5f,1.0f };
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	//深度値用レンダリングターゲットの初期化
	void RenderingEngine::InitDepthRenderTarget()
	{
		m_depthRenderTarget.Create(
			m_mainRenderTarget.GetWidth(),
			m_mainRenderTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	//法線用レンダリングターゲットの初期化
	void RenderingEngine::InitNormalRenderTarget()
	{
		m_normalRenderTarget.Create(
			m_mainRenderTarget.GetWidth(),
			m_mainRenderTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	//2D(フォントとスプライト)の初期化
	void RenderingEngine::Init2DSprite()
	{
		//2D用レンダリングターゲットの作成
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
		m_2DRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		//最終合成用のスプライトを初期化する
		SpriteInitData spriteInitData;
		//テクスチャは2Dレンダリングターゲット
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		//解像度はメインレンダリングターゲットの幅と高さ
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		//2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		//上書き
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();
		//初期化
		m_2DSprite.Init(spriteInitData);

		//テクスチャはメインレンダリングターゲット
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//解像度は2Dレンダリングターゲットの幅と高さ
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		//上書き
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();
		//初期化
		m_mainSprite.Init(spriteInitData);
	}

	//ビューポートの初期化。
	void RenderingEngine::InitViewports()
	{
		m_viewPorts.clear();
		switch (m_screenMode)
		{
		case nsK2Engine::enScreenMode_Two:
			SetLeftViewPort();
			SetRightViewPort();
			break;
		case nsK2Engine::enScreenMode_Three:
			SetLeftUpViewPort();
			SetRightUpViewPort();
			SetDownViewPort();
			break;
		case nsK2Engine::enScreenMode_Four:
			SetLeftUpViewPort();
			SetRightUpViewPort();
			SetLeftDownViewPort();
			SetRightDownViewPort();
			break;
		default:
			break;
		}
	}

	//上画面用のビューポートの設定
	void RenderingEngine::SetUpViewPort()
	{
		D3D12_VIEWPORT viewPort;
		viewPort.Width = FRAME_BUFFER_W;//画面の横サイズ
		viewPort.Height = FRAME_BUFFER_H_HALF;//画面の縦サイズ
		viewPort.TopLeftX = 0;//画面左上のx座標
		viewPort.TopLeftY = 0;//画面左上のy座標
		viewPort.MinDepth = 0.0f;//深度値の最小値
		viewPort.MaxDepth = 1.0f;//深度値の最大値

		m_viewPorts.push_back(viewPort);
	}

	//下画面用のビューポートの設定
	void RenderingEngine::SetDownViewPort()
	{
		D3D12_VIEWPORT viewPort;
		viewPort.Width = FRAME_BUFFER_W;//画面の横サイズ
		viewPort.Height = FRAME_BUFFER_H_HALF;//画面の縦サイズ
		viewPort.TopLeftX = 0;//画面左上のx座標
		viewPort.TopLeftY = FRAME_BUFFER_H_HALF;//画面左上のy座標
		viewPort.MinDepth = 0.0f;//深度値の最小値
		viewPort.MaxDepth = 1.0f;//深度値の最大値

		m_viewPorts.push_back(viewPort);
	}

	//左画面用のビューポートの設定
	void RenderingEngine::SetLeftViewPort()
	{
		D3D12_VIEWPORT viewPort;
		viewPort.Width = FRAME_BUFFER_W_HALF;//画面の横サイズ
		viewPort.Height = FRAME_BUFFER_H;//画面の縦サイズ
		viewPort.TopLeftX = 0;//画面左上のx座標
		viewPort.TopLeftY = 0;//画面左上のy座標
		viewPort.MinDepth = 0.0f;//深度値の最小値
		viewPort.MaxDepth = 1.0f;//深度値の最大値

		m_viewPorts.push_back(viewPort);
	}

	//右画面用のビューポートの設定
	void RenderingEngine::SetRightViewPort()
	{
		D3D12_VIEWPORT viewPort;
		viewPort.Width = FRAME_BUFFER_W_HALF;//画面の横サイズ
		viewPort.Height = FRAME_BUFFER_H;//画面の縦サイズ
		viewPort.TopLeftX = FRAME_BUFFER_W_HALF;//画面左上のx座標
		viewPort.TopLeftY = 0;//画面左上のy座標
		viewPort.MinDepth = 0.0f;//深度値の最小値
		viewPort.MaxDepth = 1.0f;//深度値の最大値
		m_viewPorts.push_back(viewPort);
	}

	//左上画面用のビューポートの設定
	void RenderingEngine::SetLeftUpViewPort()
	{
		D3D12_VIEWPORT viewPort;
		viewPort.Width = FRAME_BUFFER_W_HALF;//画面の横サイズ
		viewPort.Height = FRAME_BUFFER_H_HALF;//画面の縦サイズ
		viewPort.TopLeftX = 0;//画面左上のx座標
		viewPort.TopLeftY = 0;//画面左上のy座標
		viewPort.MinDepth = 0.0f;//深度値の最小値
		viewPort.MaxDepth = 1.0f;//深度値の最大値

		m_viewPorts.push_back(viewPort);
	}

	//右上画面用のビューポートの設定
	void RenderingEngine::SetRightUpViewPort()
	{
		D3D12_VIEWPORT viewPort;
		viewPort.Width = FRAME_BUFFER_W_HALF;//画面の横サイズ
		viewPort.Height = FRAME_BUFFER_H_HALF;//画面の縦サイズ
		viewPort.TopLeftX = FRAME_BUFFER_W_HALF;//画面左上のx座標
		viewPort.TopLeftY = 0;//画面左上のy座標
		viewPort.MinDepth = 0.0f;//深度値の最小値
		viewPort.MaxDepth = 1.0f;//深度値の最大値

		m_viewPorts.push_back(viewPort);
	}

	//左下画面用のビューポートの設定
	void RenderingEngine::SetLeftDownViewPort()
	{
		D3D12_VIEWPORT viewPort;
		viewPort.Width = FRAME_BUFFER_W_HALF;//画面の横サイズ
		viewPort.Height = FRAME_BUFFER_H_HALF;//画面の縦サイズ
		viewPort.TopLeftX = 0;//画面左上のx座標
		viewPort.TopLeftY = FRAME_BUFFER_H_HALF;//画面左上のy座標
		viewPort.MinDepth = 0.0f;//深度値の最小値
		viewPort.MaxDepth = 1.0f;//深度値の最大値

		m_viewPorts.push_back(viewPort);
	}

	//右下画面用のビューポートの設定
	void RenderingEngine::SetRightDownViewPort()
	{
		D3D12_VIEWPORT viewPort;
		viewPort.Width = FRAME_BUFFER_W_HALF;   //画面の横サイズ
		viewPort.Height = FRAME_BUFFER_H_HALF;   //画面の縦サイズ
		viewPort.TopLeftX = FRAME_BUFFER_W_HALF;   //画面左上のx座標
		viewPort.TopLeftY = FRAME_BUFFER_H_HALF;   //画面左上のy座標
		viewPort.MinDepth = 0.0f;   //深度値の最小値
		viewPort.MaxDepth = 1.0f;   //深度値の最大値

		m_viewPorts.push_back(viewPort);
	}

	//メインレンダリングターゲットのカラーバッファの内容を
	//フレームバッファにコピーするためのスプライトを初期化する
	void RenderingEngine::InitCopyMainRenderTargetToFrameBuffer()
	{
		//スプライトの初期化
		SpriteInitData spriteInitData;
		//テクスチャはメインレンダリングターゲット
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		//解像度はメインレンダリングターゲットの幅と高さ
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		//2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		//初期化
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	//描画処理を実行
	void RenderingEngine::Execute(RenderContext& rc)
	{
		//ライトカメラの更新処理
		m_sceneLight.LightCameraUpdate();

		//シャドウの描画処理を実行
		m_shadow.Execute(rc, m_renderObjects);

		//モデルの描画
		ModelDraw(rc);

		//トゥーンの描画処理を実行
		m_tone.Execute(rc, m_mainRenderTarget);

		if (IsEnableBloom())
		{
			//ブルームの描画処理を実行
			m_bloom.Execute(rc, m_mainRenderTarget);
		}

		//エフェクトの描画
		EffectEngine::GetInstance()->Draw();

		//2D(フォントとスプライト)の描画
		SpriteFontDraw(rc);
		
		//メインレンダリングターゲットの内容をフレームバッファにコピーする
		CopyMainRenderTargetToFrameBuffer(rc);

		//レンダリングオブジェクトのクリア
		m_renderObjects.clear();
	}

	//モデルの描画
	void RenderingEngine::ModelDraw(RenderContext& rc)
	{
		RenderTarget* rts[] = {
			&m_mainRenderTarget,
			&m_depthRenderTarget,
			&m_normalRenderTarget,
		};

		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(3,rts);
		//レンダリングターゲットを設定
		rc.SetRenderTargetsAndViewport(3, rts);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetViews(3, rts);

		if (m_screenMode != enScreenMode_One)
		{
			for (int i = 0; i < m_viewPorts.size(); i++)
			{
				rc.SetViewport(m_viewPorts[i]);
				for (auto renderObj : m_renderObjects)
				{
					//モデルの描画
					renderObj->OnRenderModel(rc);
				}
			}
		}
		else
		{
			for (auto renderObj : m_renderObjects)
			{
				//モデルの描画
				renderObj->OnRenderModel(rc);
			}
		}

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTargets(3, rts);
	}

	//2D(フォントとスプライト)の描画
	void RenderingEngine::SpriteFontDraw(RenderContext& rc)
	{
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_2DRenderTarget);

		//メイン(モデル)用のスプライトの描画
		m_mainSprite.Draw(rc);

		for (auto renderObj : m_renderObjects)
		{
			//2D(フォントとスプライト)の描画
			renderObj->OnRender2D(rc);
		}

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);

		//ターゲットをメインに戻す
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		//2D(フォントとスプライト)の描画
		m_2DSprite.Draw(rc);

		//メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	//メインレンダリングターゲットの内容をフレームバッファにコピーする
	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		//レンダリングターゲットの設定
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		//ビューポートとシザリング短形の設定
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		//メインレンダリングターゲットをフレームバッファにコピーするためのスプライトの描画
		m_copyToFrameBufferSprite.Draw(rc);
	}
}