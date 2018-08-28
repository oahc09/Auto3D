#pragma once
#include "Shader.h"
#include "Transform.h"
#include "Application.h"
#include "Math/Color.h"
#include "Material.h"
#include "RenderComponent.h"
#include "GLMeshEnable.h"
#include "Model.h"


namespace Auto3D {

class Mesh : public RenderComponent,public GLMeshEnable
{
	REGISTER_DERIVED_CLASS(Mesh, RenderComponent);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	explicit Mesh(Ambient* ambient);
	Mesh(Ambient* ambient,char* meshPath);
	Mesh(Ambient* ambient,char* meshPath, const Shader& shader);

	void Start()override;
	void Draw()override;
	Material* GetMaterial() { return _material; }

private:
	void drawMaterial();
	void drawLight();
private:
	Shader _shader;
	SharedPtr<Material> _material;
	SharedPtr<Model> _model;
	char* _meshPath;
private:
	bool _isUserShader;
};

}

