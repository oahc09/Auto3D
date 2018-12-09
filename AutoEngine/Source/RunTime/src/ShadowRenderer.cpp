#include "ShadowRenderer.h"
#include "Renderer.h"
#include "LightContainer.h"
#include "ResourceSystem.h"
#include "Configs.h"
#include "Light.h"
#include "Graphics.h"
#include "NewDef.h"



namespace Auto3D {

ShadowRenderer::ShadowRenderer(Ambient* ambient)
	: Super(ambient)
	, _shadowMapDepthShader(shader_path + "au_shadow_mapping_depth.auvs"
		, shader_path + "au_shadow_mapping_depth.aufs")
	, _shadowMapPointDepth(shader_path + "au_point_shadows_depth.auvs"
		, shader_path + "au_point_shadows_depth.aufs"
		, shader_path + "au_point_shadows_depth.augs")
{
}
ShadowRenderer::~ShadowRenderer()
{}
void ShadowRenderer::ReadyRender()
{
	auto* renderer = GetSubSystem<Renderer>();
	_lights = renderer->_lightContainer->GetAllLights();
	_shadowComponents = renderer->GetAllShadowMaps();
	renderer->_lightContainer->IsRender(true);
	//!!! Temp use one
#pragma warning
	for (VECTOR<Light*>::iterator it = _lights.begin(); it != _lights.end(); it++)
	{
		renderer->_lightContainer->SetCurrentLight(*it);
		//!!!
#pragma warning
		(*it)->GetShadowAssist()->BindDepathMap();
		/////////////////////////////////////////////////////////////////////////////////////////////
		for (LIST<RenderComponent*>::iterator it = _shadowComponents.begin(); it != _shadowComponents.end(); it++)
		{
			(*it)->DrawReady();
		}
#pragma warning
		_woodTexture = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/wood.jpg");
	}
	renderer->_lightContainer->IsRender(false);
}
void ShadowRenderer::RenderShadow()
{
	//!!! Temp use one
#pragma warning
	for (VECTOR<Light*>::iterator it = _lights.begin(); it != _lights.end(); it++)
	{
		if ((*it)->GetType() == LightType::Directional)
		{
			glm::mat4 lightSpaceMatrix = (*it)->GetLightSpaceMatrix();
			glViewport(0, 0, (*it)->GetShadowAssist()->GetShadowWidth(), (*it)->GetShadowAssist()->GetShadowHeight());
			glBindFramebuffer(GL_FRAMEBUFFER, (*it)->GetShadowAssist()->GetDepthMapFBO());
			glClear(GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _woodTexture);

			_shadowMapDepthShader.Use();
			_shadowMapDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
			//Ergodic shadows to Draw shadow
			for (LIST<RenderComponent*>::iterator it = _shadowComponents.begin(); it != _shadowComponents.end(); it++)
			{
				(*it)->DrawShadow();
			}
		}
		else if ((*it)->GetType() == LightType::Point || (*it)->GetType() == LightType::Spot)
		{

			int shadowWidth = (*it)->GetShadowAssist()->GetShadowWidth();
			int shadowHeight = (*it)->GetShadowAssist()->GetShadowHeight();
			glm::vec3 lightPos = (*it)->GetLightPosition();

			glm::mat4 shadowProj = glm::perspective(90.0f, (float)shadowWidth / (float)shadowHeight, (*it)->GetNearPlane(), (*it)->GetFarPlane());
			std::vector<glm::mat4> shadowTransforms;
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			glViewport(0, 0, shadowWidth, shadowHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, (*it)->GetShadowAssist()->GetDepthMapFBO());
			glClear(GL_DEPTH_BUFFER_BIT);
			_shadowMapPointDepth.Use();
			for (unsigned int i = 0; i < 6; ++i)
				_shadowMapPointDepth.SetMat4("shadowMatrices[" + KhSTL::ToString(i) + "]", shadowTransforms[i]);
			_shadowMapPointDepth.SetFloat("far_plane", (*it)->GetFarPlane());
			_shadowMapPointDepth.SetVec3("lightPos", lightPos);
			//Ergodic shadows to Draw shadow
			for (LIST<RenderComponent*>::iterator it = _shadowComponents.begin(); it != _shadowComponents.end(); it++)
			{
				(*it)->DrawShadow();
			}
		}
		else
		{
			ErrorString("Fail render depth map");
			continue;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}



}