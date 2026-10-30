#pragma once
#include"Car.h"

struct CarCreateInfo
{
	std::string instanceId;
	const CarDefinition* definition = nullptr;
	const ModelAssetDefinition* modelAsset = nullptr;

	CarPerformance performance{};

	Vector3 position = Vector3::Zero;
	Quaternion rotation = Quaternion::Identity;
};

/// <summary>
/// レース中の車を生成醤油してまとめて更新、描画するクラス。
/// 車種の選択やスポーン地点IDの解決は担当しない。
/// </summary>
class CarManager
{
public:
	/// <summary>
	/// レース中の車を生成、初期化して登録する。
	/// インスタンスIDが空、または重複している場合、
	/// 車の初期化に失敗した場合はfalseを返す。
	/// </summary>
	/// <param name="createInfos"></param>
	bool CreateCars(
		const std::vector<CarCreateInfo>& createInfos
	);

	/// <summary>
	/// インスタンスIDから車を取得する。
	/// 読み取り専用
	/// </summary>
	/// <param name="instanceId"></param>
	/// <returns></returns>
	const Car* FindCar(const std::string& instanceId);

	/// <summary>
	/// 車ごとの入力を使って管理中のすべての車を更新する。
	/// インプットが指定されていない車にはニュートラル入力を渡す。
	/// </summary>
	/// <param name="inputs"></param>
	void Update(
		const std::unordered_map<std::string, CarInput>& inputs,
		float deltaTime
	);

	void PostPhysicsUpdate();

	void Draw(RenderContext& rc);

	void Clear();

private:
	struct ManagedCar
	{
		std::string instanceId;
		std::unique_ptr<Car> car;
	};

	std::vector<ManagedCar> m_cars;
};