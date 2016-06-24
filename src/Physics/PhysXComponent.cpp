#include "PhysXComponent.h"
#include "geometry/PxPlaneGeometry.h"

#include "Gizmos.h"

#include <assert.h>
#include <extensions\PxDefaultAllocator.h>
#include <extensions\PxDefaultErrorCallback.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <PxPhysicsAPI.h>

using glm::vec3;
using glm::vec4;
using namespace physx;

PhysXComponent::PhysXComponent(glm::vec3 offset) :
	m_allocatorCallback(std::make_unique<PxDefaultAllocator>()),
	m_errorCallback(std::make_unique<PxDefaultErrorCallback>()),
	m_pConnection(nullptr)
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_allocatorCallback.get(), *m_errorCallback.get());
	assert(m_pFoundation != nullptr);

	m_pPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale());
	assert(m_pPhysics != nullptr);

	m_pCPUDispatcher = PxDefaultCpuDispatcherCreate(1);

	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9.8f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = m_pCPUDispatcher;

	m_pScene = m_pPhysics->createScene(sceneDesc);
	assert(m_pScene != nullptr);

	m_pScene->shiftOrigin(PxVec3(offset.x, offset.y, offset.z));

	m_pDefaultMaterial = m_pPhysics->createMaterial(c_DefaultStaticFriction, c_DefaultDynamicFriction, c_DefaultRestitution);;

	//SetupVisualDebugger(); //TODO: Setup Visual Debugger
}

PhysXComponent::~PhysXComponent()
{
	if (m_pConnection != nullptr) {
		m_pConnection->disconnect();
		m_pConnection->release();
	}

	m_pScene->release();
	m_pCPUDispatcher->release();
	m_pPhysics->release();
	m_pFoundation->release();
}

/// --------------------------------------------------------------------
/// <summary>
/// Adds a widget: Assists with visualizing Physics Collisions
/// <para><param>P1: shape: Box, sphere, capsule.</param></para>
/// <para><param>P2: Actor: .</param></para>
/// <para><param>P3: Colour: our desired colour.</param></para>
/// <example> </example>
/// </summary>
/// --------------------------------------------------------------------
void PhysXComponent::AddWidget(PxShape* shape, PxRigidActor* actor, vec4 geo_color)
{
	PxGeometryType::Enum geo_type = shape->getGeometryType();
	PxTransform full_transform = PxShapeExt::getGlobalPose(*shape, *actor);
	
	if (geo_type == PxGeometryType::ePLANE || geo_type == PxGeometryType::eHEIGHTFIELD)
	{
		full_transform.transform(PxPlaneEquationFromTransform(PxShapeExt::getGlobalPose(*shape, *actor)));
	}

	vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
	glm::quat actor_rotation(full_transform.q.w, full_transform.q.x,
		full_transform.q.y, full_transform.q.z);
	glm::mat4 rot(actor_rotation);

	glm::mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));
	float radius = 0.0f;

	switch (geo_type)
	{
	case (PxGeometryType::eBOX) :
	{
		PxBoxGeometry geo;
		shape->getBoxGeometry(geo);
		vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
		Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
	} break;
	case (PxGeometryType::eCAPSULE) :
	{
		PxCapsuleGeometry geo;
		shape->getCapsuleGeometry(geo);
		Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, geo_color, &rot);
	} break;
	case (PxGeometryType::eSPHERE) :
	{
		PxSphereGeometry geo;
		if (shape->getSphereGeometry(geo))
		{
			radius = geo.radius;
		}
		Gizmos::addSphereFilled(actor_position, radius, 16, 16, geo_color, &rot);
	} break;
	case (PxGeometryType::ePLANE) :
	{
		PxPlaneGeometry geo;
		shape->getPlaneGeometry(geo); //TODO: what do I do with the plane geo?
		//PxBounds3 bounds = PxShapeExt::getWorldBounds(*shape, *actor); 
		//vec3 extents(bounds.minimum, 0, bounds.maximum);
		//Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
		Gizmos::addAABBFilled(actor_position, vec3(0.01f, 50, 50), geo_color, &rot);
	} break;
	case (PxGeometryType::eHEIGHTFIELD) :
	{
		PxHeightFieldGeometry geo;
		shape->getHeightFieldGeometry(geo); 
		PxBounds3 bounds = PxShapeExt::getWorldBounds(*shape, *actor);
		//vec3 extents(bounds.minimum, geo.heightField, bounds.maximum);
		vec3 extents(0, geo.heightScale, 0);
		//TODO: fix this up geo.heightField, geo.heightScale, geo.rowScale, geo.columnScale
		Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
		Gizmos::addTri(actor_position, extents, vec3(0, geo.columnScale, 0), geo_color);
	} break;
	}
}

void PhysXComponent::SetupVisualDebugger()
{
	if (m_pPhysics == nullptr) return;

	const char* pHostIp = "127.0.0.1";
	const int port = 5425;
	const int timeout = 100;

	auto connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
	m_pConnection = PxVisualDebuggerExt::createConnection(m_pPhysics->getPvdConnectionManager(), pHostIp, port, timeout, connectionFlags);
}
