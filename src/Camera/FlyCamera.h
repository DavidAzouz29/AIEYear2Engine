#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	FlyCamera(glm::vec4 a_v4Perspective);
	virtual ~FlyCamera() = default;

	//void Enter() {};
	GLvoid Update(GLfloat fDeltaTime) override;
	GLvoid RenderUI() override;
	GLvoid Exit() override {}
};

