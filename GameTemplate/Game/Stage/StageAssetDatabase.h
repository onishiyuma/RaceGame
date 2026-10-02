#pragma once

struct StageAssetDefinition
{
	std::string modelPath;
	std::string skeletonPath;
	std::vector<std::string> animationPaths;
};


/// <summary>
/// ステージで使用するアセットIDとアセット情報（モデルパス、スケルトンパスなど）の対応を管理するクラス。
/// JSONファイルから対応表を読み込み、アセットIDからアセット情報を取得する。
/// 
/// 
/// 下のようにJSONファイルを作成し、アセットIDとアセット情報の対応を定義する。
/// 
/// {
///     "Palm": {
///         "modelPath": "Assets/model/stage/Palm.tkm"
///     },
///
///     "Character": {
///         "modelPath": "Assets/model/character/Character.tkm",
///         "skeletonPath": "Assets/model/character/Character.tks"
///			"animationPaths": [
///				"Assets/model/character/Idle.tka",
///				"Assets/model/character/Run.tka"
///			]
///     };
/// }
/// 
/// モデルのパスは必須で、スケルトンやアニメーションのパスはあれば設定する。
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