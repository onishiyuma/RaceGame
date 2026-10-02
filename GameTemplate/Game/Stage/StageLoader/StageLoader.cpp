#include "stdafx.h"
#include "StageLoader.h"


bool LoadStage(const std::string& filepath, StageDefinition& outSceneDefinition)
{
	std::ifstream file(filepath);

	if (!file.is_open())
	{
		return false;
	}

	nlohmann::json jsonRoot;

	try
	{
		file >> jsonRoot;
	}
	catch (...)
	{
		return false;
	}

	if (!jsonRoot.contains("objects"))
	{
		return false;
	}

	if (!jsonRoot["objects"].is_array())
	{
		return false;
	}

	outSceneDefinition.objects.clear();

	for (const auto& jsonObject : jsonRoot["objects"])
	{
		ObjectDefinition definition;

		definition.type =
			jsonObject.at("type").get<std::string>();

		definition.assetId =
			jsonObject.at("assetId").get<std::string>();

		definition.position =
			ParseVector3(
				jsonObject.at("position")
			);

		definition.rotation =
			ParseQuaternion(
				jsonObject.at("rotation")
			);

		definition.scale =
			ParseVector3(
				jsonObject.at("scale")
			);

		outSceneDefinition.objects.push_back(
			definition
		);
	}

	return true;
}
