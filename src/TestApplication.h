#pragma once

#include "BaseApplication.h"
#include "Physics.h"

#include <gl_core_4_4.h>
#include <glm/vec3.hpp> // only needed for the camera picking
#include <memory>
//#include <queue>
#include <vector>

class CameraStateMachine;
class Camera;
class Entity;
class MathCollision;
class Physics;
class RenderTarget;

/// <summary> 
/// Enumeration to decide which method to draw our scene with.
/// </summary> TODO: GLenum?
enum E_DRAW_STATE
{
	E_DRAW_STATE_FILL,
	E_DRAW_STATE_POLY, /* Polygon mode (triangles) */
	E_DRAW_STATE_DOT, /* Point mode */

	E_DRAW_STATE_COUNT,
};

class TestApplication : public BaseApplication {
public:

	//--------------------------------------------------------------------------------------
	// Default Constructor with Initializer list
	//--------------------------------------------------------------------------------------
	TestApplication() :
		m_eCurrentDrawState(E_DRAW_STATE_FILL),
		m_fPrevTime(0),
		m_v3ClearColor(glm::vec3(0.25f)),
		m_bDrawGizmoGrid(true),
		m_hasSpaceBeenPressed(false),
		//concurentThreadsSupported(0),
		chunkLength(0)
	{} //: m_pCamera(nullptr),
	virtual ~TestApplication() = default;

	virtual bool startup();
	virtual GLvoid shutdown();

	virtual bool Update(GLfloat deltaTime);
	virtual GLvoid Draw();
	// Items to be drawn to our Render Target.
	GLvoid DrawApp();
	//virtual GLvoid RenderUI();
	//static const GLuint GetNumOfThreads() { return concurentThreadsSupported; }

private:
	// This should be used for any camera related activites.
	std::shared_ptr<CameraStateMachine> m_pCameraStateMachine;
	std::shared_ptr<MathCollision> m_pMath;
	std::shared_ptr<Physics> m_pPhysics;
	std::shared_ptr<RenderTarget> m_pRenderTarget;
	std::vector< std::shared_ptr<Entity> > m_entities;
	Camera* m_pCamState;
	//std::weak_ptr<Camera> m_pCamState; //TODO: cam state?

	// ----------------------------------------------------------
	E_DRAW_STATE m_eCurrentDrawState;
	GLfloat m_fPrevTime;
	bool m_hasSpaceBeenPressed; //used for button presses
	// ----------------------------------------------------------
	// Threading
	//std::vector<std::thread> vThreads;
	//std::queue<std::thread> qThreads; //TODO: needed?
	GLuint concurentThreadsSupported;
	int chunkLength;
	// ----------------------------------------------------------
	// this is an example position for camera picking
	glm::vec3	m_pickPosition;

	glm::vec3 m_v3ClearColor;
	bool m_bDrawGizmoGrid;
};