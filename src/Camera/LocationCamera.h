#pragma once
#include "TravelCamera.h"

#define MAX_LOCATIONS 4

class LocationCamera : public TravelCamera
{
public:
	LocationCamera(vec4 a_v4Perspective);
	virtual ~LocationCamera() = default;

	GLvoid Update(GLfloat fDeltaTime) override;
	GLvoid RenderUI() override;
	GLvoid Exit() override {}

	GLvoid NextLocation();

private:
	GLint m_iIterIndex;
};