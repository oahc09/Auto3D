#pragma once
#include "Core/GameManager.h"

namespace Auto3D {

/// AScript sub system 
class AUTO_API AScript : public ABaseModule
{
	REGISTER_OBJECT_CLASS(AScript, ABaseModule)
public:
	/// Construct
	AScript();

	~AScript();

};


}