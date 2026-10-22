#pragma once
#include "Stage/ModelAssetDefinition.h"

/// <summary>
/// ステージで使用するアセットIDと
/// アセット情報（モデル、スケルトン、アニメーションなど）の対応を管理するクラス。
/// JSONファイルから対応表を読み込み、アセットIDからアセット情報を取得する。
///
/// アセットIDとアセット情報の対応を定義する。
/// JSONの記述方法については
/// Assets/StageAssets/README.md を参照。
/// </summary>
class AssetDatabase
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
	const ModelAssetDefinition* FindAssetDefinition(const std::string& assetId) const;

	/// <summary>
	/// 登録されているすべてのアセット定義を削除する。
	/// </summary>
	void Clear();

private:
	std::unordered_map<std::string, ModelAssetDefinition>m_assetMap;
};
