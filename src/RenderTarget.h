#pragma once
#include "gl_core_4_4.h"
//#include "VertexData.h"

//#include <assert.h>

//class Mesh;

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	GLuint& GetFBO() { return m_FBO; }
	GLuint& GetFboTexture() { return m_fboTexture; }
	GLuint& GetFboDepth() { return m_fboDepth; }

	bool CreateFrame();

private:
	GLuint m_FBO;
	GLuint m_fboTexture;
	GLuint m_fboDepth;
};

