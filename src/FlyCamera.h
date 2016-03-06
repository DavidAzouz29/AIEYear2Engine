#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	FlyCamera(glm::vec4 a_v4Perspective);
	virtual ~FlyCamera();

	//void Enter() {};
	void Update(float fDeltaTime) override;
	void Exit() {};
};

