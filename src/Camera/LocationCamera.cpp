/// <summary>
/// File Name:		LocationCamera.cpp
/// Author: 		David Azouz
/// Date Created: 	13/03/16
/// Date Modified: 	13/03/16
///----------------------------------------------------------
/// Brief: A TestApplication Class that Updates and Draws
/// viewed: http://docs.unity3d.com/ScriptReference/Vector3.Lerp.html
/// http://stackoverflow.com/questions/191757/c-concatenate-string-and-int
/// 
/// ***EDIT***
/// - Render Targets Working	- David Azouz 7/03/16
/// - Camera States implemented	- David Azouz 7/03/16
/// - FBX Animation working	 	- David Azouz 8/03/16
/// - Camera Cycle implemented	- David Azouz /03/16
/// 
/// TODO: 
/// 
/// </summary>
/// ----------------------------------------------------------
/// if (ImGui::Selectable("Location ", &selected[i], 0, ImVec2(50, 50)))
/// ----------------------------------------------------------

#define GLM_SWIZZLE
#include "LocationCamera.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <string>

#define MAX_COLS 4

LocationCamera::LocationCamera(glm::vec4 a_v4Perspective) : m_iIterIndex(0)
/*	v4LocationData(15),
	v4Location1(10),
	v4Location2(20, 5, 20, 0),
	v4Location3(v4LocationData),
	v4Location4(-20, 5, 20, 0),
	m_v4StartLocation(v4Location1),
	m_v4EndLocation(v4Location2),
	m_iIter(0) */
{
	setPerspective(a_v4Perspective.x, a_v4Perspective.y, a_v4Perspective.z, a_v4Perspective.w);
	//setLookAtFrom(v4Location1.xyzw(), glm::vec3(0));

	v4LocationData = vec4(36, 71, 36, 0);
	v4Location1 = vec4(10);
	v4Location2 = vec4(20, 5, 20, 0);
	v4Location3 = vec4(v4LocationData);
	v4Location4 = vec4(-20, 5, 20, 0);

	m_v4LocationArray[0] = v4Location1;
	m_v4LocationArray[1] = v4Location2;
	m_v4LocationArray[2] = v4Location3;
	m_v4LocationArray[3] = v4Location4;
}

GLvoid LocationCamera::Update(GLfloat fDeltaTime)
{
	GLfloat frameSpeed = glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? fDeltaTime * m_speed * 2 : fDeltaTime * m_speed;
	// ---------------------------------------------------------------------------------------
	// TODO: update our locations via ImGui
	for (GLint i = 0; i < MAX_LOCATIONS; ++i)
	{
		m_v4LocationArray[i] = m_v4LocationArray[i];
	}
	//m_transform[3] = m_v4LocationArray[m_iIter]; //TODO: set from to m_transform[3]?
	setLookAtFrom(m_v4LocationArray[m_iIter].xyz(), glm::vec3(0));
	// ---------------------------------------------------------------------------------------
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE))
	{
		NextLocation();
	}
}

/// -----------------------------
// Renders UI elements via ImGui
/// -----------------------------
GLvoid LocationCamera::RenderUI()
{
	if (ImGui::CollapsingHeader("Location Camera"))
	{
		// Locations in Grid format
		if (ImGui::TreeNode("Locations"))
		{
			//static bool selected[MAX_LOCATIONS] = { true, false, false, false };
			// TODO: this code is being repeated from TravelCamera - make func?
			for (int i = 0; i < MAX_LOCATIONS; i++)
			{
				ImGui::PushID(i); // TODO: better way to do this?
				std::string sLocationName = "Location ";
				sLocationName = sLocationName + std::to_string(i + 1);
				m_cLocationName[i] = sLocationName.c_str();
				if (ImGui::Button(m_cLocationName[i], ImVec2(80, 40)))
				{
					//int x = i % MAX_COLS, y = i / MAX_COLS;
					m_iIter = i;
					m_transform[3] = m_v4LocationArray[m_iIter];
				}
				if ((i % MAX_COLS) < 3)
				{
					ImGui::SameLine();
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		if (ImGui::Button("Pause"))
		{
			NextLocation();
		}

		ImGui::DragFloat("Speed", &m_speed, 0.1f, 0.01f, (GLfloat)INT_MAX);
		ImGui::Separator();
		ImGui::DragFloat4("Camera Location", m_transform[3].data, 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX);
		ImGui::DragFloat4("Location 1", v4Location1.data, 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX);
		ImGui::DragFloat4("Location 2", v4Location2.data, 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX);
		ImGui::DragFloat4("Location 3", v4Location3.data, 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX);
		ImGui::DragFloat4("Location 4", v4Location4.data, 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX);
	}
}

/// ------------------------------------
// Travels to the next location stored.
/// ------------------------------------
GLvoid LocationCamera::NextLocation()
{
	// If we're within our range...
	if (m_iIter <= MAX_LOCATIONS)
	{
		// ... set our index to our iteration.
		m_iIterIndex++;
	}
	else
	{
		// ... reset to 0 to start again.
		m_iIterIndex = 0;
	}
	setLookAtFrom(m_v4LocationArray[m_iIterIndex].xyz(), glm::vec3(0));
}