#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

//#include "TestApplication.h"
#include "Camera\Camera.h"
//#include "Render.h"
#include "PhysXComponent.h"
#include "Renderable.h"

#include <memory>
#include <PxPhysicsAPI.h>
#include <PxScene.h>

using namespace physx;

//class Camera;
class ControllerHitReport;
class FBXFile;
struct GLFWwindow;
//class PhysXComponent;

class Physics// : public TestApplication
{
public:
	Physics() :
		m_pRenderable(std::make_shared<Renderable>()),
		//m_camera(a_camState),
		isRBD(false), //TODO: causes framerate to tank
		m_pPhysXComponent(std::make_unique<PhysXComponent>()),
		m_fTimer(0)
	{}

	bool Create();
	//virtual bool Startup();
	virtual void Shutdown();
    virtual bool Update(GLfloat a_deltaTime, const Camera& a_camera);
    virtual void Draw(const Camera& a_camState);
	virtual void RenderUI();

	//Render and Clear
	//Add Widget moved to PhysXComponent

	void SetupPhysX();
	PxScene* CreateDefaultScene();
	void SetupIntroductionToPhysX();

	void SetupRBDTutorial();

	bool UpdatePlayerController(float a_deltaTime);

	void renderGizmos(PxScene* physics_scene);

	void AttachedRigidBodyConvex(float density, PxMaterial* g_PhysicsMaterial, bool isDynamic);
	void TerrainCollision(unsigned int _rows, unsigned int _cols, int* _samples, int _heightScalePX, int _size);// , PxMaterial* g_PhysicsMaterial);

	const Renderable* GetRenderable() const { return m_pRenderable.get(); }
	//static PxMaterial* GetPxMat() { return m_pPhysics_material; }

    //Renderer* m_renderer;
	//Camera& m_camera;

	bool isRBD; //enable the ball firing tutorial

	//unsigned int main_shader;

private:
	std::shared_ptr<Renderable> m_pRenderable;

	PxFoundation*	m_pPhysics_foundation;
	PxPhysics*		m_pPhysics;
	PxScene*		m_pPhysics_scene;

	PxDefaultErrorCallback		m_default_error_callback;
	PxDefaultAllocator			m_default_allocator;
	PxSimulationFilterShader	m_default_filter_shader;

	PxMaterial*					m_pPhysics_material;
	PxMaterial*					m_pBox_material;
	PxCooking*					m_pPhysics_cooker;
	PxControllerManager*		m_pController_manager;
	PxController*				m_pPlayerController;
	//ControllerHitReport* m_pMyHitReport;
	std::shared_ptr<ControllerHitReport> m_pMyHitReport;
	float						m_pCharacterYvelocity;
	float						m_pCharacterRotation;
	float						m_pPlayerGravity;

	PxRigidDynamic* m_pPlayerModel;
	PxArticulation* m_pRagdollArticulation;

	std::unique_ptr<PhysXComponent> m_pPhysXComponent;
	GLFWwindow* m_pWindow;

	float m_fTimer;
	float m_characterYvelocity;
	float m_characterRotation;
	float m_playerGravity;

	//TODO: set all these values somewhere
	PxExtendedVec3 m_v3PhysicsWorldTransform;
	//glm::mat4 m_worldTransform; // named differently to identify the physics version
	PxRigidDynamic* _pXactor;
	PxRigidStatic* _pXactorS;
	FBXFile* _FBXModel;
	int numberIndex;
	int* indexes;
};

#endif //CAM_PROJ_H_
