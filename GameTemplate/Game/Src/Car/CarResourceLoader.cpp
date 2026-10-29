#include "stdafx.h"
#include "CarResourceLoader.h"
#include "AssetDatabase.h"
#include "ModelResourceLoader.h"
#include "CarDefinition.h"
#include"CarDatabase.h"
#include <unordered_set>

bool LoadCarResources(const CarDatabase& carDatabase, const AssetDatabase& assetDatabase)
{
	// 登録済みの車種ID一覧を取得する。
	const auto& carIds = carDatabase.GetCarIds();

	if (carIds.empty())
	{
		K2_ASSERT(
			false,
			"車種が一つも登録されていません。\n"
		);
		return false;
	}

	// 重複ロードを防ぐため、処理中にロード済みのモデルアセットIDを保持する。
	std::unordered_set<std::string> loadedIds;

	for (const auto& carId : carIds)
	{
		// 車種IDから車種定義を取得する。
		const CarDefinition* carDefinition = carDatabase.FindDefinition(carId);

		if (carDefinition == nullptr)
		{
			K2_ASSERT(
				false,
				"車種定義が見つかりません。\n"
				"車種ID : %s\n",
				carId.c_str()
			);
			return false;
		}

		// 車種定義からモデルアセットIDを取得する。
		const std::string& modelAssetId = carDefinition->modelAssetId;

		// 同じモデルアセットIDは重複してロードしない。
		if (!loadedIds.insert(modelAssetId).second)continue;

		// モデルアセットIDからアセット情報を取得する。
		const ModelAssetDefinition* asset = assetDatabase.FindAssetDefinition(modelAssetId);

		if (asset == nullptr)
		{
			K2_ASSERT(
				false,
				"モデルアセット情報が見つかりません。\n"
				"アセットID : %s\n",
				modelAssetId.c_str()
			);
			return false;
		}

		// モデルリソースをロードする。
		if (!LoadModelResources(*asset))return false;
	}

	return true;
}
