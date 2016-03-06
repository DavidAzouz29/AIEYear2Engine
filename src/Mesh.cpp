#include "Mesh.h"

//#include "VertexData.h"

#include <assert.h>

void Mesh::destroy()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);

	m_VAO = (GLuint)-1;
	// m_VBO = (GLuint)-1; // TODO: Needed?
	// m_IBO = (GLuint)-1; // TODO: Needed?
}

bool Mesh::createFrame()
{
	// setup and bind a frambuffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// TODO: we would attach render targets here!
	// Create a texture and bind it
	glGenTextures(1, &m_fboTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);

	// specify texture format for storage
	// Format of the texture: GL_RGBA8, GL_RGB8. GL_RGBA32F
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 512, 512);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// attach it to the framebuffer as the first colour attachment
	// the FBO MUST still be bound
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fboTexture, 0);

	/// setup and bind a 24bit depth buffer as a render buffer
	glGenRenderbuffers(1, &m_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	// while the FBO is still bound
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);

	// while the FBO is still bound
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error!\n");
	}

	// Render Geometry to the FBO.
	RenderRenderTarget(); //TODO: this saves things from breaking?

	// unbind the FBO so that we can render to the back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void Mesh::RenderRenderTarget()
{
	float vertexData[] = {
		-5,0, -5,1,0,0,
		5,0,-5,1,1,0,
		5,10,-5,1,1,1,
		-5,10,-5,1,0,1,
	};

	// Recreates the textured quad, except that they now contain normal that point up (0,1,0)
	// and tangents to point in the direction of the texture's S axis (or U axis) which is position X (1,0,0).
	/*	Vertex vertexData[] =
	{
	{ -5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
	{ 5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
	{ 5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
	{ -5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 }
	}; */
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Add the following line to generate a VertexArrayObject
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// ----------------------------------------------------------
	// Generate our GL Buffers
	// Let's move these so that they are all generated together
	// ----------------------------------------------------------
	glGenBuffers(1, &m_VBO);
	//... Code Segment here to bind and fill VBO + IBO
	//
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IBO);
	// 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}