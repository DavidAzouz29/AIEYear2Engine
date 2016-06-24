#pragma once
#include "characterkinematic/PxController.h"

class ControllerHitReport : public physx::PxUserControllerHitReport
{
public:
	// Called when currecnt controller hits a user-defined obstacle
	ControllerHitReport() : physx::PxUserControllerHitReport() {};
	~ControllerHitReport() = default;
	
	virtual void onShapeHit(const physx::PxControllerShapeHit &hit);
	// These handle collision with other controllers and hitting obstacles
	virtual void onControllerHit(const physx::PxControllersHit &hit) {};
	virtual void onObstacleHit(const physx::PxControllerObstacleHit &hit) {};
	
	physx::PxVec3 GetPlayerContactNormal() { return m_playerContactNormal; }
	void ClearPlayerContactNormal() { m_playerContactNormal = physx::PxVec3(0, 0, 0); }
	
private:
	physx::PxVec3 m_playerContactNormal;
};

