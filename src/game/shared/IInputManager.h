#pragma once

#include "Key.h"
#include "Point.h"

class IInputManager
{
public:
	virtual Point SampleMousePosition() = 0;
	virtual short int SampleInput() = 0;
};