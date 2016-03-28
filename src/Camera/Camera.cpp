
#define GLM_SWIZZLE
#include "Camera.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
//#include <string>
//#include <glm/detail/type_vec4.hpp>

Camera::Camera(GLfloat a_fFovY, GLfloat a_fAspectRatio, GLfloat a_fNear, GLfloat a_fFar)
	: m_speed(10),
	m_fRotation(0.01f),
	m_up(0,1,0),
	m_transform(1),
	m_view(1),
	m_v3Movement(0)
{
	setPerspective(a_fFovY, a_fAspectRatio, a_fNear, a_fFar);
	setLookAtFrom(glm::vec3(10, 10, 10), glm::vec3(0));
}

Camera::Camera(glm::vec4 a_v4Perspective)
	: m_speed(10),
	m_fRotation(0.01f),
	m_up(0, 1, 0),
	m_transform(1),
	m_view(1),
	m_v3Movement(0)
{
	m_v4Perspective = a_v4Perspective;
	setPerspective(m_v4Perspective.x, m_v4Perspective.y, m_v4Perspective.z, m_v4Perspective.w);
	setLookAtFrom(glm::vec3(10, 10, 10), glm::vec3(0));
}

GLvoid Camera::setPerspective(GLfloat a_fFovY, GLfloat a_fAspectRatio, GLfloat a_fNear, GLfloat a_fFar)
{
	m_projection = glm::perspective(a_fFovY, a_fAspectRatio, a_fNear, a_fFar);
	m_projectionView = m_projection * m_view;
}

GLvoid Camera::setLookAtFrom(const glm::vec3& from, const glm::vec3& to)
{
	m_view = glm::lookAt(from, to, glm::vec3(0, 1, 0));
	m_transform = glm::inverse(m_view);
	m_projectionView = m_projection * m_view;
}

/*void Camera::setLookAtFrom(const glm::vec4& from, const glm::vec4& to)
{
	m_view = glm::lookAt(from.xyz(), to.xyz(), glm::vec4(0, 1, 0, 0));
	m_transform = glm::inverse(m_view);
	m_projectionView = m_projection * m_view;
} */

GLvoid Camera::Enter()
{
	//TODO: repeated code? ^
	setPerspective(glm::pi<GLfloat>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	if (m_speed != 10)
	{
		m_speed = 10.0f;
		printf("Speed Needed \n");
	}
	else if (m_fRotation != 0.01f)
	{
		m_fRotation = 0.01f;
		printf("Rotation Needed \n");
	}
	else if(m_transform != glm::mat4(1))
	{
		m_transform = glm::mat4(1);
	}
	else if(m_view != glm::mat4(1))
	{
		m_transform = glm::mat4(1);
	}
	else if (m_v3Movement != glm::vec3(0))
	{
		m_v3Movement = glm::vec3(0);
	}
	
	m_up = glm::vec3(0, 1, 0);

	// TODO: delete?
	setLookAtFrom(glm::vec3(10, 10, 10), glm::vec3(0));

	m_pWindow = glfwGetCurrentContext();
}

glm::vec3 Camera::screenPositionToDirection(GLfloat x, GLfloat y) const {
	
	GLint width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projection[0][0];
	screenPos.y /= m_projection[1][1];

	return glm::normalize(m_transform * glm::vec4(screenPos, 0)).xyz();
}

glm::vec3 Camera::pickAgainstPlane(GLfloat x, GLfloat y, const glm::vec4& plane) const {

	GLint width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glm::vec3 screenPos(x / width * 2 - 1, (y / height * 2 - 1) * -1, -1);

	screenPos.x /= m_projection[0][0];
	screenPos.y /= m_projection[1][1];

	glm::vec3 dir = glm::normalize(m_transform * glm::vec4(screenPos, 0)).xyz();

	GLfloat d = (plane.w - glm::dot(m_transform[3].xyz(), plane.xyz()) / glm::dot(dir, plane.xyz()));

	return m_transform[3].xyz() + dir * d;
}

GLvoid Camera::RenderUI()
{
	//std::string sLocationName = "Location "; //TODO: get camera mode? names[] from TestApp

	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::DragFloat("Speed", &m_speed, 0.1f, 0.01f, (GLfloat)INT_MAX);
		ImGui::InputFloat4("Camera Location", m_transform[3].data);
		ImGui::DragFloat4("Camera Location", m_transform[3].data, 1.1f, 0.01f, (GLfloat)INT_MAX);
	}
}