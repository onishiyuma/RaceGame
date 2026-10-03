#pragma once
/// <summary>
/// ステージオブジェクトが使用するモデルを管理するクラス。
/// ロード済みリソースを使用してModelRenderを初期化する。
/// </summary>
struct  StageAssetDefinition;
class StageModel
{
public:
	bool Init(const StageAssetDefinition& asset);

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