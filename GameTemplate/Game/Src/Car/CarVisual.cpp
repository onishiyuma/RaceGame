#include "stdafx.h"
#include "CarVisual.h"
#include "ModelAssetDefinition.h"

bool CarVisual::Init(const ModelAssetDefinition& modelAsset)
{
	return m_modelInstance.Init(modelAsset);
}

void CarVisual::Update(
	const Vector3& position,
	const Quaternion& rotation)
{
	// 物理状態から車体モデルの姿勢を更新する。
	m_modelInstance.SetTRS(
		position,
		rotation,
		Vector3::One
	);

	m_modelInstance.Update();
}

void CarVisual::Render(RenderContext& renderContext)
{
	m_modelInstance.Draw(renderContext);
}
