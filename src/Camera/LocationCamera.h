#pragma once
#include "TravelCamera.h"

#define MAX_LOCATIONS 4

class LocationCamera : public TravelCamera
{
public:
	LocationCamera(vec4 a_v4Perspective);
	virtual ~LocationCamera();

	GLvoid Update(GLfloat fDeltaTime) override;
	GLvoid RenderUI() override;
	//GLvoid Exit() {};

	GLvoid NextLocation();

private:
	GLint m_iIterIndex;
};