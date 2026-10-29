#pragma once
#include "ModelInstance.h"

struct ModelAssetDefinition;

/// <summary>
/// 車の見た目を管理するクラス。
/// 車の物理状態に応じてモデルの姿勢を更新し、描画する。
/// TODO:将来的にステッカー機能での見た目の処理なんかもやる。
/// </summary>
class CarVisual
{
public:
	/// <summary>
	/// ロード済みリソースを使用して車のモデルを初期化する。
	/// </summary>
	bool Init(const ModelAssetDefinition& modelAsset);

	/// <summary>
	/// 車の位置と回転をモデルに反映する。
	/// </summary>
	void Update(
		const Vector3& position,
		const Quaternion& rotation);

	/// <summary>
	/// 車のモデルを描画する。
	/// </summary>
	void Render(RenderContext& renderContext);

private:
	ModelInstance m_modelInstance;
};
