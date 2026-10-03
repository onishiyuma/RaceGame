#include "stdafx.h"
#include "StageLoader.h"


bool LoadStageDefinition(const std::string& filepath, StageDefinition& outStageDefinition)
{
	std::ifstream file(filepath);

	if (!file.is_open())
	{
		return false;
	}

	try
	{

		nlohmann::json jsonRoot;

		file >> jsonRoot;


		if (!jsonRoot.contains("objects"))
		{
			return false;
		}

		if (!jsonRoot["objects"].is_array())
		{
			return false;
		}

		outStageDefinition.objects.clear();

		for (const auto& jsonObject : jsonRoot["objects"])
		{
			//ジェイソンをもとにObjectDefinitionを作成する。

			ObjectDefinition definition;

			//タイプを設定
			definition.type =
				jsonObject.at("type").get<std::string>();

			//アセットIDを設定
			definition.assetId =
				jsonObject.at("assetId").get<std::string>();

			//位置を設定
			definition.position =
				ParseVector3(
					jsonObject.at("position")
				);

			//回転を設定
			definition.rotation =
				ParseQuaternion(
					jsonObject.at("rotation")
				);

			//スケールを設定
			definition.scale =
				ParseVector3(
					jsonObject.at("scale")
				);

			//プロパティを設定
			definition.properties =
				jsonObject.value(
					"properties",
					nlohmann::json::object()
				);

			outStageDefinition.objects.push_back(
				definition
			);
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}
