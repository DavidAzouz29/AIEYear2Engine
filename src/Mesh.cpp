#include "Mesh.h"

//#include "VertexData.h"

#include <assert.h>

Mesh::Mesh()
{
	m_indexCount = 10;
	m_VAO = UINT_MAX;
	m_VBO = UINT_MAX;
	m_IBO = UINT_MAX;
}

Mesh::~Mesh()
{

}

GLvoid Mesh::Destroy()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);

	m_VAO = UINT_MAX; //(GLuint)-1;
	m_VBO = UINT_MAX; //(GLuint)-1; // TODO: Needed?
	m_IBO = UINT_MAX; //(GLuint)-1; // TODO: Needed?
}