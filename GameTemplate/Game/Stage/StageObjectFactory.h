#pragma once
#include"Stage/StageObject.h"
#include <functional>
using Creator = std::function<std::unique_ptr<StageObject>()>;

struct ObjectDefinition;
class AssetDatabase;
class StageObjectFactory
{
public:
	StageObjectFactory();

	std::unique_ptr<StageObject> Create(
		const ObjectDefinition& definition,
		const AssetDatabase& assetDatabase
	);

private:
	std::unordered_map<std::string, Creator> m_creators;
};