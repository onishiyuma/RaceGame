#include "stdafx.h"
#include "RaceEntryBuilder.h"
#include "Src/Car/CarDatabase.h"
#include <unordered_set>
#include "Stage/Object/SpawnPoint.h"

bool BuildRaceEntries(
	std::vector<const SpawnPoint*>& spawnPoints,
	const CarDatabase& carDatabase,
	std::vector<RaceEntryDefinition>& outEntries)
{
	// 登録済みの車種ID一覧を取得する。
	const auto& carIds = carDatabase.GetCarIds();

	// 使用できる車種がなければ生成できない。
	if (carIds.empty())
	{
		K2_ASSERT(
			false,
			"レース参加車の生成に失敗しました。\n"
			"車種が一つも登録されていません。\n"
		);
		return false;
	}

	// スポーン位置が一つもなければ失敗。
	if (spawnPoints.empty())
	{
		K2_ASSERT(
			false,
			"レース参加車の生成に失敗しました。\n"
			"ステージにスポーン位置が存在しません。\n"
		);
		return false;
	}

	// 途中で失敗しても出力先に影響しないよう、
	// 一時的な配列に構成情報を作成する。
	std::vector<RaceEntryDefinition> newEntries;
	newEntries.reserve(spawnPoints.size());

	// スポーン位置IDの重複を防ぐため、使用済みのIDを一時的に保持する。
	std::unordered_set<std::string> usedSlots;

	for (const auto* spawnPoint : spawnPoints)
	{
		// スポーン地点が無効。
		if (spawnPoint == nullptr)
		{
			K2_ASSERT(
				false,
				"レース参加車の生成に失敗しました。\n"
				"スポーン地点がnullptrです。\n"
			);
			return false;
		}

		const std::string& spawnPointId = spawnPoint->GetId();

		// スポーン地点IDが空。
		if (spawnPointId.empty())
		{
			K2_ASSERT(
				false,
				"レース参加車の生成に失敗しました。\n"
				"スポーン地点IDが空です。\n"
			);
			return false;
		}

		// スポーン地点IDが重複している。
		if (!usedSlots.insert(spawnPointId).second)
		{
			K2_ASSERT(
				false,
				"レース参加車の生成に失敗しました。\n"
				"スポーン地点IDが重複しています。\n"
				"スポーン地点ID : %s\n",
				spawnPointId.c_str()
			);
			return false;
		}

		const size_t index = newEntries.size();

		RaceEntryDefinition entry;

		//インスタンスIDを設定。
		// 0（一番最初）ならインスタンスIDをプレイヤーにしてそれ以降はCPUにする。
		// TODO:仮だからプレイヤーの人数を変えたりしたらここも変更する。
		entry.instanceId =
			index == 0 ? "Player1" : "CPU" + std::to_string(index);

		// 登録済みの車種を順番に割り当てる。
		// 剰余演算で車種IDの配列を循環させて、参加車が車種数を超えても対応する。
		// TODO:今は車種を自動で順番に割り当てているけど、
		// プレイヤーの車種選択やCPUごとの車種設定を実装したりするならは変更しないといけない。
		entry.carDefinitionId = carIds[index % carIds.size()];

		// 使用するスポーン地点IDを設定する。
		entry.spawnPointId = spawnPointId;

		newEntries.push_back(std::move(entry));
	}

	// 全構成情報の生成に成功した場合だけ出力先に反映する。
	outEntries = std::move(newEntries);

	return true;
}