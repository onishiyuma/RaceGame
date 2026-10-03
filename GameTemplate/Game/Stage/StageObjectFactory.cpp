#include "stdafx.h"
#include "StageObjectFactory.h"
#include "Stage/StageLoader/StageDefinition.h"
#include "Stage/StageAssetDatabase.h"
#include "Stage/Object/Prop.h"



StageObjectFactory::StageObjectFactory()
{
	m_creators["Prop"] = []
		{
			//return std::make_unique<Prop>();
			return std::make_unique<Prop>();;
		};

	m_creators["SpawnPoint"] = []
		{
			//return std::make_unique<SpawnPoint>();
			return nullptr;
		};

	m_creators["BoostPad"] = []
		{
			//return std::make_unique<BoostPad>();
			return nullptr;
		};

	m_creators["ItemBox"] = []
		{
			//return std::make_unique<ItemBox>();
			return nullptr;
		};
}

std::unique_ptr<StageObject>
StageObjectFactory::Create(
	const ObjectDefinition& definition,
	const StageAssetDatabase& assetDatabase)
{
	auto it = m_creators.find(definition.type);

	if (it == m_creators.end())
	{
		// JSONに未知のtypeが指定されている。
		assert(false && "Unregistered StageObject type.");
		return nullptr;
	}

	if (it == m_creators.end())return nullptr;

	auto object = it->second();

	if (!object->Init(
		definition,
		assetDatabase))
		return nullptr;

	return object;
}
