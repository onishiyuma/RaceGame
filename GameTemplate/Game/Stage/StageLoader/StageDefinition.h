#pragma once
#include "stdafx.h"
#include"Json/json.hpp"

struct ObjectDefinition
{
	std::string type;
	std::string assetId;

	Vector3 position = Vector3::Zero;
	Quaternion rotation = Quaternion::Identity;
	Vector3 scale = Vector3::One;

	// 好きなプロパティを入れるるためのJSONオブジェクト
	nlohmann::json properties;
};

/// <summary>
/// ステージの定義を保持する構造体。
/// 中にはオブジェクトの定義の配列が入っており、ステージに配置するオブジェクトの情報を保持している。
/// </summary>
struct StageDefinition
{
	std::vector<ObjectDefinition> objects;
};
