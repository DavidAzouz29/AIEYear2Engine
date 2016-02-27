#include "TestApplication.h"
#include "gl_core_4_4.h"
#include "Camera.h"
#include "Gizmos.h"
#include "ParticleEmitter.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <assert.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

TestApplication::TestApplication()
	: m_pCamera(nullptr) {

}

TestApplication::~TestApplication() {

}

bool TestApplication::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic shapes
	Gizmos::create();

	// create a camera
	m_pCamera = std::make_shared<Camera>(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_pCamera->setLookAtFrom(vec3(10, 10, 10), vec3(0));
	
	//////////////////////////////////////////////////////////////////////////
	// YOUR STARTUP CODE HERE
	// -----------------------
	//unsigned int id = m_pRender->TextureInit("./data/textures/crate.png");
	//m_pRender->AddTexture("crate", id);

	//id = render->TextureInit("./data/models/soulspear/soulspear_diffuse.tga");
	//render->AddTexture("soulspear_d", id);

	//m_pRender->RenderTexture();
	
	///----------------------------------------------------------
	m_pParticleEmitterA = std::make_shared<ParticleEmitter>();
	m_pParticleEmitterB = std::make_shared<ParticleEmitter>();
	///----------------------------------------------------------

	GLuint uiAmount = 20; //TODO: 3000
	ParticleEmitterConfig configA;
	configA.particleCount = uiAmount; //1000 
	configA.emitRate = (float)configA.particleCount / 2; //500
	configA.startColor = glm::vec4(1.56f, 0, 1.25f, 0.8f); //glm::vec4(1, 0, 0, 1); <-RED
	configA.endColor = glm::vec4(0, 0.07f, 0.3f, 1);  //vec4(0, 0, 1, 0.8f); //vec4(0, 0, 1, 0.8f);//vec4(1, 1, 0, 1);
	configA.lifespanMin = 0.1f;
	configA.lifespanMax = 5; //5
	configA.startSize = 0.4f; // 1
	configA.endSize = 0.1f;
	configA.velocityMin = 0.1f;
	configA.velocityMax = 1.0f;
	configA.v3ParticlePosition = glm::vec3(-3, 5, 0);

	if (!m_pParticleEmitterA->create(configA)) return -4;

	ParticleEmitterConfig configB;
	configB.particleCount = uiAmount; //5000
	configB.emitRate = (float)configB.particleCount / 2; //500
	configB.startColor = glm::vec4(1, 0, 1, 0.8f); //glm::vec4(1, 0, 0, 1); <-RED
	configB.endColor = glm::vec4(0.3, 0, 0.07f, 1);  //vec4(0, 0, 1, 0.8f); //vec4(0, 0, 1, 0.8f);//vec4(1, 1, 0, 1);
	configB.lifespanMin = 0.9f;
	configB.lifespanMax = 3; //5
	configB.startSize = 0.4f; // 1
	configB.endSize = 0.2f;
	configB.velocityMin = 0.1f;
	configB.velocityMax = 2.0f;
	configB.v3ParticlePosition = glm::vec3(3, 5, 0);

	if (!m_pParticleEmitterB->create(configB)) return -5;
	///--------------------------------------------------
	//////////////////////////////////////////////////////////////////////////
	m_pickPosition = glm::vec3(0);

	return true;
}

void TestApplication::shutdown() {

	//////////////////////////////////////////////////////////////////////////
	// YOUR SHUTDOWN CODE HERE
	//////////////////////////////////////////////////////////////////////////

	// delete our camera and cleanup gizmos
	//delete m_pCamera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool TestApplication::update(float deltaTime) {
	
	// close the application if the window closes
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_pCamera->update(deltaTime);

	// clear the gizmos out for this frame
	Gizmos::clear();

	//////////////////////////////////////////////////////////////////////////
	// YOUR UPDATE CODE HERE
	// Particles
	m_pParticleEmitterA->update(deltaTime, m_pCamera->getTransform());
	m_pParticleEmitterB->update(deltaTime, m_pCamera->getTransform());

	//////////////////////////////////////////////////////////////////////////

	// an example of mouse picking
	if (glfwGetMouseButton(m_window, 1) == GLFW_PRESS) {
		double x = 0, y = 0;
		glfwGetCursorPos(m_window, &x, &y);

		// plane represents the ground, with a normal of (0,1,0) and a distance of 0 from (0,0,0)
		glm::vec4 plane(0, 1, 0, 0);
		m_pickPosition = m_pCamera->pickAgainstPlane((float)x, (float)y, plane);
	}
	Gizmos::addTransform(glm::translate(m_pickPosition));

	// ...for now let's add a grid to the gizmos
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));

		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	}

	// return true, else the application closes
	return true;
}

void TestApplication::draw() {

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projView = m_pCamera->getProjectionView();

	//////////////////////////////////////////////////////////////////////////
	// DRAW YOUR THINGS HERE
	// 
	//m_pVertexColoredGrid->draw(projView);
	//m_pSpriteSheetQuad->draw(projView);
	//m_pFBXMesh->draw(projView);
	// Particles
	m_pParticleEmitterA->draw(projView);
	m_pParticleEmitterB->draw(projView);

	//////////////////////////////////////////////////////////////////////////

	// display the 3D gizmos
	Gizmos::draw(projView);

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(projView);
}