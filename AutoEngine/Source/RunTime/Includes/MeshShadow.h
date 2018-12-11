#pragma once
#include "_Shader.h"
#include "GLMeshEnable.h"
#include "RenderComponent.h"

namespace Auto3D {

class Mesh;
class Camera;
class MeshShadow : public RenderComponent,public GLMeshEnable
{
public:
	explicit MeshShadow(Ambient* ambient);
	~MeshShadow();
	void DrawReady()override;
	void Draw()override;
	void DrawShadow()override;

	void SetMesh(Mesh* mesh) { _mesh.reset(mesh); }
private:
	_Shader _shader;
	SharedPtr<Mesh> _mesh;
	unsigned int _woodTexture;
	unsigned int _VAO;
	unsigned int _VBO;

};

}
