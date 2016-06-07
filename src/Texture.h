#pragma once
// https://github.com/johnsietsma/RefEngine-Old/blob/master/RefEngine/graphics/Texture.h
// https://github.com/johnsietsma/RefEngine/blob/f46a001e7194b57ac7777af8b56a4809366025b4/Engine/src/graphics/Texture.cpp

/// TODO: texture class that FBX class has a copy of
#include "Mesh.h" //TODO: this shouldn't be here?
#include "gl_core_4_4.h"

#include <map>

class Camera;
class Mesh;

class Texture
{
public:
	const static Texture Invalid;

	// Construct an invalid texture, call create later.
	Texture() : Texture(-1) {}

	// Takes ownership of the texture data, should be able to call destroy on any Texture object.
	Texture(GLuint a_textureID) : m_textureID(a_textureID) {}

	~Texture() = default;

	bool Create();
	// Destroys a Texture
	GLvoid Destroy();

	GLuint TextureInit(const GLchar* name);
	GLvoid GenTexture(const GLchar* a_path, GLuint a_TextureType);

	GLvoid CreateBuffers();			//RenderTexture();
	GLvoid CreateTextureShader();	//GLvoid TextureLoader();
	GLvoid Draw(const Camera& m_pCamState);
	GLvoid DrawTexture(const Camera& m_pCamState, GLuint a_uiTexture1, GLuint a_uiTexture2);
	//GLvoid DrawTextureP(Camera* cam);
	GLvoid AddTexture(const GLchar* name, const GLuint id);

	GLuint GetTextureByName(const GLchar* a_name);
	bool DoesTextureNameExist(const GLchar* a_name);
	GLuint GetId() const { return m_textureID; }

	GLvoid SetId(GLuint a_textureId) { m_textureID = a_textureId; }

private:

	GLuint m_textureID; // m_programID;

	// Store the OpenGL texture.
	std::map<const std::string, const GLuint> m_textures, m_normalmap;

	Mesh m_mesh;
};