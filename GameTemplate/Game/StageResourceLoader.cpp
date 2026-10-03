#include "stdafx.h"
#include "StageResourceLoader.h"
#include"Stage/StageAssetDatabase.h"
#include "ResourceManager.h"

bool StageResourceLoader::Load(
	const StageDefinition& stageDefinition,
	const StageAssetDatabase& assetDatabase)
{
	for (const auto& object : stageDefinition.objects)
	{
		// assetIdを持たないならスキップする。
		if (object.assetId.empty())
		{
			continue;
		}

		//idからアセット情報を取得する。
		const StageAssetDefinition* asset =
			assetDatabase.FindAssetDefinition(object.assetId);

		if (asset == nullptr)
		{
			return false;
		}

		if (
			//モデルをロードする。
			g_resourceManager->LoadTkm(asset->modelPath.c_str())
			== false)
		{
			return false;
		}

		//スケルトンパスが設定されていればロードする。
		if (!asset->skeletonPath.empty())
		{
			if (!
				g_resourceManager->LoadTks(asset->skeletonPath.c_str())
				)
			{
				return false;
			}
		}

		//アニメーションが設定されていればロードする。
		if (!asset->animations.empty())
		{
			for (const auto& animation : asset->animations)
			{
				if (!g_resourceManager->LoadTka(animation.path.c_str()))
					return false;

			}
		}
	}
	return true;
}
