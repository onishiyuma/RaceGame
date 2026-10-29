#pragma once

/// <summary>
/// 車の入力
/// </summary>
struct CarInput
{
	float accelerator = 0.0f; // アクセル量（0.0～1.0）
	float brake = 0.0f;       // ブレーキ量（0.0～1.0）
	float steering = 0.0f;    // ハンドル操作量（-1.0～1.0）
	float vertical = 0.0f;    // スティックの上下入力（-1.0～1.0）
	bool hopDrift = false;    // ミニジャンプ・ドリフト操作
	bool useItem = false;     // アイテム操作
};