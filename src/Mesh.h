#pragma once

#include "gl_core_4_4.h"
#include "FBXFile.h"
#include "VertexData.h"

#include <assert.h>

/*
	A Mesh is a vertex array object, a vertex buffer object and a vertex index object.
*/
class Mesh
{
public:
	bool isValid() { return m_indexCount > 0 && m_VAO != (GLuint)-1; }

	GLuint& GetVAO() { return m_VAO; }
	GLuint& GetVBO() { return m_VBO; }
	GLuint& GetIBO() { return m_IBO; }
	GLuint& GetFBO() { return m_FBO; }
	GLuint& GetFboTexture() { return m_fboTexture; }
	GLuint& GetFboDepth() { return m_fboDepth; }
	GLuint GetIndexCount() const { return m_indexCount; }

	void SetIndexCount(unsigned int a_iIndexCount) { m_indexCount = a_iIndexCount; }

	void RenderRenderTarget();

	// Setup OpenGL buffers and vertex attributes to be able to render these vertices.
	template<typename T>
	bool create(T* pVertices, unsigned int vertexCount, unsigned int* pInidices, unsigned int indexCount);

	bool createFrame();

	void destroy();

private:
	// Specialize this template to do custom vertex attribute setup for your struct.
	template<typename T>
	void SetupVertexAttributes() { assert(false); }

	template<>
	void SetupVertexAttributes<Vertex_PositionColor>() 
	{
		// Position vertex attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), (void*)(offsetof(Vertex_PositionColor, position)));

		// Color vertex attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), (void*)(offsetof(Vertex_PositionColor, color)));

	}

	template<>
	void SetupVertexAttributes<Vertex_PositionTexCoord>() 
	{ 
		// Position vertex attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionTexCoord), (void*)(offsetof(Vertex_PositionTexCoord, position)));

		// Texcoord vertex attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionTexCoord), (void*)(offsetof(Vertex_PositionTexCoord, uv)));

	}

	template<>
	void SetupVertexAttributes<FBXVertex>()
	{
		// Position vertex attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(offsetof(FBXVertex, position)));

		// Texcoord vertex attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(offsetof(FBXVertex, texCoord1)));

		// Normal vertex attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(offsetof(FBXVertex, normal)));

		// Tangent vertex attribute
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(offsetof(FBXVertex, tangent)));

		// Weights vertex attribute
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(offsetof(FBXVertex, weights)));

		// Indices vertex attribute
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(offsetof(FBXVertex, indices)));
	}

	unsigned int m_indexCount = 0;
	GLuint m_VAO = (GLuint)-1;
	GLuint m_VBO = (GLuint)-1;
	GLuint m_IBO = (GLuint)-1;

	GLuint m_FBO = (GLuint)-1;
	GLuint m_fboTexture = (GLuint)-1;
	GLuint m_fboDepth	= (GLuint)-1;
};

/// ----------------------------------------------------------
/// Create Mesh
///-----------------------------------------------------------------------------------------------------------
/// <summary> 
/// <para>Bind the shader and send across the virtual camera's projection and view</para>
/// <para>matrices combined, then we loop through the meshes and render them.</para>
/// <para><param name="pVertices"	type ="T*">		 P1: Our Verts to .</param></para>
/// <para><param name="vertexCount" type ="GLuint "> P2: Amount of Verts to .</param></para>
/// <para><param name="pIndices"	type ="GLuint*"> P3: Indices to.</param></para>
/// <para><param name="indexCount"	type ="GLuint "> P4: Used to determine size of buffer data.</param></para></summary>
///  _______  - 
/// |	|	| -
/// |___|___| -
/// |	|	| 
/// |___|___| - 
///-----------------------------------------------------------------------------------------------------------
template<typename T>
bool Mesh::create(T* pVertices, GLuint vertexCount, GLuint* pIndices, GLuint indexCount)
{
	assert(m_VAO == -1 && "Mesh has already been created.");
	assert(pVertices != nullptr);
	assert(vertexCount != 0);
	assert(pIndices != nullptr);
	assert(indexCount != 0);

	// Create the VAO. Must be first so the buffers are associated with it.
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO); // Create the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Make it active
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(T), pVertices, GL_STATIC_DRAW); // Upload data

	SetupVertexAttributes<T>();

	// Create the IBO
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(unsigned int), pIndices, GL_STATIC_DRAW);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_indexCount = indexCount;

	return true;
}