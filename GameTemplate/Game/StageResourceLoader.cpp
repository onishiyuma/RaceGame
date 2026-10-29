#include "stdafx.h"
#include "StageResourceLoader.h"
#include"AssetDatabase.h"
#include "ResourceManager.h"
#include "ModelAssetDefinition.h"
#include "ModelResourceLoader.h"

bool LoadStageResources(const StageDefinition& stageDefinition, const AssetDatabase& assetDatabase)
{
	for (const auto& object : stageDefinition.objects)
	{
		// assetIdを持たないならスキップする。
		if (object.assetId.empty())
		{
			continue;
		}

		//idからアセット情報を取得する。
		const ModelAssetDefinition* asset =
			assetDatabase.FindAssetDefinition(object.assetId);

		if (asset == nullptr)
		{
			K2_ASSERT(
				false,
				"アセット情報が見つかりません。\n"
				"アセットID : %s\n",
				object.assetId.c_str()
			);
			return false;
		}

		// モデルに必要なリソースをロードする。
		if (!LoadModelResources(*asset))
		{
			return false;
		}
	}
	return true;
}
