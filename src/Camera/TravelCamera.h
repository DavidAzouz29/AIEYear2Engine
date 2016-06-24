#pragma once
#include "Camera.h"
//#include <gl_core_4_4.h>

#define MAX_LOCATIONS 4

using glm::vec4;

class TravelCamera : public Camera
{
public:
	TravelCamera() = default;
	TravelCamera(vec4 a_v4Perspective);
	virtual ~TravelCamera() = default;

	GLvoid Enter() override;
	GLvoid Update(GLfloat fDeltaTime) override;
	/// --------------------------------------------------------------------------------------------------
	/// <summary> Travels via Linear interpolation (Lerp) to designated locations
	/// <param><para>P1 + P2: Time frame to Lerp between Location 'A' and 'B'.</para>
	/// <para>After time is up, we will Lerp from Location 'B' to 'C' etc.</para>
	/// <para>At the end of our loop, we will Lerp from 'C'/ last location back to 'A' etc.</para></param>
	/// <param><para>P3: Lerp From.</para></param>
	/// <param><para>P4: Lerp To (Can be changed during runtime).</para></param>
	/// </summary>
	/// --------------------------------------------------------------------------------------------------
	GLvoid Travel(GLfloat a_fStartTime, GLfloat a_fEndTime, vec4 a_v4StartLocation, vec4 a_v4EndLocation);
	GLvoid RenderUI() override;
	GLvoid Exit() override {}

protected:
	GLfloat m_fTimer;
	GLfloat m_fStartTime;
	GLfloat m_fLengthTime;
	GLfloat m_fEndTime;
	GLfloat m_fTravelLerp;
	GLint m_iIter;

	//const GLuint MAX_LOCATIONS = 4; //TODO: for C++?
	const GLchar* m_cLocationName[MAX_LOCATIONS];

	//TODO: rename to m_
	// Array of Locations - for button presses
	vec4 m_v4LocationArray[MAX_LOCATIONS];
	// Stores positional information for manipulation later
	vec4 v4LocationData;

	vec4 v4Location1;
	vec4 v4Location2;
	vec4 v4Location3;
	vec4 v4Location4;

	vec4 m_v4StartLocation;
	vec4 m_v4EndLocation;
};

