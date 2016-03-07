#define GLM_SWIZZLE
#include "OrbitCamera.h"

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>


OrbitCamera::OrbitCamera(glm::vec4 a_v4Perspective) : m_isClockwise(true), m_hasSpaceBeenPressed(false)
	/*, m_speed(10),
	m_fRotation(0.01f),
	m_up(0, 1, 0),
	m_transform(1),
	m_view(1),
	m_v3Movement(0) */
{
	setPerspective(m_v4Perspective.x, m_v4Perspective.y, m_v4Perspective.z, m_v4Perspective.w);
}


OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::Update(float fDeltaTime)
{
	float frameSpeed = glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? fDeltaTime * m_speed * 2 : fDeltaTime * m_speed;
	/// <summary> 
	/// If Any key: Rotates in the opposite direction.
	/// <para> If was rotating Clockwise, it will now rotate Counter-Clockwise. </para>
	/// </summary> 
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS && !m_hasSpaceBeenPressed )
	{
		m_isClockwise = !m_isClockwise;
	}

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS && !m_hasSpaceBeenPressed)
	{
		m_hasSpaceBeenPressed = true;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_RELEASE && m_hasSpaceBeenPressed)
	{
		m_hasSpaceBeenPressed = false;
	}

	// Rotate Clockwise if true...
	if (m_isClockwise)
	{
		m_transform[3] -= m_transform[0] * frameSpeed;
		setLookAtFrom(m_transform[3].xyz, glm::vec3(0));
	}
	// ... else rotate Counter-clockwise
	else
	{
		m_transform[3] += m_transform[0] * frameSpeed;
		setLookAtFrom(m_transform[3].xyz, glm::vec3(0));
	}
#pragma region Orbit

	/*	if (glfwGetKey(m_window, GLFW_KEY_Z) == GLFW_REPEAT) //glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(m_window, GLFW_KEY_A) ||
	{
		glfwGetKey(m_window, GLFW_KEY_C) != GLFW_PRESS;
		m_transform[3] -= m_transform[0] * frameSpeed;
		setLookAtFrom(m_transform[3].xyz, glm::vec3(0));
	}
	if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_REPEAT) //glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(m_window, GLFW_KEY_D) || 
	{
		glfwGetKey(m_window, GLFW_KEY_Z) != GLFW_PRESS;
		m_transform[3] += m_transform[0] * frameSpeed;
		setLookAtFrom(m_transform[3].xyz, glm::vec3(0));
	}
	//TODO: Fix this up
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(m_window, GLFW_KEY_Q))
	{
		if (m_transform[3].x != 0 || m_transform[3].z != 0) //m_transform[3].xz != glm::vec2(0,0)
		{
			m_transform[3].x = 0; // = glm::vec3(0, 1, 0);
			m_transform[3].z = 0;
		}
		glfwGetKey(m_window, GLFW_KEY_Q);
		//m_transform = glm::rotate(m_transform, m_fRotation, glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) && glfwGetKey(m_window, GLFW_KEY_E))
	{
		if (m_transform[3].x != 0 || m_transform[3].z != 0) //m_transform[3].xz != glm::vec2(0,0)
		{
			m_transform[3].x = 0; // = glm::vec4(0, 5, 0, 0);
			m_transform[3].z = 0;
		}
		glfwGetKey(m_window, GLFW_KEY_E);
	} */
#pragma endregion
}