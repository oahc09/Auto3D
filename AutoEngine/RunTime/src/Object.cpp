#include "Object.h"
#include "Ambient.h"
namespace Auto3D {

Object::Object(Ambient* ambient)
	: _ambient(ambient)
{
}

Object* Object::GetSubSystem(__String type) const
{
	return _ambient->GetSubSystem(type);
}

const __String Object::GetAttach() 
{
	AUTO_HASH_MAP(__String, _VECTOR(__String)) objectAttachs = _ambient->GetObjectAttachs();
	for (AUTO_HASH_MAP(__String, _VECTOR(__String))::ConstIterator i = objectAttachs.Begin(); i != objectAttachs.End(); ++i)
	{
		_VECTOR(__String) t = i->second;
		if(t.Find(GetClassString())!=t.End())
			return i->first;
	}
	// return null
	return "";
}

}

