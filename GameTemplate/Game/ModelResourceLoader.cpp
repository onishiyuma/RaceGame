#include "stdafx.h"
#include "ModelResourceLoader.h"
#include "AssetDatabase.h"
#include "ResourceManager.h"

bool LoadModelResources(const ModelAssetDefinition& asset)
{
	// モデルをロードする。
	if (!g_resourceManager->LoadTkm(asset.modelPath.c_str()))
	{
		K2_ASSERT(
			false,
			"モデルのロードに失敗しました。\n"
			"モデルパス : %s\n",
			asset.modelPath.c_str()
		);
		return false;
	}

	// スケルトンが設定されていればロードする。
	if (!asset.skeletonPath.empty())
	{
		if (!g_resourceManager->LoadTks(asset.skeletonPath.c_str()))
		{
			K2_ASSERT(
				false,
				"スケルトンのロードに失敗しました。\n"
				"パス : %s\n",
				asset.skeletonPath.c_str()
			);
			return false;
		}
	}

	// アニメーションをロードする。
	for (const auto& animation : asset.animations)
	{
		if (!g_resourceManager->LoadTka(animation.path.c_str()))
		{
			K2_ASSERT(
				false,
				"アニメーションのロードに失敗しました。\n"
				"パス : %s\n",
				animation.path.c_str()
			);
			return false;
		}
	}

	return true;
}