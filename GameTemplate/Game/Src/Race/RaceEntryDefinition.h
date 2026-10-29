#pragma once
#include"stdafx.h"

/// <summary>
/// レース参加車の識別情報と割り当て情報。
/// 車の識別ID、使用する車種ID、スポーン位置IDをもつ。
/// </summary>
struct RaceEntryDefinition
{
	std::string instanceId;
	std::string carDefinitionId;
	std::string spawnPointId;
};