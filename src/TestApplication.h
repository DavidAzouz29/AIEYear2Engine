#pragma once

#include "BaseApplication.h"
#include "Mesh.h"
#include "Render.h"

#include <gl_core_4_4.h>
#include <vector>
#include <memory>
// only needed for the camera picking
#include <glm/vec3.hpp>

class CameraStateMachine;
class Camera;
//class Render; //TODO: remove
class Entity;
class MathCollision;
class RenderTarget;
//class Mesh; // TODO: needed?

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

	TestApplication();
	virtual ~TestApplication();

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
	//std::shared_ptr<Render> m_pRenderApp; //TODO: remove other?
	//Render* m_pRenderApp; //TODO: remove other
	std::shared_ptr<MathCollision> m_pMath;
	std::shared_ptr<RenderTarget> m_pRenderTarget;
//	std::unique_ptr<Entity> m_pEntity; // <- needed?
	std::vector< std::shared_ptr<Entity> > m_entities;
	Mesh m_mesh;
	Render m_render;
	Camera* m_pCamState;

	/// ----------------------------------------------------------
	E_DRAW_STATE m_eCurrentDrawState;
	GLfloat m_fPrevTime;
	/// ----------------------------------------------------------
	// this is an example position for camera picking
	glm::vec3	m_pickPosition;
};