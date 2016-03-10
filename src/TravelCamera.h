#pragma once
#include "Camera.h"
#include "gl_core_4_4.h"

class TravelCamera : public Camera
{
public:
	TravelCamera(glm::vec4 a_v4Perspective);
	virtual ~TravelCamera();

	void Update(float fDeltaTime) override;
	void RenderUI() override;
	void Exit() {};

private:
	GLfloat m_fTimer;
	GLfloat m_fStartTime;
	GLfloat m_fEndTime;

	//TODO: rename to m_
	glm::vec4 v4LocationData;

	glm::vec4 v4Location1;
	glm::vec4 v4Location2;
	glm::vec4 v4Location3;

	glm::vec4 v4StartPos;
	glm::vec4 v4EndPos;
};

