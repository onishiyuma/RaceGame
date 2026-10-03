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

	try
	{
		//Jsonを読み込む
		//この処理中に例外が発生したらキャッチへ移動する。
		nlohmann::json jsonRoot;
		file >> jsonRoot;

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

			//アニメーションが設定されていれば設定する。
			if (jsonAsset.contains("animations"))
			{
				for (const auto& jsonAnimation :
					jsonAsset.at("animations"))
				{
					AnimationAssetDefinition animation;

					animation.name =
						jsonAnimation.at("name")
						.get<std::string>();

					animation.path =
						jsonAnimation.at("path")
						.get<std::string>();

					animation.isLoop =
						jsonAnimation.value(
							"isLoop",
							false
						);

					definition.animations.push_back(
						animation
					);
				}
			}

			m_assetMap[assetId] = std::move(definition);
		}
	}
	catch (...)
	{
		//tryブロックで例外が発生したらfalseを返す。
		return false;
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
