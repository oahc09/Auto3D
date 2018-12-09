#include "SpriteTranslucent.h"
#include "Renderer.h"
#include "VertexData.h"
#include "Transform.h"
#include "AutoOGL.h"
#include "Configs.h"
#include "ResourceSystem.h"
#include "NewDef.h"

namespace Auto3D {

SpriteTranslucent::SpriteTranslucent(Ambient* ambient, char* imagePath)
	: Texture2D(ambient)
	, _shader(Shader(shader_path + "au_texture_transform.auvs"
		, shader_path + "au_texture_transform.aufs"))
{
	_imagePath = imagePath;
}
/*
SpriteTranslucent::SpriteTranslucent(char* imagePath, const Shader & shader)
	: _shader(shader)
{
	_imagePath = imagePath;
}*/
SpriteTranslucent::~SpriteTranslucent()
{
	UnloadTranslucent(this);
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
}


void SpriteTranslucent::Start()
{
	/////////////////////////////////////////////////////////////////////////////////////////////

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	/////////////////////////////////////////////////////////////////////////////////////////////

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_vertices), texture_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(texture_vertices), texture_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	EnableVertexAttribs(VERTEX_ATTRIB_POSITION | VERTEX_ATTRIB_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/////////////////////////////////////////////////////////////////////////////////////////////

	glGenTextures(1, &_textureData);
	glBindTexture(GL_TEXTURE_2D, _textureData);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//_image = LocalImageLoad(_imagePath);

	_timage = GetSubSystem<ResourceSystem>()->ImageLoad(_imagePath);

	//SetNearestParameters();
	SetLinerParameters();
	if (_timage->value)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, _timage->format, _timage->width, _timage->height, 0, _timage->format, GL_UNSIGNED_BYTE, _timage->value);
		GenerateMipmap();
	}
	else
	{
		WarningString("Failed to load texture");
	}

	//GetSubSystem<Renderer>()->AddTranslucentGeometry(this);

	RegisterTranslucent(this);
	//stbi_image_free(m_image->Value);

	/////////////////////////////////////////////////////////////////////////////////////////////
}


void SpriteTranslucent::DrawTranslucent()
{
	GLApply();
	glBindTexture(GL_TEXTURE_2D, _textureData);
	_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;

	if (GetNodePtr())
		modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	projectionMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();

	_shader.SetMat4("model", modelMat);
	_shader.SetMat4("view", viewMat);
	_shader.SetMat4("projection", projectionMat);
	_shader.SetVec4("ourColor", _color.r, _color.g, _color.b, _color.a);

	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	GLOriginal();
}
void SpriteTranslucent::SetColor(const Color& color)
{
	_color.Set(color.r, color.g, color.b, color.a);
}

void SpriteTranslucent::SetColor(const Vector3& vec)
{
	_color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void SpriteTranslucent::SetColor(float r, float g, float b, float a)
{
	//SpriteTranslucent.Set(r, g, b, a);
}

//////////////////////////////////////////////////////////////////////////
//tImage conpontent to use
void SpriteTranslucent::SetLinerParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _timage->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _timage->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	if (_isMipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void SpriteTranslucent::SetNearestParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _timage->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _timage->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	if (_isMipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void SpriteTranslucent::SetTexParameters(const TexParams & params)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
}

void SpriteTranslucent::GenerateMipmap()
{
	glGenerateMipmap(GL_TEXTURE_2D);
	_isMipmaps = true;
}
}