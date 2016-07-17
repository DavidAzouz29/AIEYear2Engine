#pragma once

#include "BaseApplication.h"
#include "CameraStateMachine.h"
#include "MathCollision.h" //
#include "Physics.h"

#include <gl_core_4_4.h>
#include <glm/vec3.hpp> // only needed for the camera picking
#include <memory>
//#include <queue>
#include <vector>

//class CameraStateMachine;
class Camera;
class Entity;
//class MathCollision;
//class Physics;
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
		//m_v4Perspective(glm::pi<GLfloat>() * 0.25f, 16 / 9.0f, 0.1f, 10000.f),
		//m_pCameraStateMachine(std::make_shared<CameraStateMachine>(m_v4Perspective)),
		//m_pCamState(m_pCameraStateMachine->GetCurrentCamera()),
		m_eCurrentDrawState(E_DRAW_STATE_FILL),
		m_fPrevTime(0),
		m_v3ClearColor(glm::vec3(0.25f)),
		m_bDrawGizmoGrid(true),
		m_isCharacterControlled(false),
		m_hasSpaceBeenPressed(false),
		//concurentThreadsSupported(std::thread::hardware_concurrency()),
		chunkLength(0),
		m_pMath(std::make_shared<MathCollision>()),
		m_pPhysics(std::make_shared<Physics>()),
		m_pickPosition(glm::vec3(0))
		{} //: m_pCamera(nullptr),
	virtual ~TestApplication() = default;

	virtual bool startup();
	virtual GLvoid shutdown();

	virtual bool Update(GLfloat deltaTime);
	virtual GLvoid Draw();
	// Items to be drawn to our Render Target.
	GLvoid DrawApp();
	//virtual GLvoid RenderUI();

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
	//GLuint concurentThreadsSupported; //Moved to Base
	unsigned int chunkLength;
	// ----------------------------------------------------------
	// this is an example position for camera picking
	glm::vec3	m_pickPosition;

	glm::vec3 m_v3ClearColor;
	bool m_bDrawGizmoGrid;
	bool m_isCharacterControlled;
	/// ------------------------------------------------------------
	/// <summary>
	/// Camera Perspective
	/// ------------------------------------------------------------
	/// <para>P1: FoVY: Field of View Y.</para>
	/// <para>P2: Aspect Ratio.</para>
	/// <para>P3: Near: nearest clipping plane to render to.</para>
	/// <para>P4: Far: furthest clipping plane to render to.</para>
	/// </summary>
	/// ------------------------------------------------------------
	//glm::vec4 m_v4Perspective;
};