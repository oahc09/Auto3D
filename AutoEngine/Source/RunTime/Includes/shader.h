#pragma once
#include "Auto.h"
#include "AutoOGL.h"
#include "ShaderVariation.h"
#include "Resource.h"

namespace Auto3D {

class Shader : public Resource
{
	REGISTER_DERIVED_CLASS(Shader, Resource);
	DECLARE_OBJECT_SERIALIZE(Shader)
public:
	/**
	* @brief : Constructor
	*/
	explicit Shader(Ambient* ambient);
	/**
	* @brief : Register object factory
	*/
	static void RegisterObject(Ambient* ambient);
	/*
	*@brief : Load resource from stream.May be called from a worker thread.Return true if successful
	*/
	bool BeginLoad(Deserializer& source)override;
private:
	/**
	* @brief : Process source code and include files. Return true if successful
	*/
	bool processSource(STRING& code, Deserializer& source);
public:
	/// source code adapted for vertex shader.
	STRING _vsSourceCode;
	/// source code adapted for fragment shader.
	STRING _fsSourceCode;
	/// source code adapted for geometry shader
	STRING _gsSourceCode;
	/// Vertex shader variations.
	HASH_MAP<STRING, SharedPtr<ShaderVariation> > _vsVariations;
	/// Fragment shader variations.
	HASH_MAP<STRING, SharedPtr<ShaderVariation> > _fsVariations;
	/// Geometry shader variations.
	HASH_MAP<STRING, SharedPtr<ShaderVariation> > _gsVariations;


	/// Source code timestamp.
	unsigned _timeStamp;
	/// number of unique variations so far.
	unsigned _numVariations;
};
}
