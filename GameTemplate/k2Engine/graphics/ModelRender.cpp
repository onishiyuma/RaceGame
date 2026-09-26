#include "k2EnginePreCompile.h"
#include "ModelRender.h"

namespace nsK2Engine
{
	//通常描画用の初期化
	void ModelRender::Init(const char* filePath,//ファイルパスはあとでなくす
		TkmFile* tkmFile,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isShadowCaster,
		bool isShadowReceiver,
		AlphaBlendMode alphaBlendMode)
	{
		//スケルトンの初期化
		InitSkeleton(filePath);
		//アニメーションの初期化
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		//影を描画するなら
		if (isShadowCaster)
		{
			//モデル(影描画)の初期化
			InitShadowCasterDrawing(tkmFile, enModelUpAxis, alphaBlendMode);
		}
		//モデルの初期化
		InitModel(tkmFile, enModelUpAxis, isShadowReceiver, alphaBlendMode);
	}

	//半透明描画用の初期化
	void ModelRender::IniTranslucent(const char* filePath,
		TkmFile* tkmFile,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isShadowCaster,
		bool isShadowReceiver,
		AlphaBlendMode alphaBlendMode)
	{
		//スケルトンの初期化
		InitSkeleton(filePath);
		//アニメーションの初期化
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		//影を描画するなら
		if (isShadowCaster)
		{
			//モデル(影描画)の初期化
			InitShadowCasterDrawing(tkmFile, enModelUpAxis, alphaBlendMode);
		}
		//モデルの初期化
		InitModel(tkmFile, enModelUpAxis, isShadowReceiver, alphaBlendMode);
	}

	//スカイキューブの初期化
	void ModelRender::InitSkyCube(ModelInitData& initData)
	{
		m_model.Init(initData);
	}

	//スケルトンの初期化
	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	//アニメーションの初期化
	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;

		//アニメーションを設定していたら初期化する
		if (m_animationClips != nullptr)
		{
			m_animation.Init(m_skeleton,
				m_animationClips,
				m_numAnimationClips);
		}
	}

	//モデル(影描画)の初期化
	void ModelRender::InitShadowCasterDrawing(TkmFile* tkmFile, EnModelUpAxis enModelUpAxis, AlphaBlendMode alphaBlendMode)
	{
		//モデルの初期化
		ModelInitData shadowInitData;
		//tkmリソースを設定
		shadowInitData.m_tkmFile = tkmFile;
		//シェーダーファイルのファイルパスを指定
		shadowInitData.m_fxFilePath = "Assets/shader/shadowMap.fx";
		shadowInitData.m_vsEntryPointFunc = "VSMain";
		shadowInitData.m_psEntryPointFunc = "PSShadowCaster";

		//アニメーションが設定されていたら
		if (m_animationClips != nullptr)
		{
			//アニメーション用の頂点シェーダーのエントリーポイントに変更
			shadowInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//スケルトンの設定
			shadowInitData.m_skeleton = &m_skeleton;
		}

		//モデルの初期化
		shadowInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;
		shadowInitData.m_expandConstantBuffer = &g_renderingEngine->GetLight().lightPos;
		shadowInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetLight().lightPos);

		//モデルの上方向
		shadowInitData.m_modelUpAxis = enModelUpAxis;
		//描画モードを指定
		shadowInitData.m_alphaBlendMode = alphaBlendMode;

		//初期化
		m_shadowModel.Init(shadowInitData);
	}

	//モデルの初期化
	void ModelRender::InitModel(TkmFile* tkmFile, EnModelUpAxis enModelUpAxis, bool isShadowReceiver, AlphaBlendMode alphaBlendMode)
	{
		//モデルの初期化
		ModelInitData modelInitData;
		modelInitData.m_tkmFile = tkmFile;
		//シェーダーファイルのファイルパスを指定
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		modelInitData.m_vsEntryPointFunc = "VSMain";

		//影を描画するなら
		if (isShadowReceiver)
		{
			//影描画用のピクセルシェーダーのエントリーポイントの設定
			modelInitData.m_psEntryPointFunc = "PSShadowReceiverMain";
			//シャドウマップを取得
			modelInitData.m_expandShaderResoruceView[0] = &(g_renderingEngine->GetShadowBlur().GetBokeTexture());
		}
		else
		{
			//通常のピクセルシェーダーのエントリーポイントの設定
			modelInitData.m_psEntryPointFunc = "PSNormalMain";
		}

		//アニメーションが設定されていたら
		if (m_animationClips != nullptr)
		{
			//アニメーション用の頂点シェーダーのエントリーポイントに変更
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//スケルトンの設定
			modelInitData.m_skeleton = &m_skeleton;
		}

		//モデルの上方向
		modelInitData.m_modelUpAxis = enModelUpAxis;
		//描画モードを指定
		modelInitData.m_alphaBlendMode = alphaBlendMode;

		//定数バッファの設定
		modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetLight();
		modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetLight());

		//初期化
		m_model.Init(modelInitData);
	}

	//更新処理
	void ModelRender::Update()
	{
		//スケルトンを更新
		m_skeleton.Update(m_model.GetWorldMatrix());

		//モデルの更新
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		//ワールド行列の更新
		m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		//アニメーションを進める
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	//更新処理
	void ModelRender::Draw(RenderContext& rc)
	{
		//レンダリングオブジェクトの追加
		g_renderingEngine->AddRenderObject(this);
	}

	//シャドウマップの描画
	void ModelRender::OnRenderShadowMap(RenderContext& rc, Camera& ca)
	{
		//初期化されていたら
		if (m_shadowModel.IsInited())
		{
			//影描画用モデルの描画
			m_shadowModel.Draw(rc, ca, 1);
		}
	}

	//モデルの描画
	void ModelRender::OnRenderModel(RenderContext& rc)
	{
		//初期化されていたら
		if (m_model.IsInited())
		{
			//モデルの描画
			m_model.Draw(rc, 1);
		}
	}
}