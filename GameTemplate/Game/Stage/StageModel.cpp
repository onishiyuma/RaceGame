#include "stdafx.h"
#include "StageModel.h"
#include"Stage/StageAssetDatabase.h"

bool StageModel::Init(const StageAssetDefinition& asset)
{
	//tkmを取得する。
	TkmFile* tkmFile =
		g_resourceManager->GetTkm(
			asset.modelPath.c_str()
		);

	if (tkmFile == nullptr)
	{
		return false;
	}

	//tksを作る。
	TksFile* tksFile = nullptr;
	//スケルトンパスが設定されていれば取得する。
	if (!asset.skeletonPath.empty())
	{
		tksFile =
			g_resourceManager->GetTks(
				asset.skeletonPath.c_str()
			);

		if (tksFile == nullptr)
		{
			return false;
		}
	}


	m_numAnimationClips = static_cast<int>(asset.animations.size());

	//アニメーションが設定されていれば作る。
	if (m_numAnimationClips > 0)
	{
		//アニメーションクリップを作る。
		m_animationClips = std::make_unique<AnimationClip[]>(m_numAnimationClips);

		for (int i = 0; i < m_numAnimationClips; i++)
		{
			const auto& animation = asset.animations[i];

			//TKAを取得する。
			TkaFile* tkaFile =
				g_resourceManager->GetTka(
					animation.path.c_str()
				);

			if (tkaFile == nullptr)	return false;

			m_animationClips[i].Init(tkaFile);

			//ループフラグを設定する。
			m_animationClips[i].SetLoopFlag(animation.isLoop);

			m_animationIndices[animation.name] = i;
		}
	}

	m_modelRender.Init(
		tkmFile,
		tksFile,
		m_animationClips.get(),
		m_numAnimationClips
	);

	return true;
}

bool StageModel::PlayAnimation(const std::string& name, float interpolateTime)
{
	auto it = m_animationIndices.find(name);

	if (it == m_animationIndices.end())return false;

	m_modelRender.PlayAnimation(
		it->second,
		interpolateTime
	);

	return true;
}
