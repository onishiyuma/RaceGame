
#include "stdafx.h"
#include "PlayerController.h"
#include <algorithm>

PlayerController::PlayerController(GamePad& gamePad)
	: m_gamePad(gamePad)
{}

CarInput PlayerController::GetInput(const Car& car)
{
	(void)car;

	CarInput input{};

	// 左スティックでハンドルを操作する。
	input.steering = m_gamePad.GetLStickXF();
	input.vertical = m_gamePad.GetLStickYF();

	// アクセル・ブレーキを取得する。
	input.accelerator =
		m_gamePad.IsPress(enButtonB) ? 1.0f : 0.0f;

	input.brake =
		m_gamePad.IsPress(enButtonA) ? 1.0f : 0.0f;

	// R / ZRでミニジャンプ・ドリフトを行う。
	input.hopDrift =
		m_gamePad.IsPress(enButtonRB1) ||
		m_gamePad.IsPress(enButtonRB2);

	// L / ZLでアイテムを操作する。
	input.useItem =
		m_gamePad.IsPress(enButtonLB1) ||
		m_gamePad.IsPress(enButtonLB2);

	return input;
}
