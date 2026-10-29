#pragma once
#include "stdafx.h"

/// <summary>
/// 車の現在の物理状態
/// </summary>
struct CarPhysicsState
{
	//TODO:まだ仮。
	Vector3 position = Vector3::Zero;
	Vector3 velocity = Vector3::Zero;

	Quaternion rotation = Quaternion::Identity;

	bool isGrounded = false;
	Vector3 groundNormal = Vector3::Up;
};