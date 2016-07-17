#pragma once

#include "FBXFile.h"
#include "VertexData.h"

#include <gl_core_4_4.h>
#include <assert.h>

/**
	A Mesh contains a vertex array object, a vertex buffer object and a vertex index object.
*/
class Mesh
{
public:
	Mesh() :
		m_indexCount(10),
		m_VAO(USHRT_MAX),
		m_VBO(USHRT_MAX),
		m_IBO(USHRT_MAX)
	{}
	~Mesh() = default;

	bool isValid() { return m_indexCount > 0 && m_VAO != (GLuint)-1; } // TODO: make one for vertex?

	//TODO: these should not be public
	GLuint& GetVAO() { return m_VAO; }
	GLuint& GetVBO() { return m_VBO; }
	GLuint& GetIBO() { return m_IBO; }
	GLuint GetVertexCount() const { return m_vertexCount; }
	GLuint GetIndexCount() const { return m_indexCount; }
	//--------------------------------

	GLvoid SetVertexCount(GLuint a_iVertexCount) { m_vertexCount = a_iVertexCount; }
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
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), (void*)(sizeof(glm::vec4)));
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

	template<>
	GLvoid SetupVertexAttributes<SimpleVertex>()
	{
		// Position vertex attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);

		// Texcoord vertex attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(sizeof(float) * 3));
		
		// Normal vertex attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(sizeof(float) * 6));
	}

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;
	GLuint m_indexCount;

	// Meshes/ Mesh/ SubMesh
	unsigned int sub_mesh_count;
	SubMesh* sub_meshes;

	GLuint m_vertexCount;
	SimpleVertex* vertex_data;
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

	// OpenGL Index Buffer/ Create the IBO
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

/*// what used to be the 
// Meshes* CreateMeshFromBuffers(SimpleVertex* vertex_data, unsigned int vertex_count,
// unsigned int *index_data, unsigned int index_count, Material material) func
template<typename T>
bool Mesh::Create(T* pVertices, GLuint vertexCount, GLuint* pIndices, GLuint indexCount, Material material)
{
	////assert(m_VAO == (GLushort)-1 && "Mesh has already been created.");
	//assert(pVertices != nullptr);
	//assert(vertexCount != 0);
	//assert(pIndices != nullptr);
	//assert(indexCount != 0);

	if (!vertex_data || !index_data) return 0;

	//NOTE(aidan): all data for the scene amortized into a single buffer for easy deallocation
	//			This is easy to do as its all just simple POD types
	size_t total_bytes = sizeof(Meshes) +
		sizeof(SubMesh) +
		sizeof(SimpleVertex) * vertex_count +
		sizeof(unsigned int) * index_count;
	void* memory = calloc(total_bytes, 1);

	Meshes* result = (Meshes*)memory;
	SubMesh* sub_mesh = (SubMesh*)(result + 1);
	SimpleVertex* verts = (SimpleVertex*)(sub_mesh + 1);
	unsigned int* indices = (unsigned int*)(verts + vertex_count);

	memcpy(verts, vertex_data, sizeof(SimpleVertex) * vertex_count);
	memcpy(indices, index_data, sizeof(unsigned int) * index_count);

	result->sub_meshes = sub_mesh;
	result->sub_mesh_count = 1;
	result->vertex_count = vertex_count;
	result->vertex_data = verts;

	sub_mesh->index_count = index_count;
	sub_mesh->index_data = indices;
	sub_mesh->material = material;

	glGenBuffers(1, &result->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, result->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * result->vertex_count, result->vertex_data, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &sub_mesh->vao);
	glBindVertexArray(sub_mesh->vao);

	glGenBuffers(1, &sub_mesh->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub_mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sub_mesh->index_count, sub_mesh->index_data, GL_STATIC_DRAW);

	SetupVertexAttributes<T>();

	glBindVertexArray(0);

	return true;
} */