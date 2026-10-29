#pragma once
#include"Src/Car/CarInput.h"

class Car;
class ICarController
{
public:
	virtual ~ICarController() = default;

	virtual CarInput GetInput(const Car& car) = 0;
};