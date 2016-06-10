#pragma once
#include "Camera.h"

class OrbitCamera : public Camera
{
public:
	OrbitCamera(glm::vec4 a_v4Perspective);
	virtual ~OrbitCamera() = default;

	//void Enter() {};
	GLvoid Update(GLfloat fDeltaTime) override;
	GLvoid RenderUI() override;
	GLvoid Exit() override {}

private:
	bool m_isClockwise;
	bool m_hasSpaceBeenPressed;
};

