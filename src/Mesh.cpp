#include "Mesh.h"
//#include "VertexData.h"

GLvoid Mesh::Destroy()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);

	m_VAO = USHRT_MAX; //(GLuint)-1; //UINT_MAX
	m_VBO = USHRT_MAX; //(GLuint)-1; // TODO: Needed?
	m_IBO = USHRT_MAX; //(GLuint)-1; // TODO: Needed?
}