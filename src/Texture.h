#pragma once
// https://github.com/johnsietsma/RefEngine/blob/f46a001e7194b57ac7777af8b56a4809366025b4/Engine/src/graphics/Texture.cpp
// https://github.com/richard-stern/SpaceCowboy/blob/6dd6520b484d4a31a814bc203ef3ab67d420cd70/AIE_GL/include/Texture.h
// https://github.com/johnsietsma/RefEngine-Old/blob/master/RefEngine/graphics/Texture.h
/// Represents a texture owned by OpenGL.
/// Holds Texture ID and the file path.
/// TODO: texture class that FBX class has a copy of
/// ***Edit***
/// Split into TextureMan and Text classes - David Azouz 8/06/16

#include "Mesh.h" //TODO: this shouldn't be here?
#include "gl_core_4_4.h"

#include <string>

class Camera;
class Mesh;

class Texture
{
public:
	const static Texture Invalid;

	// Construct an invalid texture, call create later.
	Texture() : Texture(USHRT_MAX) {} //-1
	
	// overloaded constructor
	// loads a texture given the filename...
	// program will crash if texture was unable to be found.
	Texture(const GLchar *filename);

	// lodepng
	//Texture(const GLchar *filename, GLuint *out_width, GLuint *out_height);

	// overloaded constructor
	// loads a texture given the filename...
	// program will crash if texture was unable to be found.
	Texture(GLenum a_format, GLuint width, GLuint height, GLuint a_textureID, GLenum a_components, GLenum a_type, const GLvoid* pixels);

	~Texture();

	Texture& operator=(Texture&&);

	// Returns m_textureID.
	GLuint GetId() const { return m_textureID; }
	const std::string& GetPath() const { return sPath; }

private:
	// Takes ownership of the texture data, should be able to call destroy on any Texture object.
	Texture(GLuint a_textureID) : m_textureID(a_textureID) {}

	Texture(const Texture&) = default;
	GLuint m_width;
	GLuint m_height;
	GLuint m_textureID; // m_programID;

	std::string sPath;
	// Store the OpenGL texture. Texture Manager.
	//std::map<std::string, const GLuint> m_textures, m_normalmap;
};