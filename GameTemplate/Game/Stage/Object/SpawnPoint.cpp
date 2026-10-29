#include"stdafx.h"
#include "SpawnPoint.h"

bool SpawnPoint::OnInit(const nlohmann::json& properties)
{
	if (!properties.is_object())
	{
		K2_ASSERT(false, "スポーン地点のプロパティがオブジェクトではありません。\n");
		return false;
	}

	if (!properties.contains("spawnPointId"))
	{
		K2_ASSERT(false, "spawnPointIdが定義されていません。\n");
		return false;
	}

	if (!properties.at("spawnPointId").is_string())
	{
		K2_ASSERT(false, "spawnPointIdの型が文字列ではありません。\n");
		return false;
	}

	m_id = properties.at("spawnPointId").get<std::string>();

	if (m_id.empty())
	{
		K2_ASSERT(
			false,
			"スポーン地点IDが空です。\n"
		);
		return false;
	}

	return true;
}
