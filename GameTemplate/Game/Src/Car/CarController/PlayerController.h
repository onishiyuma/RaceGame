
#pragma once
#include "ICarController.h"

class PlayerController : public ICarController
{
public:
	/// <summary>
	/// 操作に使用するゲームパッドを指定する。
	/// </summary>
	explicit PlayerController(GamePad& gamePad);

	/// <summary>
	/// プレイヤーの操作入力を取得する。
	/// </summary>
	CarInput GetInput(const Car& car) override;

private:
	GamePad& m_gamePad;
};
