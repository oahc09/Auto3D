#include "Launch.h"
#include "MultLightsSpace.h"
#include "TextureSpace.h"
#include "MoreCameraSpace.h"
#include "DepthTestSpace.h"
USING_AUTO


#ifdef __OPENGL__
int main()
{


	//TextureSpace work;
	//MultLightsSpace work;
	//MoreCameraSpace work;
	DepthTestSpace work;
	return work.Launch();
}

#endif //__OPENGL__