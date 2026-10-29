#pragma once
#include "RaceEntryDefinition.h"

struct StageDefinition;
class CarDatabase;
class SpawnPoint;

/// <summary>
/// ステージのスポーン情報と車種定義から、
/// レース参加車の構成情報を生成する。
/// </summary>
bool BuildRaceEntries(
	std::vector<const SpawnPoint*>& spawnPoints,
	const CarDatabase& carDatabase,
	std::vector<RaceEntryDefinition>& outEntries
);