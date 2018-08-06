#pragma once
#include "MotionSpace.h"
	
using namespace Auto3D;
class StencilTestSpace : public MotionSpace
{
public:
	explicit StencilTestSpace(Ambient* ambient);
	~StencilTestSpace();
	void Start()override;
	void Update()override;
	int Launch();
};	