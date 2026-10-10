#include"stdafx.h"
#include "StageObject.h"
#include"Stage/StageAssetDatabase.h"
#include "Stage/StageLoader/StageDefinition.h"

bool StageObject::Init(
	const ObjectDefinition& definition,
	const StageAssetDatabase& assetDatabase,
	const bool isActive
)
{
	m_position = definition.position;
	m_rotation = definition.rotation;
	m_scale = definition.scale;
	m_isActive = isActive;

	//アセットIDが入っていたらステージモデルを作る。
	if (!definition.assetId.empty())
	{
		const StageAssetDefinition* asset =
			assetDatabase.FindAssetDefinition(
				definition.assetId
			);

		// アセットIDが見つからなかったらfalseを返す。
		if (asset == nullptr)return false;

		auto stageModel = std::make_unique<StageModel>();

		//	モデルの初期化に失敗したらfalseを返す。
		if (!stageModel->Init(*asset))return false;

		stageModel->SetTRS(
			m_position,
			m_rotation,
			m_scale
		);

		//モデルの初期化に成功してからメンバとして持つ。
		m_stageModel = std::move(stageModel);
	}

	return OnInit(definition.properties);
}

bool StageObject::Start()
{
	return OnStart();
}

void StageObject::Update()
{
	if (!m_isActive)return;

	// モデルが存在するなら更新する。
	if (m_stageModel != nullptr)
	{
		m_stageModel->Update();
	}

	// 派生クラス固有の更新。
	OnUpdate();
}

void StageObject::Draw(RenderContext& rc)
{
	if (!m_isActive)return;

	// モデルが存在するなら描画する。
	if (m_stageModel != nullptr)
	{
		m_stageModel->Draw(rc);
	}

	// 必要なら派生クラス固有の描画。
	OnDraw(rc);
}

bool StageObject::PlayAnimation(const std::string& name, float interpolateTime)
{

	//そもそもモデルがないなら再生できない。
	if (m_stageModel == nullptr)return false;


	return m_stageModel->PlayAnimation(name, interpolateTime);
}
