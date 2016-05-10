#pragma once

#include "FBXFile.h"
#include "VertexData.h"

#include <gl_core_4_4.h>
#include <assert.h>

/*
	A Mesh is a vertex array object, a vertex buffer object and a vertex index object.
*/
class Mesh
{
public:
	Mesh();
	~Mesh();

	bool isValid() { return m_indexCount > 0 && m_VAO != (GLuint)-1; }

	//TODO: these should not be public
	GLuint& GetVAO() { return m_VAO; }
	GLuint& GetVBO() { return m_VBO; }
	GLuint& GetIBO() { return m_IBO; }
	GLuint GetIndexCount() const { return m_indexCount; }
	//--------------------------------

	GLvoid SetIndexCount(GLuint a_iIndexCount) { m_indexCount = a_iIndexCount; }

	//GLvoid CreateRenderTargetQuad();

	// Setup OpenGL buffers and vertex attributes to be able to render these vertices.
	template<typename T>
	bool Create(T* pVertices, GLuint vertexCount, GLuint* pInidices, GLuint indexCount);

	GLvoid Destroy();

private:
	// Specialize this template to do custom vertex attribute setup for your struct.
	template<typename T>
	GLvoid SetupVertexAttributes() { assert(false); }

	template<>
	GLvoid SetupVertexAttributes<GLfloat>()
	{
		// Position vertex attribute
		glEnableVertexAttribArray(0); //TODO: substitute 6 for another value
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);

		// Color vertex attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, ((GLchar*)0) + 16);
	}

	template<>
	GLvoid SetupVertexAttributes<Vertex_PositionColor>()
	{
		// Position vertex attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), (GLvoid*)(offsetof(Vertex_PositionColor, position)));

		// Color vertex attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), (GLvoid*)(offsetof(Vertex_PositionColor, color)));
	}

	template<>
	GLvoid SetupVertexAttributes<Vertex_PositionTexCoord>()
	{ 
		// Position vertex attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionTexCoord), (GLvoid*)(offsetof(Vertex_PositionTexCoord, position)));

		// Texcoord vertex attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionTexCoord), (GLvoid*)(offsetof(Vertex_PositionTexCoord, uv)));

	}

	template<>
	GLvoid SetupVertexAttributes<FBXVertex>()
	{
		// Position vertex attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)(offsetof(FBXVertex, position)));

		// Texcoord vertex attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)(offsetof(FBXVertex, texCoord1)));

		// Normal vertex attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)(offsetof(FBXVertex, normal)));

		// Tangent vertex attribute
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)(offsetof(FBXVertex, tangent)));

		// Weights vertex attribute
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)(offsetof(FBXVertex, weights)));

		// Indices vertex attribute
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)(offsetof(FBXVertex, indices)));
	}

	GLuint m_indexCount;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;
};

/// ----------------------------------------------------------
/// Create Mesh
///-----------------------------------------------------------------------------------------------------------
/// <summary> 
/// <para>Bind the shader and send across the virtual camera's projection and view</para>
/// <para>matrices combined, then we loop through the meshes and render them.</para>
/// <para><param name="pVertices"	type ="T*">		 P1: Verts data.</param></para>
/// <para><param name="vertexCount" type ="GLuint "> P2: How many Verts we have (allocate buffer).</param></para>
/// <para><param name="pIndices"	type ="GLuint*"> P3: Index/ Indices data.</param></para>
/// <para><param name="indexCount"	type ="GLuint "> P4: Used to determine size of buffer data.</param></para></summary>
///  _______  - 
/// |	|	| -
/// |___|___| -
/// |	|	| 
/// |___|___| - 
///-----------------------------------------------------------------------------------------------------------
template<typename T>
bool Mesh::Create(T* pVertices, GLuint vertexCount, GLuint* pIndices, GLuint indexCount)
{
	assert(m_VAO == (GLushort)-1 && "Mesh has already been created.");
	assert(pVertices != nullptr);
	assert(vertexCount != 0);
	assert(pIndices != nullptr);
	assert(indexCount != 0);

	// Create the VAO. Must be first so the buffers are associated with it.
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// ----------------------------------------------------------
	// Generate GL Buffers
	// ----------------------------------------------------------
	glGenBuffers(1, &m_VBO); // Create the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Make it active
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(T), pVertices, GL_STATIC_DRAW); // Upload data

	SetupVertexAttributes<T>();

	// Create the IBO
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(GLuint), pIndices, GL_STATIC_DRAW);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_indexCount = indexCount;

	return true;
}