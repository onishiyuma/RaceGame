#include "stdafx.h"
#include "StageBuilder.h"
#include"Stage/Stage.h"

bool StageBuilder::Build(
	const StageDefinition& definition,
	const StageAssetDatabase& assetDatabase,
	Stage& stage
)
{
	stage.Clear();


	for (const auto& objectDef : definition.objects)
	{

		//ファクトリーでオブジェクトを作る。
		auto object =
			m_factory.Create(
				objectDef,
				assetDatabase
			);

		if (object == nullptr)
		{
			// 失敗したオブジェクトだけスキップして、
			// 他のオブジェクトの構築は続行する。

			std::string errorMessage = "StageObject creation failed. type: " + objectDef.type;

			if (!objectDef.assetId.empty())
			{
				errorMessage += ", assetId: " + objectDef.assetId;
			}

			m_errors.push_back(errorMessage);
			continue;
		}

		stage.AddObject(std::move(object));
	}

	return true;
}
