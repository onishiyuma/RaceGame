#pragma once
#include "Stage/StageObject.h"

/// <summary>
/// ステージ上のスポーン地点を表すクラス。
/// スポーン地点の識別IDを保持し、
/// 位置と回転はStageObjectから継承する。
/// </summary>
class SpawnPoint : public StageObject
{
public:
	const std::string& GetId() const
	{
		return m_id;
	}

protected:
	/// <summary>
	/// スポーン地点固有の設定を読み込む。
	/// </summary>
	bool OnInit(const nlohmann::json& properties) override;

private:
	std::string m_id;
};