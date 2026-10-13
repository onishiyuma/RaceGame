#include "stdafx.h"
#include "StageLoader.h"

namespace
{
	constexpr float UNITY_TO_ENGINE_LENGTH = 9.0f;	//Unityの距離単位をエンジン側の距離単位へ変換する倍率。
	constexpr float UNITY_TO_ENGINE_SCALE = 390.0f;	//Unityのスケール単位をエンジン側のスケール単位へ変換する倍率。

	/// <summary>
	/// Unityの座標系からエンジンの座標系へ変換するための関数群。k
	/// </summary>
	namespace UnityTransformConverter
	{
		Vector3 ToEnginePosition(const Vector3& position)
		{
			return position * UNITY_TO_ENGINE_LENGTH;
		}

		Vector3 ToEngineScale(const Vector3& scale)
		{
			return scale;
		}

		Quaternion ToEngineRotation(const Quaternion& rotation)
		{
			return rotation;
		}
	}
}


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
			{
				//Jsonからの位置情報を取得。
				const Vector3 jsonPosition = ParseVector3(jsonObject.at("position"));
				//Unityの座標系からエンジンの座標系へ変換する。
				definition.position = UnityTransformConverter::ToEnginePosition(jsonPosition);
			}

			//回転を設定
			{
				//Jsonからの回転情報を取得。
				const Quaternion jsonRotation = ParseQuaternion(jsonObject.at("rotation"));
				//Unityの座標系からエンジンの座標系へ変換する。
				definition.rotation = UnityTransformConverter::ToEngineRotation(jsonRotation);
			}


			//スケールを設定
			{
				//Jsonからのスケール情報を取得。
				const Vector3 jsonScale = ParseVector3(jsonObject.at("scale"));
				//Unityの座標系からエンジンの座標系へ変換する。
				definition.scale = UnityTransformConverter::ToEngineScale(jsonScale);
			}

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