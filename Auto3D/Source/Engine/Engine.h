#pragma once
#include "AutoConfig.h"
#include "Platform//PlatformDef.h"

namespace Auto3D
{

class AUTO_API FEngine
{
public:
	/// Construct
	FEngine();
	/// Cestructor
	~FEngine();
	/// Init engine, Register some SubSystem
	bool Init();
	/// AEngine normal exit
	void Exit();
	/// Render geometry
	void Render();
	/// Sub system Update data,  If pause when _minimized -mode return false
	bool Update();
	/// Frame finish
	void FrameFinish();
};


}