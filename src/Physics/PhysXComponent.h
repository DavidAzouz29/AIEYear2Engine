#pragma once

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <memory>

// Fwd decls
namespace physx {
	class PxAllocatorCallback;
	class PxDefaultCpuDispatcher;
	class PxErrorCallback;
	class PxFoundation;
	//class PxGeometry;
	class PxMaterial;
	class PxPhysics;
	class PxPlaneGeometry;
	class PxRigidActor;
	class PxScene;
	class PxShape;
	namespace debugger {
		namespace comm {
			class PvdConnection;
		}
	}
	typedef debugger::comm::PvdConnection PxVisualDebuggerConnection;
}

class PhysXComponent
{
public:
	PhysXComponent() : PhysXComponent(glm::vec3(0)) {}
	PhysXComponent(glm::vec3 offset);
	~PhysXComponent();

	//TOOD: make private?
	void AddWidget(physx::PxShape* shape, physx::PxRigidActor* actor, glm::vec4 geo_color);

private:
	const float c_DefaultStaticFriction = 0.5f;
	const float c_DefaultDynamicFriction = 0.5f;
	const float c_DefaultRestitution = 0.5f;
	const float c_DefaultDensity = 1;

	void SetupVisualDebugger();

	physx::PxVisualDebuggerConnection* m_pConnection;

	physx::PxFoundation* m_pFoundation;
	physx::PxPhysics* m_pPhysics;
	physx::PxScene* m_pScene;
	
	physx::PxDefaultCpuDispatcher* m_pCPUDispatcher;

	physx::PxMaterial* m_pDefaultMaterial;

	std::unique_ptr<physx::PxAllocatorCallback> m_allocatorCallback;
	std::unique_ptr<physx::PxErrorCallback> m_errorCallback;
};

