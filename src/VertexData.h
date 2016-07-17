#pragma once

//#include "Mesh.h"
//#include "glm\glm.hpp"
//#include <gl_core_4_4.h> //TODO: remove once this header is in Mesh/ Render
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;

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
	vec4 position;
	vec4 color;
};

struct Vertex_PositionTexCoord
{
	vec4 position;
	vec2 uv;
};

struct SimpleVertex
{
	vec3 pos;
	vec3 normal;
	vec2 uv;
};

struct Material
{
	int diffuse_texture;
	int normal_texture;
	int specular_texture;

	vec3 diffuse_color;
	vec3 specular_color;
};

struct SubMesh
{
	Material material;

	unsigned int ibo;
	unsigned int vao;

	unsigned int* index_data;
	unsigned int index_count;
};

struct Meshes
{
	unsigned int sub_mesh_count;
	SubMesh* sub_meshes;

	unsigned int vbo;

	unsigned int vertex_count;
	SimpleVertex* vertex_data;
}; 

struct Scene
{
	Meshes* meshes;
	unsigned int mesh_count;
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