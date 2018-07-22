#include "PBRTexture.h"
#include "AtConfig.h"
#include "Camera.h"
#include "LoadResource.h"
#include "BaseMesh.h"
#include "RenderManager.h"
AUTO_BEGIN
glm::vec3 t_lightPositions[] = {
	glm::vec3(-10.0f,  10.0f, 10.0f),
	glm::vec3(10.0f,  10.0f, 10.0f),
	glm::vec3(-10.0f, -10.0f, 10.0f),
	glm::vec3(10.0f, -10.0f, 10.0f),
};
glm::vec3 t_lightColors[] = {
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f)
};

PBRTexture::PBRTexture()
	:m_shader(AtConfig::shader_path + "au_pbr.auvs"
		, AtConfig::shader_path + "au_pbr_texture.aufs")
{
}


PBRTexture::~PBRTexture()
{
}
void PBRTexture::Start()
{
	m_shader.Use();
	m_shader.SetInt("albedoMap", 0);
	m_shader.SetInt("normalMap", 1);
	m_shader.SetInt("metallicMap", 2);
	m_shader.SetInt("roughnessMap", 3);
	m_shader.SetInt("aoMap", 4);
	albedo = LocalTextureLoad("resource/texture/pbr/gold/albedo.png");
	normal = LocalTextureLoad("resource/texture/pbr/gold/normal.png");
	metallic = LocalTextureLoad("resource/texture/pbr/gold/metallic.png");
	roughness = LocalTextureLoad("resource/texture/pbr/gold/roughness.png");
	ao = LocalTextureLoad("resource/texture/pbr/gold/ao.png");

}
void PBRTexture::Draw()
{
	glm::mat4 projection = INSTANCE(RenderManager).GetCurrentCamera().GetProjectionMatrix();
	m_shader.Use();
	m_shader.SetMat4("projection", projection);
	glm::mat4 view = INSTANCE(RenderManager).GetCurrentCamera().GetViewMatrix();
	m_shader.SetMat4("view", view);
	m_shader.SetVec3("camPos", INSTANCE(RenderManager).GetCurrentCamera().GetPosition());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metallic);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, roughness);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ao);

	glm::mat4 model;

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	for (unsigned int row = 0; row < nrRows; ++row)
	{
		for (unsigned int col = 0; col < nrColumns; ++col)
		{
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(
				(float)(col - (nrColumns / 2)) * spacing,
				(float)(row - (nrRows / 2)) * spacing,
				0.0f
			));
			m_shader.SetMat4("model", model);
			renderSphere(&sphereVAO,&indexCount);
		}
	}

	// render light source (simply re-render sphere at light positions)
	// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
	// keeps the codeprint small.
	for (unsigned int i = 0; i < sizeof(t_lightPositions) / sizeof(t_lightPositions[0]); ++i)
	{
		glm::vec3 newPos = t_lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = t_lightPositions[i];
		m_shader.SetVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		m_shader.SetVec3("lightColors[" + std::to_string(i) + "]", t_lightColors[i]);

		model = glm::mat4();
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		m_shader.SetMat4("model", model);
		renderSphere(&sphereVAO, &indexCount);
	}
}
AUTO_END