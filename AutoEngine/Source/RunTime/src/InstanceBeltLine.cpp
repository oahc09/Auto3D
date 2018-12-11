#include "InstanceBeltLine.h"
#include "Mesh.h"
#include "Math/Rect.h"
#include "Graphics.h"
#include "Camera.h"
#include "Renderer.h"
#include "NewDef.h"

namespace Auto3D {

InstanceBeltLine::InstanceBeltLine(Ambient* ambient,Mesh* mesh, const Shader& shader, glm::mat4* modelMat,int count)
	:RenderComponent(ambient)
	,_mesh(mesh)
	,_shader(shader)
	,_modelMatrices(modelMat)
	,_count(count)
{
}
InstanceBeltLine::~InstanceBeltLine()
{
	UnloadOpaque(this);
}
void InstanceBeltLine::Start()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, _count * sizeof(glm::mat4), &_modelMatrices[0], GL_STATIC_DRAW);
	for (unsigned int i = 0; i < _mesh->GetMeshNodes().size(); i++)
	{
		unsigned int VAO = _mesh->GetMeshNodes()[i].vao;
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	RegisterOpaque(this);
}
void InstanceBeltLine::Draw()
{
	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();

	glm::mat4 projectionMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	glm::mat4 viewMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();

	_shader.Use();
	_shader.SetMat4("projection", projectionMat);
	_shader.SetMat4("view", viewMat);
	_shader.SetInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _mesh->GetTextureDatas()[0].data);
	for (unsigned int i = 0; i < _mesh->GetMeshNodes().size(); i++)
	{
		glBindVertexArray(_mesh->GetMeshNodes()[i].vao);
		glDrawElementsInstanced(GL_TRIANGLES, _mesh->GetMeshNodes()[i].indices.size(), GL_UNSIGNED_INT, 0, _count);
		glBindVertexArray(0);
	}

}
}