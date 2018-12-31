#pragma once
#include "Auto.h"
#include "ClassID.h"
#include "RTTI.h"
#include "ObjectDef.h"
#include "MemoryDef.h"
#include "AssertDef.h"

namespace Auto3D {

class Ambient;

/**
* Base object for all regisiter class
*/
class Object
{	
public:
	explicit Object(SharedPtr<Ambient> ambient);
	virtual ~Object() = default;
	///////////////////////////////////////////////////////////////////////////
	// @brief : Register class function
	virtual int GetClassID() { return ClassID(Object); }
	virtual bool IsAbstract() { return true; }
	virtual char* GetClassCstrName() { return "Object"; }
	virtual STRING GetClassString() { return "Object"; }
	virtual Auto3D::RTTI* GetRTTI() { return GetRTTIStatic(); }
	static int GetClassIDStatic() { return ClassID(Object); }
	static bool IsAbstractStatic() { return true; }
	static char* GetClassCstrNameStatic() { return "Object"; }
	static STRING GetClassStringStatic() { return "Object"; }
	static Auto3D::RTTI* GetRTTIStatic() { return nullptr; }
	//////////////////////////////////////////////////////////////////////////
	
	/**
	* @brief : Return execution ambient.
	*/
	SharedPtr<Ambient> GetAmbient() const { return _ambient; }
	/**
	* @brief : Return subsystem by type.
	*/
	SharedPtr<Object> GetSubSystem(STRING type)const;
	/**
	* @brief : Template version of returning a subsystem.
	*/
	template<typename _Ty> SharedPtr<_Ty> GetSubSystem() const;
	/**
	* @brief : Get object attach
	*/
	const STRING GetAttach();


protected:
	SharedPtr<Ambient> _ambient;
};

template<typename _Ty> 
SharedPtr<_Ty> Object::GetSubSystem() const
{ 
	return StaticCast<_Ty>(Object::GetSubSystem(_Ty::GetClassStringStatic()));
}

class ObjectFactory
{
public:
	explicit ObjectFactory(SharedPtr<Ambient> ambient)
		:_ambient(ambient)
	{
		Assert(_ambient);
	}
	/**
	* @brief : Create an object.Implemented in templated subclasses.
	*/
	virtual SharedPtr<Object> CreateObject() = 0;
	/**
	* @brief : Return execution ambient
	*/
	SharedPtr<Ambient> GetAmbient() const { return _ambient; }
	/**
	* @brief : Retrun template Class Name for c str
	*/
	const char* GetClassCstrName() { return _RTTI->GetClassCstrName(); }
	/**
	* @brief : Retrun template Class Name for string
	*/
	const STRING GetClassString() { return _RTTI->GetClassString(); }
	/**
	* @brief : Retrun template Class RTTI
	*/
	const Auto3D::RTTI* GetRTTI() { return _RTTI; }
protected:
	///Execution ambient
	SharedPtr<Ambient> _ambient;
	///RTTI info
	RTTI* _RTTI{};
};

template<typename _Ty> class ObjectFactoryImpl : public ObjectFactory
{
public:
	/// Construct
	explicit ObjectFactoryImpl(SharedPtr<Ambient> ambient) :
		ObjectFactory(ambient)
	{
		_RTTI = _Ty::GetRTTIStatic();
	}
	/**
	* @brief : Create an object of the specific type.
	*/
	SharedPtr<Object> CreateObject() override { return StaticCast<Object>(MakeShared<_Ty>(_ambient)); }
};

}
