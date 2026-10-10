#pragma once
#include "Scene/IScene.h"
#include "Stage/StageAssetDatabase.h"
#include "Stage/StageLoader/StageDefinition.h"
#include "Stage/Stage.h"

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

	/// <summary>
	/// デバッグカメラとステージを更新する。
	/// </summary>
	void OnUpdate() override;

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
	StageAssetDatabase m_assetDatabase;
	StageDefinition m_stageDefinition;

	std::unique_ptr<Stage> m_stage;
};
