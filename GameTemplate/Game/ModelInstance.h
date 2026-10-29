#pragma once
/// <summary>
/// モデルのインスタンスを管理するクラス。
/// ロード済みリソースを使用してModelRenderを初期化する。
/// </summary>
struct  ModelAssetDefinition;
class ModelInstance
{
public:
	/// <summary>
	/// 初期化。
	/// 今は再初期化には対応してない。
	/// </summary>
	/// <param name="asset"></param>
	/// <param name="isShadowCaster"></param>
	/// <param name="isShadowReceiver"></param>
	/// <param name="enModelUpAxis"></param>
	/// <param name="alphaBlendMode"></param>
	/// <returns></returns>
	bool Init(
		const ModelAssetDefinition& asset,
		bool isShadowCaster = false,
		bool isShadowReceiver = false,
		EnModelUpAxis enModelUpAxis = enModelUpAxisY,
		AlphaBlendMode alphaBlendMode = AlphaBlendMode_None);

	void SetTRS(
		const Vector3& position,
		const Quaternion& rotation,
		const Vector3& scale)
	{
		m_modelRender.SetTRS(
			position,
			rotation,
			scale
		);
	}

	bool PlayAnimation(
		const std::string& name,
		float interpolateTime);

	void Update()
	{
		m_modelRender.Update();
	}

	void Draw(RenderContext& rc)
	{
		m_modelRender.Draw(rc);
	}

	const Model& GetModel() const
	{
		return m_modelRender.GetModel();
	}

private:
	ModelRender m_modelRender;

	// ModelRenderへ渡すAnimationClipの寿命を保持する。
	std::unique_ptr<AnimationClip[]>m_animationClips;

	// アニメーション名からアニメーション番号を取得するための対応表。
	std::unordered_map<std::string, int>m_animationIndices;

	int m_numAnimationClips = 0;
};