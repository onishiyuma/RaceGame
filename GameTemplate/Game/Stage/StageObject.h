#pragma once
#include"Stage/StageModel.h"
#include"Json/json.hpp"

struct ObjectDefinition;
class StageAssetDatabase;
class StageObject
{
public:
	virtual ~StageObject() = default;

	bool Init(
		const ObjectDefinition& definition,
		const StageAssetDatabase& assetDatabase,
		const bool isActive = true
	);

	bool Start();

	void Update();

	void Render(RenderContext& rc);

protected:
	/// <summary>
	/// 派生クラスでオブジェクトの初期化を行うための仮想関数。
	/// この中にオブジェクト固有の初期化処理を実装する。
	/// </summary>
	/// <param name="properties"></param>
	/// <returns></returns>
	virtual bool OnInit(
		const nlohmann::json& properties)
	{
		return true;
	}

	virtual bool OnStart() { return true; }

	/// <summary>
	/// 派生クラス固有の更新。
	/// </summary>
	virtual void OnUpdate() {}

	/// <summary>
	/// 派生クラス固有の描画。
	/// </summary>
	virtual void OnRender(RenderContext& rc) {}

	/// <summary>
	/// アニメーションを再生する。
	/// </summary>
	/// <param name="name">ステージのアセット設定のjsonで設定したアニメーション名を入れてください</param>
	/// <param name="interpolateTime"></param>
	/// <returns></returns>
	bool PlayAnimation(
		const std::string& name,
		float interpolateTime = 0.0f);

	void SetActive(bool isActive)
	{
		m_isActive = isActive;
	}

	/// <summary>
/// オブジェクトの位置を設定する。
/// </summary>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
		UpdateModelTransform();
	}

	/// <summary>
	/// オブジェクトの回転を設定する。
	/// </summary>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
		UpdateModelTransform();
	}

	/// <summary>
	/// オブジェクトの拡大率を設定する。
	/// </summary>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		UpdateModelTransform();
	}

	/// <summary>
	/// オブジェクトの位置を取得する。
	/// </summary>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// オブジェクトの回転を取得する。
	/// </summary>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/// <summary>
	/// オブジェクトの拡大率を取得する。
	/// </summary>
	const Vector3& GetScale() const
	{
		return m_scale;
	}

private:
	/// <summary>
	/// StageObjectのTransformをStageModelに反映する。
	/// </summary>
	void UpdateModelTransform();

protected:
	std::unique_ptr<StageModel> m_stageModel;

private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
	bool m_isActive = true;
};

