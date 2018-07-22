#pragma once
#include "MotionSpace.h"
USING_AUTO

class MultLightsSpace : public MotionSpace
{
public:
	MultLightsSpace();
	~MultLightsSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
