#include "HDRSpace.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"

AUTO_APPLICATION_MAIN(HDRSpace)

void HDRSpace::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void HDRSpace::Destruct()
{
	RemoveScene(0);
}