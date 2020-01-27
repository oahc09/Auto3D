#pragma once
#include "AutoConfig.h"
#include "Adapter/AutoRttr.h"
#include "Adapter/Ptr.h"

namespace Auto3D
{
#define DECLARE_RTTR_BASE_CLASS(_This) \
	/*The base Object does not need to specify a parent class*/\
	RTTR_ENABLE() \
	/*Reflected private tag*/\
	RTTR_REGISTRATION_FRIEND 

#define DECLARE_RTTR_CLASS(_This,_Base) \
	/*The base Object does not need to specify a parent class*/\
	RTTR_ENABLE(_Base) \
	/*Reflected private tag*/\
	RTTR_REGISTRATION_FRIEND 

#define DECLARE_CLASS(_This,_Base) \
public: \
	_This& operator=(_This&&) = delete;   \
	_This& operator=(const _This&)= delete;  \
	using This = _This;\
	using Super = _Base;\
	DECLARE_RTTR_CLASS(_This, _Base) \
private:\
	SPtr<_This> SPtrThis() {return std::dynamic_pointer_cast<_This>(shared_from_this());}\
	WPtr<_This> WPtrThis() { return std::dynamic_pointer_cast<_This>(shared_from_this()); }\
public:

#define REGISTER_CALSS_IMP(_Class) \
	using namespace rttr;\
	registration::class_<_Class>(#_Class)

/// Base class for objects.
class AUTO_API OObject : public IEnablePtrThis<OObject>
{
	DECLARE_RTTR_BASE_CLASS(OObject)
public:
	/// Structure
	OObject() = default;
	/// Destructor
	virtual ~OObject() = default;

	//FVariant _variant;
};

}
