/// <summary>
/// File Name:		Renderable.h
/// Author: 		David Azouz
/// Date Created: 	10/05/16
/// Date Modified: 	24/05/16
///----------------------------------------------------------
/// Brief: An Abstract Renderable Class defining the interface all renderable objects must implement.
/// Enables more than one object to share the same mesh or texture etc.
/// 
/// viewed: http://www.dofactory.com/net/factory-method-design-pattern
/// http://www.ogre3d.org/docs/api/1.9/class_ogre_1_1_renderable.html#details
/// https://github.com/johnsietsma/RefEngine/blob/f46a001e7194b57ac7777af8b56a4809366025b4/Engine/src/graphics/Renderable.h
/// Batching https://www.opengl.org/sdk/docs/man/html/glDrawElementsBaseVertex.xhtml
/// Batching https://www.opengl.org/sdk/docs/man/html/glDrawElementsInstancedBaseVertex.xhtml
/// https://www.khronos.org/opengles/sdk/docs/man3/html/glDrawElementsInstanced.xhtml
/// https://www.khronos.org/opengles/sdk/docs/man3/html/glVertexAttribDivisor.xhtml
/// https://www.khronos.org/opengles/sdk/docs/man3/html/glEnableVertexAttribArray.xhtml
/// http://learnopengl.com/#!Advanced-OpenGL/Instancing
/// ***EDIT***
/// - Renderable created	 	- David Azouz 10/05/16
/// - .cpp removed, ref to Sampler added  	- David Azouz 24/05/16
/// -  	- David Azouz 6/05/16
/// 
/// TODO: 
/// Reff to Mesh
/// Ref to Textures
/// ref to mesh = Mesh* - maybe? [] - hold onto different submeshes - FBX models
///	ref to textures[]
///	ref to shader prog(which is) m_programID
/// </summary>
/// ----------------------------------------------------------
#pragma once

//#include "Helpers.h"

#include "gl_core_4_4.h"
#include "Mesh.h"
#include "Sampler.h"
#include "TextureManager.h"

#include <memory>
#include <vector>

struct Renderable
{
	Renderable() = default;
	//Renderable(const Program& a_program, const Mesh& a_mesh) :
	Renderable(const GLuint& a_program_ID, const Mesh& a_mesh);

	//Renderable(const Program& a_program, const Mesh& a_mesh, const std::vector<Sampler> a_samplers) :
	Renderable(const GLuint& a_program_ID, const Mesh& a_mesh, const std::vector<Sampler>& a_samplers);

	GLvoid Draw(const Camera& a_pCamState);
	template<typename T>
	GLvoid DrawTexture(const Camera& m_pCamState, T a_uiTexture1, T a_uiTexture2);

	std::shared_ptr<Texture> GetTextureByPath(const GLchar* a_path);
	GLuint program_ID; //Program program;
	Mesh mesh;
	std::vector<Sampler> samplers;
	GLenum renderMode = GL_FILL;

	//TODO: Batching Details
	/*ref to mesh = Mesh* - maybe? [] - hold onto different submeshes - FBX models
	ref to textures[]
	ref to shader prog (which is) m_programID

	// TODO: which? const GLuint& m_program_ID;

	// TODO: replace all my draw calls with one of these?
	// Draw multiple instances of a submesh in one call
	// glDrawElementsBaseVertex();

	// Allows drawing of 'N' instances of a SUBSET of indexed vertices in one call.
	// pass in the visible instances.
	//glDrawElementsInstancedBaseVertex();

	// Buckets - draw items that are the same - sort into buckets
	//TODO: Bucket sorter - a bucket is a batch
	// a batch is whatever we can squeeze into a single draw call (a batch is a draw call)
	//GPU-based instancing */

	//---------------------------------------------
	// Template used if a Texture is of type:
	// 'GLuint'.
	// 'Texture' //TOOD: are some of these needed?
	// std::shared_ptr<Texture>
	//---------------------------------------------
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
GLvoid DrawTexture(const Camera& a_pCamState, T a_uiTexture1, T a_uiTexture2)
{
	// use our texture program
	glUseProgram(m_textureID);

	// bind the camera
	GLint loc = glGetUniformLocation(m_textureID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(a_pCamState.getProjectionView()[0][0]));

	/* unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	// unsigned int uiTime = glGetUniformLocation(m_programID, "time");
	//
	// glUniform1f(uiHeightScale, fHeightScale);
	// glUniform1f(uiTime, fTime);

	//glBindTexture(GL_TEXTURE_2D, m_textures["crate"]); */

	SetupTextureSlots();

	//glBindTexture(GL_TEXTURE_2D, m_normalmap["soulspear_n"]);

	// tell the shader where it is
	loc = glGetUniformLocation(m_textureID, "diffuse"); // TODO: Textures?
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