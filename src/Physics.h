#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

//#include "TestApplication.h"
#include "Camera\Camera.h"
//#include "Render.h"
#include "Renderable.h"

#include <memory>
#include <PxPhysicsAPI.h>
#include <PxScene.h>

using namespace physx;

//class Camera;

class Physics// : public TestApplication
{
public:
	Physics(Camera& a_camState) :
		m_pRenderable(std::make_shared<Renderable>()),
		m_camera(a_camState),
		isRBD(true),
		m_fTimer(0)
	{}

	bool Create();
	//virtual bool Startup();
	virtual void Shutdown();
    virtual bool Update(GLfloat deltaTime);
    virtual void Draw(const Camera& a_camState);

	//Render and Clear
	//Add Widget

	void SetupPhysX();
	PxScene* CreateDefaultScene();
	void SetupIntroductionToPhysX();

	void SetupRBDTutorial();

	void renderGizmos(PxScene* physics_scene);

	const Renderable* GetRenderable() const { return m_pRenderable.get(); }

    //Renderer* m_renderer;
    //FlyCamera m_camera;
	Camera& m_camera;
    float m_delta_time;

	bool isRBD; //enable the ball firing tutorial

	//unsigned int main_shader;

private:
	std::shared_ptr<Renderable> m_pRenderable;

	PxFoundation*	m_physics_foundation;
	PxPhysics*		m_physics;
	PxScene*		m_physics_scene;

	PxDefaultErrorCallback		m_default_error_callback;
	PxDefaultAllocator			m_default_allocator;
	PxSimulationFilterShader	m_default_filter_shader;

	PxMaterial*		m_physics_material;
	PxMaterial*		m_box_material;
	PxCooking*		m_physics_cooker;
	PxControllerManager*		m_controller_manager;
	//PxController*				m_playerController; //TOOD: 

	float m_fTimer;

};

#endif //CAM_PROJ_H_
