/// viewed: http://docs.unity3d.com/ScriptReference/Vector3.Lerp.html
#define GLM_SWIZZLE
#include "TravelCamera.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

TravelCamera::TravelCamera(glm::vec4 a_v4Perspective) :
	v4LocationData(15),
	v4Location1(10),
	v4Location2(20, 5, 20, 0),
	v4Location3(v4LocationData),
	v4StartPos(v4Location1),
	v4EndPos(v4Location2),
	m_fTimer(0),
	m_fStartTime(0),
	m_fEndTime(10)
{
	setPerspective(m_v4Perspective.x, m_v4Perspective.y, m_v4Perspective.z, m_v4Perspective.w);
	//setLookAtFrom(v4Location1.xyz, glm::vec3(0));
}

TravelCamera::~TravelCamera()
{
}

void TravelCamera::Update(float fDeltaTime)
{
	m_fTimer += fDeltaTime;

	float frameSpeed = glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? fDeltaTime * m_speed * 2 : fDeltaTime * m_speed;

#pragma region Movement
	// ---------------------------------------------------------------------------------------
	//glm::slerp(v4Location1, v4Location2, fFracJourney);
	float travelLerp = (m_fTimer - m_fStartTime) / (m_fEndTime - m_fStartTime);
	//printf("Lerp: %f \n", travelLerp);
	if (m_fTimer >= m_fStartTime && m_fTimer < m_fEndTime)
	{
		m_transform[3] = glm::lerp(v4Location1, v4Location2, travelLerp);
		setLookAtFrom(m_transform[3].xyz(), glm::vec3(0));
	}
	else if (m_fTimer >= m_fStartTime && m_fTimer < m_fEndTime)
	{
		if (m_fTimer >= m_fEndTime)
		{
			m_fStartTime = 10;
			m_fEndTime = 20;
		}
		m_transform[3] = glm::lerp(v4Location2, v4Location3, travelLerp);
		setLookAtFrom(m_transform[3].xyz(), glm::vec3(0));
		if (m_fTimer >= m_fEndTime)
		{
			m_fStartTime = 20;
			m_fEndTime = 30;
		}
	}
	else if (m_fTimer >= m_fStartTime && m_fTimer < m_fEndTime)
	{
		if (m_fTimer >= m_fEndTime)
		{
			m_fStartTime = 0;
			m_fEndTime = 10;
		}
		m_transform[3] = glm::lerp(v4Location3, v4Location1, travelLerp);
		setLookAtFrom(m_transform[3].xyz(), glm::vec3(0));
	}

	/* ---------------------------------------------------------------------------------------
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) || glfwGetKey(m_pWindow, GLFW_KEY_UP)
		|| (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) && glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT)))
	{
		m_transform[3].x += m_transform[1].x * frameSpeed;
		m_transform[3].z += m_transform[1].z * frameSpeed;
	}
	if ((glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) && glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_RIGHT)) ||
		glfwGetKey(m_pWindow, GLFW_KEY_S) || glfwGetKey(m_pWindow, GLFW_KEY_DOWN))
	{
		m_transform[3].x -= m_transform[1].x * frameSpeed;
		m_transform[3].z -= m_transform[1].z * frameSpeed;
	} */
#pragma endregion

	if (m_fTimer >= 30.1f)
	{
		m_fTimer = 0.0f;
		//m_fStartTime = 0.0f;
		//m_fEndTime = 10.0f;
	}
}

void TravelCamera::RenderUI()
{
	if (ImGui::CollapsingHeader("Travel Camera"))
	{
		// Locations
		if (ImGui::TreeNode("Grid"))
		{
			const GLuint MAX_LOCATIONS = 4;
			static bool selected[MAX_LOCATIONS] = { true, false, false, false };
			for (int i = 0; i < MAX_LOCATIONS; i++)
			{
				ImGui::PushID(i);
				//if (ImGui::Selectable("Location ", &selected[i], 0, ImVec2(50, 50)))
				if (ImGui::Button("Location ", ImVec2(70, 70)))
				{
					int x = i % 4, y = i / 4;
					if (selected[i] == 0)
					{
						m_transform[3] = v4Location1;
					}
					else if (selected[i] == 1)
					{
						m_transform[3] = v4Location2;
					}
					else if (selected[i] == 2)
					{
						m_transform[3] = v4Location2;
					}
					else if (selected[i] == 3)
					{
						m_transform[3] = v4Location3;
					}
				}
				if ((i % 4) < 3) ImGui::SameLine();
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		if (ImGui::Button("Planet 1"))
		{
			m_transform[3] = v4Location1;
		}
		else if (ImGui::Button("Planet 2"))
		{
			m_transform[3] = v4Location2;
		}
		else if (ImGui::Button("Planet 3"))
		{
			m_transform[3] = v4Location3;
		}
		if (ImGui::Button("Pause"))
		{
			m_transform[3] = m_transform[3];
		}

		static float vec4f[4] = { v4Location3.x, v4Location3.y, v4Location3.z, v4Location3.w };

		ImGui::DragFloat("Speed", &m_speed, 0.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat("Timer", &m_fTimer, 0.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat3("Camera Location", vec4f, 1.1f, 0.01f, (float)INT_MAX);
		/*ImGui::DragFloat3("Camera Location X", &v4Location3.x, 1.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat3("Camera Location Y", &v4Location3.y, 1.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat3("Camera Location Z", &v4Location3.z, 1.1f, 0.01f, (float)INT_MAX); */
	}
}