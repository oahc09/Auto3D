#pragma once
#include "Object.h"
#include "GameObject.h"
AUTO_BEGIN
class Prefab : public Object
{
	REGISTER_DERIVED_CLASS(Prefab, Object);
	DECLARE_OBJECT_SERIALIZE(Prefab);
public:
	Prefab(GameObject* gameObject);
	Prefab(GameObject& gameObject);
private:
	GameObject* _gameObject;
};

AUTO_END