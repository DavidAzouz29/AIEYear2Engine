#pragma once
// https://github.com/johnsietsma/RefEngine/blob/f46a001e7194b57ac7777af8b56a4809366025b4/Engine/src/graphics/Texture.cpp
// https://github.com/johnsietsma/RefEngine-Old/blob/master/RefEngine/graphics/Texture.h

/// TODO: texture class that FBX class has a copy of
#include "Mesh.h" //TODO: this shouldn't be here?
#include "gl_core_4_4.h"

#include <map>

class Camera;
class Mesh;

class Texture
{
public:
	// Construct an invalid texture, call create later.
	Texture() : Texture(-1) {}

	~Texture() = default;

	bool Create();

	GLuint TextureInit(const GLchar* name);
	GLvoid CreateBuffers();			//RenderTexture();
	GLvoid CreateTextureShader();	//GLvoid TextureLoader();
	GLvoid Draw(const Camera& m_pCamState);
	GLvoid DrawTexture(const Camera& m_pCamState, GLuint a_uiTexture1, GLuint a_uiTexture2);
	//GLvoid DrawTextureP(Camera* cam);
	GLvoid AddTexture(const GLchar* name, const GLuint id);

	GLuint GetTextureByName(const GLchar* name);
	GLuint GetId() const { return m_textureId; }

	GLvoid SetId(GLuint a_textureId) { m_textureId = a_textureId; }

private:

	GLuint m_textureId; // m_programID;

	// Store the OpenGL texture.
	std::map<const std::string, const GLuint> m_textures, m_normalmap;

	Mesh m_mesh;
};