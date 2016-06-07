#pragma once

/// viewed: https://github.com/johnsietsma/RefEngine/blob/102a07439ebf40182c4cab27df3001f0607234cc/Engine/src/graphics/RenderPass.cpp
//#include "Render.h" //TODO: for inheritance 
#include "Mesh.h"
#include "Texture.h"
//#include "VertexData.h"

#include <gl_core_4_4.h>
//#include <assert.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp> //TODO: replace RRTQ with const Camera& a_cam;

//#include <bitset> std::bitset<8> layers; //TODO: 
#include <memory>

class Camera;

// don't inherit from anything
class RenderTarget
{
public:
	//RenderTarget(std::weak_ptr<Camera> a_pCamera, glm::vec3 a_v3ClearColor, glm::ivec2 a_iv2FboSize) :
	RenderTarget(Camera* a_pCamera, glm::ivec2 a_iv2FboSize) :
		m_fboID(USHRT_MAX),
		m_fboTexture(-1),
		m_fboDepth(USHRT_MAX),
		m_pCamera(a_pCamera),
		//m_clearColor(a_v3ClearColor),
		m_fboSize(a_iv2FboSize)
	{}
	
	bool Create();
	GLvoid Destroy();
	
	//GLvoid Draw() {};
	GLvoid RenderTargetLoader();
	bool CreateFrame();
	GLvoid CreateRenderTargetQuad();
	GLvoid RenderRenderTargetQuad(const glm::mat4& a_projectionView); // TODO: <- replace with cam
	//To be called after Draw
	GLvoid BindDraw();

	GLuint& GetFBO() { return m_fboID; }
	//GLuint& GetFboTexture() { return m_fboTexture; }
	Texture GetTexture() const { return m_fboTexture; }
	Mesh GetMesh() const { return m_mesh; }
	GLuint& GetFboDepth() { return m_fboDepth; }
	
private:
	GLuint m_fboID; //What was GLuint m_programID;
	Texture m_fboTexture = -1;
	GLuint m_fboDepth;
	Camera* m_pCamera;
	//std::weak_ptr<Camera> m_pCamera;
	//glm::vec3 m_clearColor;
	glm::ivec2 m_fboSize;

	Mesh m_mesh; // TODO: solve - only needed in BindDraw GetVAO?

};

