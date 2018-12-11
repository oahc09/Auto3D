#include "AudioBuffer.h"
#include "NewDef.h"

namespace Auto3D {

AudioBuffer::AudioBuffer(Ambient* ambient, STRING path)
	: Super(ambient)
	, _data(path)
{
	Print(GetData());
}

AudioBuffer::~AudioBuffer()
{
}

}