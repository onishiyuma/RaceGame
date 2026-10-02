#pragma once
#include"Stage/StageLoader/StageDefinition.h"
class StageAssetDatabase;
/// <summary>
/// ステージのリソースをロードするクラス。
/// </summary>
class StageResourceLoader
{
public:
	bool Load(
		const StageDefinition& stageDefinition,
		const StageAssetDatabase& assetDatabase
	);
};

