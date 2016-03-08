#pragma once

#include "BaseApplication.h"
#include "Render.h"
#include "VertexData.h"
#include "MathCollision.h"
#include "gl_core_4_4.h"

// only needed for the camera picking
#include <glm/vec3.hpp>

#include <memory>

class Camera;
class CameraStateMachine;
class FBXFile;
class ParticleEmitter;
class MathCollision;

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
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();
	virtual void DrawApp();

	/// ----------------------------------------------------------
	void PlanetCreation();
	void RenderGeometry();
	/// ----------------------------------------------------------
	void FBXLoader();
	void RenderFBX(Camera* cam);
	void FBXSkeletonLoader();
	void FBXSkeletonRender();
	void FBXUpdate();
	void FBXDraw();
	/// ----------------------------------------------------------

private:

	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<CameraStateMachine> m_pCameraStateMachine;
	std::shared_ptr<FBXFile> m_pFbx;
	std::shared_ptr<ParticleEmitter> m_pParticleEmitterA;
	std::shared_ptr<ParticleEmitter> m_pParticleEmitterB;
	std::shared_ptr<Render> m_pRender;
	std::shared_ptr<MathCollision> m_pMath;

	/// ----------------------------------------------------------
	/// FBXLoader
	/// ----------------------------------------------------------
	void CreateOpenGLBuffers(FBXFile* fbx);
	void CleanupOpenGLBuffers(FBXFile* fbx);
	GLfloat m_timer;

	GLuint m_program_ID;
	GLuint m_FBX_program_ID;
	/// ----------------------------------------------------------
	E_DRAW_STATE m_eCurrentDrawState;
	/// ----------------------------------------------------------

	// this is an example position for camera picking
	glm::vec3	m_pickPosition;
};