#pragma once
//#include "VertexData.h"
#include "Camera\Camera.h"
#include "RenderTarget.h"

#include <gl_core_4_4.h>
#include <glm/vec3.hpp>
//#include <glm/mat4x2.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

//class CameraStateMachine;
//class Camera;
class ParticleEmitter;
class GPUParticleEmitter;
class FBXModel;
class Render; //TODO: render?
class Mesh;
//class MathCollision;
class BoundingShape;

class Entity
{
public:
	Entity();
	virtual ~Entity() {};

	virtual bool	Create(); //TODO: GLboolean?
	virtual GLvoid	Update()	{};
	virtual GLvoid	Draw(Camera* m_pCamState);
	// Items to be drawn to our Render Target.
	GLvoid	DrawApp()			; //TODO: create a DrawApp for each class that inherits from Entity
	virtual GLvoid	Destroy()	{};
	virtual GLvoid	RenderUI()	;

	const std::shared_ptr<Render>&	GetRender() const { return m_pRender; }

	/// <summary> Function Pointer TODO: what do I do with this?
	/// <param>P1: Program ID</param>
	/// </summary>
	/*Entity */ void(*EntityRender)(GLint) = NULL;
/*	//glm::mat4 GetColor() { return m_m4Colors; }

	static std::shared_ptr<Entity> GetSingleton() { return ms_pSingleton; }
	static void CreateSingleton() { ms_pSingleton = std::make_shared<Entity>().get(); }
	static void DestroySingleton() { delete ms_pSingleton; } */

// Protected so that other classes that inherit from
// Entity can have access to certain member variables.
protected:
	// Every entity uses frustum culling.
	//BoundingShape m_boundShape; //TODO: frustum culling

/*	//std::shared_ptr<Camera> m_pCurrentCamera;
	std::shared_ptr<ParticleEmitter> m_pParticleEmitterA;
	std::shared_ptr<ParticleEmitter> m_pParticleEmitterB;
	std::shared_ptr<GPUParticleEmitter> m_pGPUEmitter; //*/
	std::shared_ptr<Render> m_pRender; 
	std::shared_ptr<Mesh> m_pMesh;
	//std::shared_ptr<RenderTarget> m_pRenderTarget;
	//std::shared_ptr<MathCollision> m_pMath;
	//Render* m_pRender;

	/// ----------------------------------------------------------
	/// TODO: not every class should have a start and end colour?
	/// Particles would need a start and end.
	glm::vec3 m_v3ClearColor;
	glm::vec4 m_v4StartColor;
	glm::vec4 m_v4EndColor;

	/* TODO: ?
	///glm::mat4x2 m_m4Colors[2];
	//glm::mat4 m_m4Colors; */

private:
	//static Entity* ms_pSingleton;
	/// ----------------------------------------------------------
	//GLvoid PlanetCreation();
	//GLvoid RenderGeometry();
	/// ----------------------------------------------------------

	RenderTarget m_renderTarget;
	bool m_bDrawGizmoGrid;

	// TODO: Transform class?
	// where our entity is in the world
	glm::mat4 m_m4WorldTransform;
};

