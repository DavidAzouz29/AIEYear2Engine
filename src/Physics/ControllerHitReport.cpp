#include "ControllerHitReport.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>

void ControllerHitReport::onShapeHit(const physx::PxControllerShapeHit &hit)
{
	// gets a ref to a structure which tells us what has been hit and where
	// get the actor from the shape we hit
	physx::PxRigidActor* actor = hit.shape->getActor();
	// get the normal
	m_playerContactNormal = hit.worldNormal;
	// try to cast to a dynamic actor
	physx::PxRigidDynamic* myActor = actor->is<physx::PxRigidDynamic>();

	if (myActor)
	{
		// this is where we can apply forces to things we hit
	}
}
