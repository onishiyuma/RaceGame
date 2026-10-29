#pragma once
class CarDefinition;
#include"CarDefinition.h"

/// <summary>
/// 車種IDと車種定義の対応を管理するクラス。
/// JSONファイルから車種情報を読み込み、
/// 定義情報を取得する。
/// </summary>
class CarDatabase
{
public:
	/// <summary>
	/// JSONから車種定義を読み込み、登録する。
	/// </summary>
	bool LoadDefinitions(const std::string& filePath);

	/// <summary>
	/// 車種IDから車種定義を取得する。
	/// </summary>
	const CarDefinition* FindDefinition(
		const std::string& carId
	) const;

	/// <summary>
	/// 登録されている車種IDの一覧を取得する。
	/// </summary>
	const std::vector<std::string>& GetCarIds() const;

	/// <summary>
	/// 登録済みの車種定義をすべて削除する。
	/// </summary>
	void Clear();

private:
	std::unordered_map<std::string, CarDefinition> m_carMap;
	// 自動選択などで使用する車種ID一覧。
	std::vector<std::string> m_carIds;
};