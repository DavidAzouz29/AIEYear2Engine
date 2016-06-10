#pragma once

#include "BaseApplication.h"
#include "Mesh.h"
#include "Render.h"
#include "Texture.h"

#include <gl_core_4_4.h>
#include <vector>
#include <memory>
// only needed for the camera picking
#include <glm/vec3.hpp>

class CameraStateMachine;
class Camera;
class Entity;
class MathCollision;
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
		//m_pRenderApp(nullptr),
		m_fPrevTime(0),
		m_v3ClearColor(glm::vec3(0.25f)),
		m_bDrawGizmoGrid(true)
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
	std::shared_ptr<RenderTarget> m_pRenderTarget;
	std::vector< std::shared_ptr<Entity> > m_entities;
	Camera* m_pCamState;
	//std::weak_ptr<Camera> m_pCamState; //TODO: cam state?

	/// ----------------------------------------------------------
	E_DRAW_STATE m_eCurrentDrawState;
	GLfloat m_fPrevTime;
	/// ----------------------------------------------------------
	// this is an example position for camera picking
	glm::vec3	m_pickPosition;

	glm::vec3 m_v3ClearColor;
	bool m_bDrawGizmoGrid;
};