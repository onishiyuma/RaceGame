#include "stdafx.h"
#include "DebugScene.h"
#include <cmath>
#include "system/system.h"
#include "Stage/StageLoader/StageLoader.h"
#include "StageResourceLoader.h"
#include "Stage/StageBuilder.h"
#include "Src/Car/CarDefinition.h"
#include "Src/Car/CarPerformance.h"
#include "Src/Car/CarResourceLoader.h"
#include "Src/Race/RaceEntryBuilder.h"
#include "Stage/Object/SpawnPoint.h"
#include "Src/Car/CarController/PlayerController.h"

bool DebugScene::OnLoad()
{
	//ステージをロードする
	{
		// コースのアセットIDとアセットの対応表を読み込む。
		if (!m_assetDatabase.LoadDefinitions("Assets/assetsData/PhysicsDebugAssets.json"))return false;

		// ステージ配置情報を読み込む。
		if (!LoadStageDefinition(
			"Assets/stage/PhysicsDebugStage.json",
			m_stageDefinition
		))return false;

		// ステージ上のオブジェクトが使用するリソースをロードする。
		if (!LoadStageResources(m_stageDefinition, m_assetDatabase))
		{
			K2_ASSERT(
				false,
				"ステージリソースのロードに失敗しました。\n"
			);
			return false;
		}
	}

	//車をロードする
	{
		// 車のアセットIDとアセットの対応表を読み込む。
		if (!m_assetDatabase.LoadDefinitions("Assets/assetsData/CarAssets.json"))return false;

		// 車種定義を読み込む。
		if (!m_carDatabase.LoadDefinitions("Assets/carData/CarDefinitions.json"))return false;

		// 車のリソースをロードする。
		if (!LoadCarResources(
			m_carDatabase,
			m_assetDatabase))return false;
	}

	return true;
}

bool DebugScene::OnInit()
{
	//ステージを作成.
	{
		auto stage = std::make_unique<Stage>();
		StageBuilder stageBuilder;

		// ロードした定義から実際のStageを構築する。
		if (!stageBuilder.Build(
			m_stageDefinition,
			m_assetDatabase,
			*stage
		))return false;

		m_stage = std::move(stage);
	}


	//車を作成。
	{
		// ステージからスポーン地点一覧を取得する。
		auto spawnPoints = m_stage->GetObjectsOfType<SpawnPoint>();

		// レース参加車の構成情報を作成する。
		if (!BuildRaceEntries(
			spawnPoints,
			m_carDatabase,
			m_raceEntries
		))return false;

		// スポーン地点IDからオブジェクトを検索できるようにする。
		std::unordered_map<std::string, const SpawnPoint*> spawnPointMap;

		for (const auto* spawnPoint : spawnPoints)
		{
			// BuildRaceEntriesでnullptr、空ID、重複IDは検証済み。
			spawnPointMap.emplace(
				spawnPoint->GetId(),
				spawnPoint
			);
		}

		//TODO:将来的にCarCreateInfoを作る専用のビルダーを作る。
		// TODO:将来的に車のレースマネージャーで参加車の生成処理をまとめる。
		// レース参加車の構成情報から、実際の車の生成情報を作る。
		std::vector<CarCreateInfo> createInfos;
		createInfos.reserve(m_raceEntries.size());

		//レース参加者の構成情報をもとにレースに参加する車を作るための生成情報を作る。
		for (const auto& entry : m_raceEntries)
		{
			// 車種IDから車種定義を取得する。
			const CarDefinition* carDefinition = m_carDatabase.FindDefinition(entry.carDefinitionId);

			if (carDefinition == nullptr)
			{
				K2_ASSERT(
					false,
					"車種定義が見つかりません。\n"
					"車種ID : %s\n",
					entry.carDefinitionId.c_str()
				);
				return false;
			}

			// 車種定義のモデルアセットIDからモデル定義を取得する。
			const ModelAssetDefinition* modelAsset = m_assetDatabase.FindAssetDefinition(carDefinition->modelAssetId);

			if (modelAsset == nullptr)
			{
				K2_ASSERT(
					false,
					"車のモデルアセット定義が見つかりません。\n"
					"アセットID : %s\n",
					carDefinition->modelAssetId.c_str()
				);
				return false;
			}

			// スポーン地点IDから対応するスポーン地点を取得する。
			auto it = spawnPointMap.find(entry.spawnPointId);

			if (it == spawnPointMap.end())
			{
				K2_ASSERT(
					false,
					"スポーン地点が見つかりません。\n"
					"スポーン地点ID : %s\n",
					entry.spawnPointId.c_str()
				);
				return false;
			}

			const SpawnPoint* spawnPoint = it->second;

			//カークリエイトインフォーを作成。
			CarCreateInfo info;
			info.instanceId = entry.instanceId;
			info.definition = carDefinition;
			// TODO:現在は性能を仮設定している。
			// 車種・ステッカーなどから性能を構築するように変更する。
			info.performance = CarPerformance{};
			info.modelAsset = modelAsset;
			info.position = spawnPoint->GetPosition();
			info.rotation = spawnPoint->GetRotation();

			createInfos.push_back(std::move(info));
		}

		// 生成情報から全車を作る。
		if (!m_carManager.CreateCars(createInfos))
		{
			K2_ASSERT(
				false,
				"レース参加車の生成に失敗しました。\n"
			);
			return false;
		}

		// TODO:今後プレイヤーの参加情報から操作対象の車を決定するようにする。
		// 今はは先頭の車をプレイヤーとして扱っている。
		if (m_raceEntries.empty() || g_pad[0] == nullptr)
		{
			return false;
		}

		//プレイヤーのインスタンスIDを取得する。
		m_playerCarInstanceId = m_raceEntries.front().instanceId;

		// プレイヤーのコントローラーを作成する。
		m_playerController =
			std::make_unique<PlayerController>(*g_pad[0]);

	}

	return true;
}

void DebugScene::OnActivate()
{
	auto* physics = nsK2EngineLow::PhysicsWorld::GetInstance();
	const auto gravity = physics->GetDynamicWorld()->getGravity();
	m_previousGravity = Vector3(gravity.x(), gravity.y(), gravity.z());
	physics->SetGravity(Vector3(0.0f, -15.0f, 0.0f));
	nsK2EngineLow::PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	// デバッグカメラの遠クリップ距離を設定する。
	constexpr float debugFarClip = 50000.0f;
	g_camera3D->SetFar(debugFarClip);
	UpdateDebugCamera();
}

void DebugScene::OnUpdate(float deltaTime)
{
	// ステージ上のオブジェクトを更新する。
	if (m_stage)
	{
		m_stage->Update();
	}

	// 車ごとの操作入力を作成する。
	std::unordered_map<std::string, CarInput> inputs;

	// プレイヤーが操作する車の入力を取得する。
	if (m_playerController)
	{
		const Car* car = m_carManager.FindCar(m_playerCarInstanceId);

		if (car != nullptr)
		{
			inputs.emplace(
				m_playerCarInstanceId,
				m_playerController->GetInput()
			);
		}
	}

	// 管理しているすべての車を更新する。
	m_carManager.Update(inputs, deltaTime);
}

void DebugScene::OnPostPhysicsUpdate()
{
	m_carManager.PostPhysicsUpdate();
	UpdateDebugCamera();
}

void DebugScene::OnRender(RenderContext& renderContext)
{
	if (m_stage)
	{
		m_stage->Render(renderContext);
	}

	m_carManager.Draw(renderContext);
}

void DebugScene::OnDeactivate()
{
	nsK2EngineLow::PhysicsWorld::GetInstance()->SetGravity(m_previousGravity);
	nsK2EngineLow::PhysicsWorld::GetInstance()->DisableDrawDebugWireFrame();
}

void DebugScene::OnUnload()
{
	// プレイヤーの操作情報を破棄する。
	m_playerController.reset();
	m_playerCarInstanceId.clear();

	// 車を破棄する。
	m_carManager.Clear();
	m_raceEntries.clear();
	// シーンが所有しているステージを破棄する。
	m_stage.reset();
}

void DebugScene::UpdateDebugCamera()
{
	const Car* car = m_carManager.FindCar(m_playerCarInstanceId);
	if (car == nullptr) return;

	const Vector3& position = car->GetPosition();
	g_camera3D->SetPosition(position + Vector3(120.0f, 75.0f, -180.0f));
	g_camera3D->SetTarget(position);
	g_camera3D->Update();
}
