#pragma once
#include "RenderComponent.h"
#include "_Shader.h"
namespace Auto3D {

class Bloom : public RenderComponent
{
public:
	explicit Bloom(Ambient* ambient);
	~Bloom();
	void Start()override;
	void Draw()override;
private:
	_Shader m_shader;
	_Shader m_shaderLight;
	_Shader m_shaderBlur;
	_Shader m_shaderBloomFinal;

	unsigned int woodTexture;
	unsigned int containerTexture;

	unsigned int hdrFBO;
	unsigned int colorBuffers[2];
	unsigned int rboDepth;
	
	
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	bool bloom = true;
	bool bloomKeyPressed = false;
	float exposure = 1.0f;
	unsigned int cubeVAO{};
	unsigned int cubeVBO{};
	// -----------------------------------------
	unsigned int quadVAO{};
	unsigned int quadVBO{};


};

}
