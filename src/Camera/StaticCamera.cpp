#include "StaticCamera.h"
#include "imgui.h"

//#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

StaticCamera::StaticCamera(glm::vec4 a_v4Perspective)
{
	setPerspective(m_v4Perspective.x, m_v4Perspective.y, m_v4Perspective.z, m_v4Perspective.w);
}

/// -----------------------------
// Renders UI elements via ImGui
/// -----------------------------
GLvoid StaticCamera::RenderUI()
{
	if (ImGui::CollapsingHeader("Static Camera"))
	{
		ImGui::DragFloat("Speed", &m_speed, 0.1f, 0.01f, (GLfloat)INT_MAX);
		ImGui::InputFloat4("Camera Location", glm::value_ptr(m_transform[3]));// .data);
		ImGui::DragFloat4("Camera Location", glm::value_ptr(m_transform[3]), 1.1f, 0.01f, (GLfloat)INT_MAX);
	}
}