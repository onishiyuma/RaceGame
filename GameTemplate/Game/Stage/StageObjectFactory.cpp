#include "stdafx.h"
#include "StageObjectFactory.h"
#include "Stage/StageLoader/StageDefinition.h"
#include "Stage/StageAssetDatabase.h"
#include "Stage/Object/Prop.h"



StageObjectFactory::StageObjectFactory()
{
	m_creators["Prop"] = []
		{
			return std::make_unique<Prop>();;
		};

	m_creators["Coin"] = []
		{
			//return std::make_unique<Coin>();
			return  std::make_unique<StageObject>();
		};

	m_creators["SpawnPoint"] = []
		{
			//return std::make_unique<SpawnPoint>();
			return  std::make_unique<StageObject>();
		};

	m_creators["BoostPad"] = []
		{
			//return std::make_unique<BoostPad>();
			return  std::make_unique<StageObject>();
		};

	m_creators["ItemBox"] = []
		{
			//return std::make_unique<ItemBox>();
			return  std::make_unique<StageObject>();
		};

	m_creators["finishLine"] = []
		{
			//return std::make_unique<FinishLine>();
			return  std::make_unique<StageObject>();
		};
}

std::unique_ptr<StageObject>
StageObjectFactory::Create(
	const ObjectDefinition& definition,
	const StageAssetDatabase& assetDatabase)
{
	auto it = m_creators.find(definition.type);

	//対応するクリエイターがない
	if (it == m_creators.end())
	{
		// JSONに未知のtypeが指定されている。
		assert(false && "Unregistered StageObject type.");
		return nullptr;
	}

	auto object = it->second();

	if (!object->Init(
		definition,
		assetDatabase))
		return nullptr;

	return object;
}
