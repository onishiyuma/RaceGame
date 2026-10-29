#pragma once
#include"stdafx.h"

/// <summary>
/// 車種ごとの定義データ。
/// </summary>
struct CarDefinition
{
	std::string id;
	std::string name;

	std::string modelAssetId;
	std::string iconAssetId;
};