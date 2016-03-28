#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	FlyCamera(glm::vec4 a_v4Perspective);
	virtual ~FlyCamera();

	//void Enter() {};
	GLvoid Update(GLfloat fDeltaTime) override;
	//GLvoid Exit() {};
};

