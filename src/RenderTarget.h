#pragma once
#include "gl_core_4_4.h"
//#include "Render.h" //TODO: for inheritance 
#include "Mesh.h"
//#include "VertexData.h"

//#include <assert.h>

//class Mesh;

// don't inherit from anything
class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();
	
	bool Create();
	GLvoid Destroy();
	GLvoid RenderTargetLoader();
	bool CreateFrame();
	GLvoid CreateRenderTargetQuad();

	GLuint& GetFBO() { return m_FBO; }
	GLuint& GetFboTexture() { return m_fboTexture; }
	GLuint& GetFboDepth() { return m_fboDepth; }
	
private:
	GLuint m_FBO;
	GLuint m_fboTexture;
	GLuint m_fboDepth;

	GLuint m_programID;

	Mesh m_mesh;

};

