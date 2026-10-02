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
			if (g_resourceManager->LoadTks(asset->skeletonPath.c_str()) == false)
			{
				return false;
			}
		}

		//アニメーションパスが設定されていればロードする。
		if (!asset->animationPaths.empty())
		{
			for (const auto& animationPath : asset->animationPaths)
			{
				if (g_resourceManager->LoadTka(animationPath.c_str()) == false)
				{
					return false;
				}
			}
		}
		return true;
	}
}
