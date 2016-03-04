#pragma once

// only needed for the camera picking
#include <glm/vec3.hpp>

#include "BaseApplication.h"
#include "Render.h"
#include "VertexData.h"
#include "MathCollision.h"

#include <memory>

class Camera;
class FBXFile;
class ParticleEmitter;
class MathCollision;

class TestApplication : public BaseApplication {
public:

	TestApplication();
	virtual ~TestApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

	/// ----------------------------------------------------------
	void PlanetCreation();
	void RenderGeometry();
	/// ----------------------------------------------------------
	void FBXLoader();
	void RenderFBX(Camera* cam);
	/// ----------------------------------------------------------

private:

	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<FBXFile> m_pFbx;
	std::shared_ptr<Mesh> m_pMesh;
	std::shared_ptr<ParticleEmitter> m_pParticleEmitterA;
	std::shared_ptr<ParticleEmitter> m_pParticleEmitterB;
	std::shared_ptr<Render> m_pRender;	//Render* render;
	std::shared_ptr<MathCollision> m_pMath;

	/// ----------------------------------------------------------
	/// FBXLoader
	/// ----------------------------------------------------------
	void CreateOpenGLBuffers(FBXFile* fbx);
	void CleanupOpenGLBuffers(FBXFile* fbx);
	
	unsigned int m_program;
	/// ----------------------------------------------------------

	// this is an example position for camera picking
	glm::vec3	m_pickPosition;
};