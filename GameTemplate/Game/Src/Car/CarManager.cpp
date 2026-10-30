#include"stdafx.h"
#include "CarManager.h"
#include <unordered_set>

bool CarManager::CreateCars(const std::vector<CarCreateInfo>& createInfos)
{
	std::vector<ManagedCar> newCars;
	newCars.reserve(createInfos.size());

	//インスタンスIDが重複しているか確認する用の変数。
	//unordered_setはinsertで追加したものが重複していたらfalseを返す。
	std::unordered_set<std::string> usedIds;

	for (const auto& info : createInfos)
	{
		// インスタンスIDが設定されているか確認する。
		if (info.instanceId.empty())
		{
			K2_ASSERT(
				false,
				"車のインスタンスIDが空です。\n"
			);
			return false;
		}

		// 車の定義データが設定されているか確認する。
		if (info.definition == nullptr)
		{
			K2_ASSERT(
				false,
				"車の定義データが設定されていません。\n"
				"インスタンスID : %s\n",
				info.instanceId.c_str()
			);
			return false;
		}

		// モデルアセットが設定されているか確認する。
		if (info.modelAsset == nullptr)
		{
			K2_ASSERT(
				false,
				"車のモデルアセットが設定されていません。\n"
				"インスタンスID : %s\n",
				info.instanceId.c_str()
			);
			return false;
		}

		// インスタンスIDが重複していないか確認する。
		if (!usedIds.insert(info.instanceId).second)
		{
			K2_ASSERT(
				false,
				"車のインスタンスIDが重複しています。\n"
				"インスタンスID : %s\n",
				info.instanceId.c_str()
			);
			return false;
		}

		auto car = std::make_unique<Car>();

		// 車の初期化に失敗した場合は登録しない。
		if (!car->Init(
			*info.definition,
			info.performance,
			*info.modelAsset,
			info.position,
			info.rotation
		))
		{
			K2_ASSERT(
				false,
				"車の初期化に失敗しました。\n"
				"インスタンスID : %s\n",
				info.instanceId.c_str()
			);
			return false;
		}

		newCars.push_back(
			ManagedCar{
				info.instanceId,
				std::move(car)
			}
		);
	}

	// すべて生成できた場合だけ管理対象を置き換える。
	m_cars = std::move(newCars);

	return true;
}

const Car* CarManager::FindCar(const std::string& instanceId)
{
	for (auto& managed : m_cars)
	{
		if (managed.instanceId == instanceId)
		{
			return managed.car.get();
		}
	}

	return nullptr;
}

void CarManager::Update(
	const std::unordered_map<std::string,
	CarInput>& inputs, float deltaTime)
{
	const CarInput neutralInput{};

	for (auto& managed : m_cars)
	{
		//対象の車のインプットをIDから取得する。
		const auto it = inputs.find(managed.instanceId);

		//インプットがないならニュートラルを入れる。
		const CarInput& input =
			it != inputs.end() ? it->second : neutralInput;

		//インプットを渡して車を更新。
		managed.car->Update(input, deltaTime);
	}
}

void CarManager::PostPhysicsUpdate()
{
	for (auto& managed : m_cars)
	{
		managed.car->PostPhysicsUpdate();
	}
}

void CarManager::Draw(RenderContext& rc)
{
	for (auto& managed : m_cars)
	{
		managed.car->Render(rc);
	}
}

void CarManager::Clear()
{
	m_cars.clear();
}
