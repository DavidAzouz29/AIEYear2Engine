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

#include <map>

class Camera;
class Mesh;

class Texture
{
public:
	const static Texture Invalid;

	// Construct an invalid texture, call create later.
	Texture() : Texture(-1) {}
	
	//Texture() = default;

	// overloaded constructor
	// loads a texture given the filename...
	// program will crash if texture was unable to be found.
	Texture(const GLchar *filename);

	// overloaded constructor
	// loads a texture given the filename...
	// program will crash if texture was unable to be found.
	Texture(GLuint width, GLuint height, GLuint *pixels = 0);

	// Takes ownership of the texture data, should be able to call destroy on any Texture object.
	Texture(GLuint a_textureID) : m_textureID(a_textureID) {}

	~Texture();

	bool Create();
	// Destroys a Texture
	//GLvoid Destroy();

	GLuint TextureInit(const GLchar* name);
	GLvoid GenTexture(const GLchar* a_path, GLuint a_TextureType);

	GLuint LoadTexture(const GLchar *filename, GLuint *out_width, GLuint *out_height);

	GLvoid CreateBuffers();			//RenderTexture();
	GLvoid CreateTextureShader();	//GLvoid TextureLoader();
	GLvoid Draw(const Camera& m_pCamState);
	template<typename T>
	GLvoid DrawTexture(const Camera& m_pCamState, T a_uiTexture1, T a_uiTexture2);
	//GLvoid DrawTextureP(Camera* cam);
	//GLvoid AddTexture(const GLchar* name, const GLuint id);

	//GLuint GetTextureByName(const GLchar* a_name);
	//bool DoesTextureNameExist(const GLchar* a_name);

	// Returns m_textureID.
	GLuint GetId() const { return m_textureID; }

	GLvoid SetId(GLuint a_textureId) { m_textureID = a_textureId; }

private:

	GLuint m_width;
	GLuint m_height;
	GLuint m_textureID; // m_programID;

	// Store the OpenGL texture. Texture Manager.
	std::map<std::string, const GLuint> m_textures, m_normalmap;

	static GLvoid DeleteTexture(GLuint a_textureID);

	Mesh m_mesh; //TODO: remove this

	// Template used if a Texture is of type:
	// 'GLuint'.
	// 'Texture' //TOOD: are some of these needed?
	// std::shared_ptr<Texture>
	template<typename T>
	GLvoid SetupTextureSlots(T a_uiTexture1, T a_uiTexture2) { assert(false); }

	template<> //GLvoid DrawTexture(const Camera& m_pCamState, GLuint a_uiTexture1, GLuint a_uiTexture2)
	GLvoid SetupTextureSlots<GLuint>(GLuint a_uiTexture1, GLuint a_uiTexture2)
	{
		// Set texture slots
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, a_uiTexture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, a_uiTexture2);
	}

	template<> //GLvoid DrawTexture(const Camera& m_pCamState, Texture a_uiTexture1, Texture a_uiTexture2)
	GLvoid SetupTextureSlots<Texture>(Texture a_uiTexture1, Texture a_uiTexture2)
	{
		// Set texture slots
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, a_uiTexture1.GetId());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, a_uiTexture2.GetId());
	}

	template<> //Shared 
	GLvoid SetupTextureSlots<std::shared_ptr<Texture>>(std::shared_ptr<Texture> a_uiTexture1, std::shared_ptr<Texture> a_uiTexture2)
	{
		// Set texture slots
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, a_uiTexture1.get()->GetId());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, a_uiTexture2.get()->GetId());
	}
};

template<typename T>
GLvoid DrawTexture(const Camera& m_pCamState, T a_uiTexture1, T a_uiTexture2)
{
	// use our texture program
	glUseProgram(m_textureID);

	// bind the camera
	GLint loc = glGetUniformLocation(m_textureID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_pCamState.getProjectionView()[0][0]));

	/* unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	// unsigned int uiTime = glGetUniformLocation(m_programID, "time");
	//
	// glUniform1f(uiHeightScale, fHeightScale);
	// glUniform1f(uiTime, fTime);

	//glBindTexture(GL_TEXTURE_2D, m_textures["crate"]); */

	SetupTextureSlots();
	
	//glBindTexture(GL_TEXTURE_2D, m_normalmap["soulspear_n"]);

	// tell the shader where it is
	loc = glGetUniformLocation(m_textureID, "diffuse");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(m_textureID, "normal");
	glUniform1i(loc, 1);

	// bind the light
	glm::vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_textureID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	// draw
	glBindVertexArray(m_mesh.GetVAO()); //TODO: replace m_VAO with VAO
										//glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}