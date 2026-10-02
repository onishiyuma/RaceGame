#include "stdafx.h"
#include "StageAssetDatabase.h"
#include "json/json.hpp"
#include <fstream>

bool StageAssetDatabase::Init(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		return false;
	}

	nlohmann::json jsonRoot;

	try
	{
		//Jsonを読み込む
		//この処理中に例外が発生したらキャッチへ移動する。
		file >> jsonRoot;
	}
	catch (...)
	{
		//tryブロックで例外が発生したらfalseを返す。
		return false;
	}

	if (!jsonRoot.is_object())
	{
		return false;
	}

	m_assetMap.clear();

	for (auto it = jsonRoot.begin();
		it != jsonRoot.end();
		++it)
	{
		const std::string assetId = it.key();

		const auto& jsonAsset = it.value();


		StageAssetDefinition definition;

		definition.modelPath =
			jsonAsset.at("modelPath").get<std::string>();

		//スケルトンパスが設定されていれば設定する。
		if (jsonAsset.contains("skeletonPath"))
		{
			definition.skeletonPath =
				jsonAsset.at("skeletonPath").get<std::string>();
		}

		//アニメーションパスが設定されていれば設定する。
		if (jsonAsset.contains("animationPaths"))
		{
			for (const auto& animationPath :
				jsonAsset.at("animationPaths"))
			{
				definition.animationPaths.push_back(
					animationPath.get<std::string>());
			}
		}

		m_assetMap[assetId] = definition;
	}

	return true;
}

const StageAssetDefinition* StageAssetDatabase::FindAssetDefinition(const std::string& assetId) const
{
	auto it = m_assetMap.find(assetId);

	//見つからなかった場合はnullptrを返す。
	if (it == m_assetMap.end())
	{
		return nullptr;
	}

	return &it->second;
}
