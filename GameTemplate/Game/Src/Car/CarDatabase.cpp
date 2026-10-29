#include"stdafx.h"
#include "CarDatabase.h"
#include "CarDefinition.h"
#include "json/json.hpp"
#include <fstream>

bool CarDatabase::LoadDefinitions(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		K2_ASSERT(
			false,
			"車種定義JSONを開けません。\n"
			"ファイルパス : %s\n",
			filePath.c_str()
		);
		return false;
	}

	// 読み込み中に失敗しても既存の定義に影響しないようにする。
	std::unordered_map<std::string, CarDefinition> newCars;
	std::vector<std::string> newCarIds;

	try
	{
		nlohmann::json jsonRoot;
		file >> jsonRoot;

		if (!jsonRoot.is_object())
		{
			K2_ASSERT(
				false,
				"車種定義JSONのルートがオブジェクトではありません。\n"
				"ファイルパス : %s\n",
				filePath.c_str()
			);
			return false;
		}

		for (auto it = jsonRoot.begin();
			it != jsonRoot.end();
			++it)
		{
			const std::string carId = it.key();
			const auto& jsonCar = it.value();

			// IDが空、または登録済みなら失敗。
			if (carId.empty() ||
				m_carMap.find(carId) != m_carMap.end() ||
				newCars.find(carId) != newCars.end())
			{
				K2_ASSERT(
					false,
					"車種IDが空、または重複しています。\n"
					"車種ID : %s\n",
					carId.c_str()
				);
				return false;
			}

			if (!jsonCar.is_object())
			{
				K2_ASSERT(
					false,
					"車種定義が不正です。\n"
					"車種ID : %s\n",
					carId.c_str()
				);
				return false;
			}

			CarDefinition definition;

			definition.id = carId;

			definition.name =
				jsonCar.at("name").get<std::string>();

			definition.modelAssetId =
				jsonCar.at("modelAssetId").get<std::string>();

			// アイコンは未設定でもよい。
			definition.iconAssetId =
				jsonCar.value(
					"iconAssetId",
					std::string()
				);

			// 必須項目を検証する。
			if (definition.name.empty() ||
				definition.modelAssetId.empty())
			{
				K2_ASSERT(
					false,
					"車種定義の必須項目が空です。\n"
					"車種ID : %s\n",
					carId.c_str()
				);
				return false;
			}

			newCarIds.push_back(carId);

			newCars.emplace(
				carId,
				std::move(definition)
			);
		}
	}
	catch (const std::exception& e)
	{
		K2_ASSERT(
			false,
			"車種定義JSONの読み込みに失敗しました。\n"
			"ファイルパス : %s\n"
			"エラー : %s\n",
			filePath.c_str(),
			e.what()
		);
		return false;
	}

	// 検証が成功した場合だけ登録する。
	for (auto& car : newCars)
	{
		m_carMap.emplace(
			car.first,
			std::move(car.second)
		);
	}

	m_carIds.insert(
		m_carIds.end(),
		newCarIds.begin(),
		newCarIds.end()
	);

	return true;
}

const CarDefinition* CarDatabase::FindDefinition(
	const std::string& carId) const
{
	auto it = m_carMap.find(carId);

	if (it == m_carMap.end())
	{
		return nullptr;
	}

	return &it->second;
}

const std::vector<std::string>& CarDatabase::GetCarIds() const
{
	return m_carIds;
}

void CarDatabase::Clear()
{
	m_carMap.clear();
	m_carIds.clear();
}