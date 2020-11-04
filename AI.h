#pragma once

#include "Pitch.h"

class AI
{
protected:
	Pitch& pitch;

public:
	AI(Pitch& p) : pitch(p) {}

	virtual void MakeOneMove() = 0;
};