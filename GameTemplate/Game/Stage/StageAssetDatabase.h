#pragma once

struct AnimationAssetDefinition
{
	std::string name;
	std::string path;
	bool isLoop = false;
};

struct StageAssetDefinition
{
	std::string modelPath;
	std::string skeletonPath;
	std::vector<AnimationAssetDefinition> animations;
};


/// <summary>
/// ステージで使用するアセットIDと
/// アセット情報（モデル、スケルトン、アニメーションなど）の対応を管理するクラス。
/// JSONファイルから対応表を読み込み、アセットIDからアセット情報を取得する。
///
/// 下のようにJSONファイルを作成し、
/// アセットIDとアセット情報の対応を定義する。
/// JSONの記述方法については
/// Assets/StageAssets/README.md を参照。
/// </summary>
class StageAssetDatabase
{
public:
	/// <summary>
	/// アセット定義JSONを読み込み、アセットIDとアセット情報の対応表を作成する。
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	bool Init(const std::string& filePath);

	/// <summary>
	/// アセットIDから対応するアセット情報を取得する。
	/// </summary>
	/// <param name="assetId"></param>
	/// <returns></returns>
	const StageAssetDefinition* FindAssetDefinition(
		const std::string& assetId
	) const;

private:
	std::unordered_map<std::string, StageAssetDefinition>m_assetMap;
};