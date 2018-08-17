#include "LightDirectional.h"


namespace Auto3D {
LightDirectional::LightDirectional(Ambient* ambi)
	: Super(ambi, LightType::kDirectional)
{
	_color.Set(1.0f, 1.0f, 1.0f);
	direction.Set(-0.5f, -0.5f, -0.0f);
	ambient.Set(0.3f, 0.3f, 0.3f);
	diffuse.Set(1.0f, 1.0f, 1.0f);
	specular.Set(0.7f, 0.7f, 0.7f);
}


LightDirectional::~LightDirectional()
{
}
}