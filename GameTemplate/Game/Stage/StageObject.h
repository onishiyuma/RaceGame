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

	void Draw(RenderContext& rc);

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
	virtual void OnDraw(RenderContext& rc) {}

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


protected:
	std::unique_ptr<StageModel> m_stageModel;

private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
	bool m_isActive = true;
};

