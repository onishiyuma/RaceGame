#pragma once
#include"Stage/StageLoader/StageDefinition.h"
class StageAssetDatabase;
/// <summary>
/// ステージのリソースをロードするクラス。
/// 一つでもロードに失敗したらfalseを返します。
/// </summary>
class StageResourceLoader
{
public:
	bool Load(
		const StageDefinition& stageDefinition,
		const StageAssetDatabase& assetDatabase
	);
};

