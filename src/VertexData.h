#pragma once

//#include "glm\glm.hpp"
//#include <gl_core_4_4.h> //TODO: remove once this header is in Mesh/ Render
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

//#include "Mesh.h"

/*
	Vertex definitions.
	Add new structs here to define new vertex types.
	If you add a new vertex type you can add a new Mesh::create override to setup vertex attributes correctly.
*/

/*struct Vertex
{
	glm::vec4 position;
	//glm::vec4 colour;

	Vertex()
	{

	}

	Vertex(glm::vec4 in_pos)
	{
		position = in_pos;
		//colour = in_color;
	}
};

struct VertexColourTexture : public Vertex
{
	glm::vec2 texCoord;
	glm::vec4 normal;
	VertexColourTexture()
	{

	}

	VertexColourTexture(glm::vec2 in_texCoord, glm::vec4 in_norm, glm::vec4 in_pos) : Vertex(in_pos)
	{
		texCoord = in_texCoord;
		normal = in_norm;
	}
}; */

struct Vertex_PositionColor
{
	glm::vec4 position;
	glm::vec4 color;
};

struct Vertex_PositionTexCoord
{
	glm::vec4 position;
	glm::vec2 uv;
};

/*class Geometry
{
	GLuint VAO; //TODO: is it GLuint || GLuint*
	GLuint VBO;
	GLuint IBO;
	unsigned int m_iIndexCount;

public:

	///----------------------------------------------------------
	/// Getters
	///----------------------------------------------------------
	/// <summary> Gets the VAO GL Buffer.
	/// <returns>Returns our GL Buffer.</returns>
	/// </summary> //inline GLuint GetVAO() const { return VAO; }
	///----------------------------------------------------------
	GLuint GetVAO() const { return VAO; }
	GLuint GetVBO() const { return VBO; }
	GLuint GetIBO() const { return IBO; }
	GLuint GetIndexCount() const { return m_iIndexCount; }

	///----------------------------------------------------------
	/// Setters
	///----------------------------------------------------------
	/// <summary>Sets the VAO GL Buffer.
	/// <param name="a_iVAO" type ="GLuint">Holds our buffer.</param>
	/// </summary> 
	///----------------------------------------------------------
	inline void SetVAO(GLuint a_iVAO) { VAO = a_iVAO; }
	inline void SetVBO(GLuint a_iVBO) { VBO = a_iVBO; }
	inline void SetIBO(GLuint a_iIBO) { IBO = a_iIBO; }
	inline void SetIndexCount(unsigned int a_iIndexCount) { m_iIndexCount = a_iIndexCount; }

}; */

// Push on your vector