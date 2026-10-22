#pragma once

#include <string>
#include <vector>

struct AnimationAssetDefinition
{
	std::string name;
	std::string path;
	bool isLoop = false;
};

struct ModelAssetDefinition
{
	std::string modelPath;
	std::string skeletonPath;
	std::vector<AnimationAssetDefinition> animations;
};
