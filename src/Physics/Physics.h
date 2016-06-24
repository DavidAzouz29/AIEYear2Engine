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
struct GLFWwindow;
//class PhysXComponent;

class Physics// : public TestApplication
{
public:
	Physics(Camera& a_camState) :
		m_pRenderable(std::make_shared<Renderable>()),
		m_camera(a_camState),
		isRBD(false), //TODO: causes framerate to tank
		m_pPhysXComponent(std::make_unique<PhysXComponent>()),
		m_fTimer(0)
	{}

	bool Create();
	//virtual bool Startup();
	virtual void Shutdown();
    virtual bool Update(GLfloat a_deltaTime);
    virtual void Draw(const Camera& a_camState);

	//Render and Clear
	//Add Widget moved to PhysXComponent

	void SetupPhysX();
	PxScene* CreateDefaultScene();
	void SetupIntroductionToPhysX();

	void SetupRBDTutorial();

	bool UpdatePlayerController(float a_deltaTime);

	void renderGizmos(PxScene* physics_scene);

	//void attachedRigidBodyConvex(float density, PxMaterial* g_PhysicsMaterial, bool isDynamic);
	void TerrainCollision(unsigned int _rows, unsigned int _cols, int* _samples, int _heightScalePX, int _size, PxMaterial* g_PhysicsMaterial);

	const Renderable* GetRenderable() const { return m_pRenderable.get(); }

    //Renderer* m_renderer;
    //FlyCamera m_camera;
	Camera& m_camera;
    float m_delta_time;

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

	PxMaterial*		m_pPhysics_material;
	PxMaterial*		m_pBox_material;
	PxCooking*		m_pPhysics_cooker;
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

};

#endif //CAM_PROJ_H_
