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
			stage.Clear();
			return false;
		}

		stage.AddObject(std::move(object));
	}

	return true;
}
