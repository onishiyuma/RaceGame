#pragma once
#include "Stage/StageLoader/StageDefinition.h"
#include"Stage/StageAssetDatabase.h"
#include"Stage/StageObjectFactory.h"

/// <summary>
/// StageDefinitionをもとにStageを組み立てるクラス。
/// ステージオブジェクトの生成や、スポーン地点などの
/// ステージ情報の登録を行う。
/// </summary>
class Stage;
class StageBuilder
{
public:
	StageBuilder() = default;
	~StageBuilder() = default;
	/// <summary>
	/// StageDefinitionをもとにStageを組み立てる。
	/// </summary>
	/// <param name="definition"></param>
	/// <param name="stage"></param>
	/// <returns>全部成功したときだけtrue</returns>
	bool Build(
		const StageDefinition& definition,
		const StageAssetDatabase& assetDatabase,
		Stage& stage);

	/// <summary>
	/// ステージ構築中に発生したエラー一覧を取得する。
	/// </summary>
	const std::vector<std::string>& GetErrors() const
	{
		return m_errors;
	}

private:
	StageObjectFactory m_factory;
	std::vector<std::string> m_errors;
};

