#pragma once
#include "Auto.h"
#include "Shader.h"
#include "GameObject.h"
#include "MeshShadow.h"
namespace Auto3D {

class Shadow : public Component
{
public:
	explicit Shadow(Ambient* ambient);
	virtual ~Shadow();
	void Start()override;
	void Draw()override;
private:

	Shader m_ShadowMapDepth;
	glm::vec3 lightPos;
	unsigned int woodTexture;
	unsigned int planeVAO;

	MeshShadow* mesh;
	MeshShadow* mesh1;
	MeshShadow* mesh2;
	MeshShadow* mesh3;
};

}


