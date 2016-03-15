#pragma once
#include "TravelCamera.h"

#define MAX_LOCATIONS 4

class LocationCamera : public TravelCamera
{
public:
	LocationCamera(vec4 a_v4Perspective);
	virtual ~LocationCamera();

	void Update(GLfloat fDeltaTime) override;
	void RenderUI() override;
	void Exit() {};

	void NextLocation();

private:
	GLint m_iIterIndex;
};