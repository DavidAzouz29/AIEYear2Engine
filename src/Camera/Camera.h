#pragma once

#include <gl_core_4_4.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class CameraStateMachine;
struct GLFWwindow;

class Camera 
{
public:

	Camera() = default;
	Camera(GLfloat a_fFovY, GLfloat a_fAspectRatio, GLfloat a_fNear, GLfloat a_fFar);
	Camera(glm::vec4 a_v4Perspective);
	virtual ~Camera() = default;

	virtual GLvoid Enter();
	virtual GLvoid Update(GLfloat deltaTime) {};
	virtual GLvoid RenderUI();
	virtual GLvoid Exit() {}; //TODO: = 0;?

	GLvoid	setSpeed(GLfloat speed)		{ m_speed = speed; }
	GLvoid	setRotation(GLfloat a_fRotation)	{ m_fRotation = a_fRotation; }

	GLvoid	setPerspective(GLfloat a_fFovY, GLfloat a_fAspectRatio, GLfloat a_fNear, GLfloat a_fFar);

	GLvoid	setLookAtFrom(const glm::vec3& from, const glm::vec3& to);
	//GLvoid	setLookAtFrom(const glm::vec4& from, const glm::vec4& to);

	const glm::mat4&	getTransform() const		{ return m_transform; }
	const glm::mat4&	getProjection() const		{ return m_projection; }
	const glm::mat4&	getView() const				{ return m_view; }
	const glm::mat4&	getProjectionView() const	{ return m_projectionView; }

	// returns a world-space normalized vector pointing away from the camera's world-space position
	glm::vec3			screenPositionToDirection(GLfloat x, GLfloat y) const;

	// returns the point of intersection of a camera ray and a world-space plane
	// the plane has a normal of XYZ and is offset from (0,0,0) by -W in the direction of the normal
	glm::vec3			pickAgainstPlane(GLfloat x, GLfloat y, const glm::vec4& plane) const;

protected:
	
	GLFWwindow* m_pWindow;

	GLfloat		m_speed;
	GLfloat		m_fRotation;
	glm::vec3	m_up;
	glm::mat4	m_transform;
	glm::mat4	m_projection;
	glm::mat4	m_view;
	glm::mat4	m_projectionView;
	glm::vec3	m_v3Movement;
	glm::vec4	m_v4Perspective;
};