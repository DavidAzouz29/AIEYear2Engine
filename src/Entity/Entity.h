#pragma once
//#include "VertexData.h"
#include "Camera\Camera.h"
#include "Helpers.h"
//#include "RenderTarget.h"

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
struct Renderable;
class Mesh;
class Texture;
//class MathCollision;
class BoundingShape;

class Entity
{
public:
	//--------------------------------------------------------------------------------------
	// Default Constructor with Initializer list
	//--------------------------------------------------------------------------------------
	Entity() :
		m_v3ClearColor(glm::vec3(0.25f)),
		m_v4StartColor(1, 0, 0, 1),
		m_v4EndColor(1, 1, 0, 1),
		m_bDrawGizmoGrid(true)
	{}
		// The 'default' keyword is a c++11 feature and should be used instead of {}.
	// This way the class can remain aggregate and/ or trivial.
	virtual ~Entity() = default;

	// Is an abstract class
	// cannot create instances of this class
	// Pure virtual function
	virtual bool	Create() = 0; //TODO: GLboolean?
	virtual GLvoid	Update()	{};
	virtual GLvoid	Draw(const Camera& m_pCamState) = 0; // pure func
	// Items to be drawn to our Render Target.
	GLvoid	DrawApp()			; //TODO: create a DrawApp for each class that inherits from Entity - and remove from here
	virtual GLvoid	Destroy()	{};
	virtual GLvoid	RenderUI();// = 0;

	const Renderable* GetRenderable() const { return m_pRenderable.get(); }

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

	//---------------------------------------
	// Remove
	//---------------------------------------
	//std::shared_ptr<Render> m_pRender; 
	//std::shared_ptr<Mesh> m_pMesh;
	//std::shared_ptr<Texture> m_pTexture; //TODO: remain here?
	//---------------------------------------

	std::shared_ptr<Renderable> m_pRenderable;

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

	//RenderTarget m_renderTarget; //TODO: render target? renderTarget
	bool m_bDrawGizmoGrid;

	// TODO: Transform class?
	// where our entity is in the world

	glm::mat4 m_m4WorldTransform;
};

