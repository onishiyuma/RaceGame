#include"stdafx.h"
#include "StageObject.h"
#include"Stage/StageAssetDatabase.h"
#include "Stage/StageLoader/StageDefinition.h"

bool StageObject::Init(const ObjectDefinition& definition, const StageAssetDatabase& assetDatabase)
{
	m_position = definition.position;
	m_rotation = definition.rotation;
	m_scale = definition.scale;

	//アセットIDが入っていたらステージモデルを作る。
	if (!definition.assetId.empty())
	{
		const StageAssetDefinition* asset =
			assetDatabase.FindAssetDefinition(
				definition.assetId
			);

		if (asset == nullptr)return false;

		m_stageModel = std::make_unique<StageModel>();

		if (!m_stageModel->Init(*asset))return false;

		m_stageModel->SetTRS(
			m_position,
			m_rotation,
			m_scale
		);
	}

	return OnInit(definition.properties);
}

void StageObject::Update()
{
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
