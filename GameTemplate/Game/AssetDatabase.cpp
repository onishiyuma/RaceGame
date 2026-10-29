#include "stdafx.h"
#include "AssetDatabase.h"
#include "json/json.hpp"
#include <fstream>

bool AssetDatabase::LoadDefinitions(const std::string& filePath)
{
	std::ifstream file(filePath);

	//ファイルが開けない。
	if (!file.is_open())
	{
		K2_ASSERT(
			false,
			"アセット定義JSONを開けません。\n"
			"ファイルパス : %s\n",
			filePath.c_str()
		);
		return false;
	}

	// 読み込み途中で失敗しても既存の定義に影響しないよう、
	// 一時的なマップに読み込む。
	std::unordered_map<std::string, ModelAssetDefinition> newAssets;


	//Jsonを読み込む
	//この処理中に例外が発生したらキャッチへ移動する。
	try
	{
		nlohmann::json jsonRoot;

		//ファイルを読み込む。
		file >> jsonRoot;

		//ルートがオブジェクトじゃない。
		if (!jsonRoot.is_object())
		{
			K2_ASSERT(
				false,
				"JSONのルートがオブジェクトではありません。\n"
				"ファイルパス : %s\n",
				filePath.c_str()
			);
			return false;
		}

		for (auto it = jsonRoot.begin();
			it != jsonRoot.end();
			++it)
		{
			const std::string assetId = it.key();
			const auto& jsonAsset = it.value();

			// アセットIDが既に登録されていないか
			// もしくはアセット空じゃないか確認する。
			if (assetId.empty() ||
				m_assetMap.find(assetId) != m_assetMap.end() ||
				newAssets.find(assetId) != newAssets.end())
			{
				K2_ASSERT(
					false,
					"アセットIDが空、または重複しています。\n"
					"アセットID : %s\n"
					"ファイルパス : %s\n",
					assetId.c_str(),
					filePath.c_str()
				);
				return false;
			}

			//アセット定義がオブジェクトじゃない。
			if (!jsonAsset.is_object())
			{
				K2_ASSERT(
					false,
					"アセット定義が不正です。\n"
					"アセットID : %s\n",
					assetId.c_str()
				);
				return false;
			}



			////////モデルディフィニションを作成する。////////

			ModelAssetDefinition definition;

			definition.modelPath = jsonAsset.at("modelPath").get<std::string>();

			//スケルトンパスが設定されていれば。
			if (jsonAsset.contains("skeletonPath"))
			{
				//スケルトンパスを設定する。
				definition.skeletonPath = jsonAsset.at("skeletonPath").get<std::string>();
			}

			//アニメーションが設定されていれば。
			if (jsonAsset.contains("animations"))
			{
				//アニメーションを設定する。
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

					//アニメーション定義が不正。
					if (animation.name.empty() || animation.path.empty())
					{
						K2_ASSERT(
							false,
							"アニメーション定義が不正です。\n"
							"アセットID : %s\n",
							assetId.c_str()
						);
						return false;
					}

					definition.animations.push_back(
						animation
					);
				}
			}

			newAssets.emplace(assetId, std::move(definition));
		}
	}
	catch (const std::exception& e)
	{
		K2_ASSERT(
			false,
			"アセット定義JSONの読み込みに失敗しました。\n"
			"ファイルパス : %s\n"
			"エラー : %s\n",
			filePath.c_str(),
			e.what()
		);
		return false;
	}

	// 全定義の検証が成功した場合だけ、既存のマップに追加する。
	for (auto& asset : newAssets)
	{
		m_assetMap.emplace(asset.first, std::move(asset.second));
	}
	return true;
}

const ModelAssetDefinition* AssetDatabase::FindAssetDefinition(const std::string& assetId) const
{
	auto it = m_assetMap.find(assetId);

	//見つからなかった場合はnullptrを返す。
	if (it == m_assetMap.end())
	{
		return nullptr;
	}

	return &it->second;
}

void AssetDatabase::Clear()
{
	m_assetMap.clear();
}
