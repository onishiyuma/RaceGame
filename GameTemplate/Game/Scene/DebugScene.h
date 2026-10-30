#pragma once
#include "Scene/IScene.h"
#include "AssetDatabase.h"
#include "Stage/StageLoader/StageDefinition.h"
#include "Stage/Stage.h"
#include "Src/Car/CarDatabase.h"
#include "Src/Race/RaceEntryDefinition.h"
#include "Src/Car/Car.h"
#include "Src/Car/CarManager.h"

#include "Src/Car/CarController/PlayerController.h"
/// <summary>
/// ステージやカメラ操作などの動作確認を行うデバッグシーン。
/// </summary>
class DebugScene : public IScene
{
public:
	DebugScene() = default;
	~DebugScene() override = default;

protected:
	/// <summary>
	/// ステージ情報と必要なリソースをロードする。
	/// </summary>
	bool OnLoad() override;

	/// <summary>
	/// ロードした情報からステージを構築する。
	/// </summary>
	bool OnInit() override;

	/// <summary>
	/// デバッグカメラを設定する。
	/// </summary>
	void OnActivate() override;
	void OnDeactivate() override;

	/// <summary>
	/// デバッグカメラとステージを更新する。
	/// </summary>
	void OnUpdate(float deltaTime) override;


	void OnPostPhysicsUpdate() override;

	/// <summary>
	/// ステージを描画する。
	/// </summary>
	void OnRender(RenderContext& renderContext) override;

	/// <summary>
	/// ステージを破棄する。
	/// </summary>
	void OnUnload() override;

private:
	/// <summary>
	/// デバッグ用のカメラ操作。
	/// </summary>
	void UpdateDebugCamera();

private:
	Vector3 m_previousGravity = Vector3::Zero;
	AssetDatabase m_assetDatabase;
	CarDatabase m_carDatabase;
	StageDefinition m_stageDefinition;
	std::unique_ptr<Stage> m_stage;
	std::vector<RaceEntryDefinition> m_raceEntries;			//レース参加者の構成情報。
	CarManager m_carManager;								//車を管理するクラス。
	std::unique_ptr<PlayerController> m_playerController;	// プレイヤーの操作を管理する。
	std::string m_playerCarInstanceId;						// プレイヤーが操作する車のインスタンスID。
};
