#pragma once
#include "Application.h"
	
using namespace Auto3D;
class StencilTestSpace : public Application
{
	REGISTER_SPACE_CLASS(StencilTestSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};	
