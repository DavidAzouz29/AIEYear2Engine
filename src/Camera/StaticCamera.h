#pragma once
#include "Camera.h"
#include "gl_core_4_4.h"

class StaticCamera : public Camera
{
public:
	StaticCamera(glm::vec4 a_v4Perspective);
	virtual ~StaticCamera() = default;

	//void Enter() {};
	GLvoid RenderUI() override;
	GLvoid Exit() override {} ;
};

