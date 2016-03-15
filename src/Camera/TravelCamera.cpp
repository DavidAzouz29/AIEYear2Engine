/// ----------------------------------------------------------
/// viewed: http://docs.unity3d.com/ScriptReference/Vector3.Lerp.html
/// http://stackoverflow.com/questions/191757/c-concatenate-string-and-int
/// if (ImGui::Selectable("Location ", &selected[i], 0, ImVec2(50, 50)))
/// glm::slerp(v4Location1, v4Location2, fFracJourney);
/// printf("Lerp: %f \n", travelLerp);
/// ----------------------------------------------------------

#define GLM_SWIZZLE
#include "TravelCamera.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <string>

#define MAX_COLS 4

TravelCamera::TravelCamera(glm::vec4 a_v4Perspective) :
	v4LocationData(15),
	v4Location1(10),
	v4Location2(20, 5, 20, 0),
	v4Location3(v4LocationData),
	v4Location4(-20, 5, 20, 0),
	m_v4StartLocation(v4Location1),
	m_v4EndLocation(v4Location2),
	m_fTimer(0),
	m_fStartTime(0),
	m_fLengthTime(10),
	m_fEndTime(m_fLengthTime),
	m_fTravelLerp(0.0f),
	m_iIter(0)
{
	setPerspective(m_v4Perspective.x, m_v4Perspective.y, m_v4Perspective.z, m_v4Perspective.w);
	setLookAtFrom(v4Location1.xyz(), glm::vec3(0));
	
	m_v4LocationArray[0] = v4Location1;
	m_v4LocationArray[1] = v4Location2;
	m_v4LocationArray[2] = v4Location3;
	m_v4LocationArray[3] = v4Location4;
}

TravelCamera::~TravelCamera()
{
}

/// ----------------------------------------------------------
/// Enter: Used to reset values upon entering or return.
/// ----------------------------------------------------------
void TravelCamera::Enter()
{
	// Call our Enter func. of our Camera to attain its properties.
	Camera::Enter();
	// 
	if (m_fTimer != 0)
	{
		m_fTimer = 0;
		m_fStartTime = 0.0f;
		m_iIter = 0;
		//m_fLengthTime = 10;
		m_fEndTime = m_fLengthTime;
	}
}

void TravelCamera::Update(GLfloat fDeltaTime)
{
	m_fTimer += fDeltaTime;

	float frameSpeed = glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? fDeltaTime * m_speed * 2 : fDeltaTime * m_speed;

	// ---------------------------------------------------------------------------------------
	/// <summary><example><code>
	/// if (m_fTimer >= m_fStartTime && m_fTimer < m_fEndTime)
	/// {
	/// 	m_fTravelLerp = (m_fTimer - m_fStartTime) / (m_fEndTime - m_fStartTime);
	/// 	
	/// 	m_transform[3] = glm::lerp(v4Location1, v4Location2, m_fTravelLerp);
	/// 	setLookAtFrom(m_transform[3].xyz(), glm::vec3(0));
	/// }
	/// </code></example></summary> 
	m_v4StartLocation = m_v4LocationArray[m_iIter];
	m_v4EndLocation = m_v4LocationArray[m_iIter + 1];

	// Travels to a location, from a location in a designated time frame
	// View comments above function for more information
	Travel(m_fStartTime, m_fEndTime, m_v4StartLocation, m_v4EndLocation);

	// ---------------------------------------------------------------------------------------
	if (glfwGetKey(m_pWindow, GLFW_KEY_SPACE))
	{
		Enter();
	}

	/// ----------------------------------------------------------
	// Resets to start of Lerping process
	/// ----------------------------------------------------------
	// If the time is greater than the number of locations
	// ... multiplied by our length of time plus an 
	// extra Length of Time needed for the return
	// ...home from our last location to location A.
	/// ----------------------------------------------------------
	if (m_fTimer >= MAX_LOCATIONS * m_fLengthTime + m_fLengthTime) // + 0.1f
	{
		// then reset
		m_fTimer = 0.0f;
		m_fTravelLerp = (m_fTimer - m_fStartTime) / (m_fEndTime - m_fStartTime);
		m_fStartTime = 0.0f;
		m_iIter = 0;
		m_fLengthTime = m_fLengthTime;
		m_fEndTime = m_fLengthTime;
	}
}

/// --------------------------------------------------------------------------------------------------
/// <summary> Travels via Linear interpolation (Lerp) to designated locations
/// <param><para>P1 + P2: Time frame to Lerp between Location 'A' and 'B'.</para>
/// <para>After time is up, we will Lerp from Location 'B' to 'C' etc.</para>
/// <para>At the end of our loop, we will Lerp from 'C'/ last location back to 'A' etc.</para></param>
/// <param><para>P3: Lerp From.</para></param>
/// <param><para>P4: Lerp To (Can be changed during runtime).</para></param>
/// </summary>
/// --------------------------------------------------------------------------------------------------
void TravelCamera::Travel(GLfloat a_fStartTime, GLfloat a_fEndTime, vec4 a_v4StartLocation, vec4 a_v4EndLocation)
{
	// If within the desired time frame...
	if (m_fTimer >= a_fStartTime && m_fTimer < a_fEndTime)
	{
		/// ----------------------------------------------------------
		/// <summary>
		/// <param><para>P1: 5 - 10 = 5.</para></param>
		/// <param><para>P2: 20 - 10 = 10.</para></param>
		/// <param><para>P1 / P2 = x.</para></param>
		/// <param><para> 5 / 10 = 0.5.</para></param>
		/// <returns> Normalised value used as an interpolant.</returns>
		/// </summary>
		/// ----------------------------------------------------------
		m_fTravelLerp = (m_fTimer - a_fStartTime) / (a_fEndTime - a_fStartTime);
		// ... Lerp between our designated locations.
		m_transform[3] = glm::lerp(a_v4StartLocation, a_v4EndLocation, m_fTravelLerp);
		setLookAtFrom(m_transform[3].xyz(), glm::vec3(0));
	}
	else
	{
		//... else skip to the next time frame.
		m_fStartTime += m_fLengthTime;
		m_fEndTime += m_fLengthTime;
		if (m_iIter < MAX_LOCATIONS)
		{
			m_iIter++;
		}
		else
		{
			m_iIter = 0;
		}
	}
}

/// -----------------------------
// Renders UI elements via ImGui
/// -----------------------------
void TravelCamera::RenderUI()
{
	if (ImGui::CollapsingHeader("Travel Camera"))
	{
		// Locations in Grid format
		if (ImGui::TreeNode("Locations"))
		{
			//static bool selected[MAX_LOCATIONS] = { true, false, false, false };
			for (int i = 0; i < MAX_LOCATIONS; i++)
			{
				ImGui::PushID(i); // TODO: better way to do this?
				std::string sLocationName = "Location ";
				sLocationName = sLocationName + std::to_string(i + 1);
				m_cLocationName[i] = sLocationName.c_str();
				// If button 'A' is pressed...
				if (ImGui::Button(m_cLocationName[i], ImVec2(80, 40)))
				{
					//int x = i % MAX_COLS, y = i / MAX_COLS;
					// ... go to location 'A'
					m_v4EndLocation = m_v4LocationArray[i];
					/*if (selected[i] == 0)
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
					}*/
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
			m_transform[3] = m_transform[3];
		}

		ImGui::DragFloat("Speed", &m_speed, 0.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat("Timer", &m_fTimer, 0.1f, 0.01f, MAX_LOCATIONS * m_fLengthTime + m_fLengthTime);
		ImGui::DragFloat("Start Time", &m_fStartTime, 0.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat("End Time", &m_fEndTime, 0.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat("Length of time", &m_fLengthTime, 0.1f, 0.01f, MAX_LOCATIONS * m_fLengthTime);
		ImGui::DragInt("Iterations", &m_iIter, 0.1f, m_iIter, MAX_LOCATIONS);
		ImGui::DragFloat("Lerped Interpolant", &m_fTravelLerp, 0.1f, 0.01f, 1.0f);
		ImGui::Separator();
		ImGui::DragFloat4("Camera Location", m_transform[3].data, 1.1f, -(float)INT_MAX, (float)INT_MAX);
		ImGui::DragFloat4("Location 1", v4Location1.data, 1.1f, -(float)INT_MAX, (float)INT_MAX);
		ImGui::DragFloat4("Location 2", v4Location2.data, 1.1f, -(float)INT_MAX, (float)INT_MAX);
		ImGui::DragFloat4("Location 3", v4Location3.data, 1.1f, -(float)INT_MAX, (float)INT_MAX);
		ImGui::DragFloat4("Location 4", v4Location4.data, 1.1f, -(float)INT_MAX, (float)INT_MAX);
	}
}