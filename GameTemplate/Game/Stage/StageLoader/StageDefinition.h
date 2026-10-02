#pragma once
#include "stdafx.h"

struct ObjectDefinition
{
	std::string type;
	std::string assetId;

	Vector3 position = Vector3::Zero;
	Quaternion rotation = Quaternion::Identity;
	Vector3 scale = Vector3::One;
};

struct StageDefinition
{
	std::vector<ObjectDefinition> objects;
};
