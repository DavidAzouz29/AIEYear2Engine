#pragma once
#include "Camera.h"

class OrbitCamera : public Camera
{
public:
	OrbitCamera(glm::vec4 a_v4Perspective);
	virtual ~OrbitCamera();

	//void Enter() {};
	void Update(float fDeltaTime) override;
	void RenderUI() override;
	void Exit() {};

private:
	bool m_isClockwise;
	bool m_hasSpaceBeenPressed;
};

