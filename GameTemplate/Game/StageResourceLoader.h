#pragma once
#include"Stage/StageLoader/StageDefinition.h"
class AssetDatabase;
/// <summary>
/// ステージのリソースをロードするクラス。
/// ステージ定義とassetIDとassetの対応表を受けて取ってそれをもとに、
/// 必要なものをロードする。
/// 一つでもロードに失敗したらfalseを返します。
/// </summary>
bool LoadStageResources(
	const StageDefinition& stageDefinition,
	const AssetDatabase& assetDatabase
);
