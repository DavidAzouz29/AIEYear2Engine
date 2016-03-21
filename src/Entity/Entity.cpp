/// <summary>
/// File Name:		Entity.cpp
/// Author: 		David Azouz
/// Date Created: 	18/03/16
/// Date Modified: 	18/03/16
///----------------------------------------------------------
/// Brief: An Entity Class that handles entities/ objects within our scene.
/// viewed: 
/// Singletons using smart pointers http://www.cplusplus.com/forum/general/37113/
/// 
/// ***EDIT***
/// - Classes inheriting from Entity	- David Azouz 18/03/16
/// 
/// TODO: View the link above
/// 
/// </summary>
/// ----------------------------------------------------------
#include "Entity\Entity.h"
#include "Render.h"
#include "Mesh.h"
#include "Gizmos.h"
#include "MathCollision.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;

//--------------------------------------------------------------------------------------
// Default Constructor with Initializer list
//--------------------------------------------------------------------------------------
Entity::Entity() :
	m_v3ClearColor(glm::vec3(0.25f)),
	m_v4StartColor(1, 0, 0, 1),
	m_v4EndColor(1, 1, 0, 1),
	/*m_m4Colors[0](m_v4StartColor),
	m_m4Colors[1](m_v4EndColor), */
	m_bDrawGizmoGrid(true)
{
	m_pRender = std::make_shared<Render>();
	m_pMesh = std::make_shared<Mesh>();
}

/*bool Entity::Create()
{
	// -----------------------
#pragma region FBX
	// -----------------------
/*	m_pFbx = std::make_shared<FBXFile>();
	//m_pFbx->load("./data/models/stanford/Bunny.fbx");
	//m_pFbx->load("./data/models/soulspear/soulspear.fbx");
	m_pFbx->load("./data/models/characters/Pyro/pyro.fbx", FBXFile::UNITS_METER);
	//m_pFbx->load("./data/models/characters/Pyro/pyro.fbx", m_pFbx->UNITS_METER);
	//FBXLoader(); // Needed if FBX without Animation
	FBXSkeletonLoader();

	CreateOpenGLBuffers(m_pFbx.get()); * /
#pragma endregion
	///------------------------------------------------------
	return false;
} 


GLvoid Entity::Update()
{
} */

GLvoid Entity::Draw()
{
	// For the render target
	glBindFramebuffer(GL_FRAMEBUFFER, m_pMesh->GetFBO());
	//printf("%d\n", m_pRenderApp->GetSharedPointer());
	glViewport(0, 0, 512, 512); // 265 lower quarter of the texture

	// ----------------------------------------------------------
	//vec3 m_v3ClearColor(0.25f); //TODO: remove
	glClearColor(m_v3ClearColor.r, m_v3ClearColor.g, m_v3ClearColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear ImGui
	ImGui_ImplGlfwGL3_NewFrame();
	// ----------------------------------------------------------
	glBindVertexArray(m_pMesh->GetVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Draw Captured Objects Here
	bool m_bDrawGizmoGrid = true; //TODO: remove - found in Entity
	if (m_bDrawGizmoGrid)
	{
		// ...for now let's add a grid to the gizmos
		for (int i = 0; i < 21; ++i) {
			Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
				i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));

			Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
				i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
		}
	}

	Gizmos::draw(m_pCamState->getProjectionView());
	// draw
	// unbind the FBO so that we can render to the back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(m_pRender->GetProgramID());
}

GLvoid Entity::DrawApp()
{
	Gizmos::addSphere(glm::vec3(0, 7, 0), 0.5f, 8, 8, m_v4EndColor);

}

/*GLvoid Entity::Destroy()
{
} */

GLvoid Entity::RenderUI()
{
	ImGui::ColorEdit3("clear color", glm::value_ptr(m_v3ClearColor));
	ImGui::ColorEdit3("Particle Start Colour", glm::value_ptr(m_v4StartColor));
	ImGui::ColorEdit3("Particle End Colour", glm::value_ptr(m_v4EndColor));
	ImGui::Checkbox("Should render Gizmo grid", &m_bDrawGizmoGrid);
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Entity"))
	{
		// Locations in Grid format
		if (ImGui::TreeNode("Locations"))
		{
			// If button 'A' is pressed...
			if (ImGui::Button("Dingo", ImVec2(40, 40)))
			{
				printf("Helllooo Dingo");
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::Button("Pause"))
	{
		printf("Helllooo Pause");
	}

	/*	ImGui::DragFloat("Speed", &m_speed, 0.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat("Timer", &m_fTimer, 0.1f, 0.01f, MAX_LOCATIONS * m_fLengthTime + m_fLengthTime);
		ImGui::DragFloat("Start Time", &m_fStartTime, 0.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat("End Time", &m_fEndTime, 0.1f, 0.01f, (float)INT_MAX);
		ImGui::DragFloat("Length of time", &m_fLengthTime, 0.1f, 0.01f, MAX_LOCATIONS * m_fLengthTime);
		ImGui::DragInt("Iterations", &m_iIter, 0.1f, m_iIter, MAX_LOCATIONS);
		ImGui::DragFloat("Lerped Interpolant", &m_fTravelLerp, 0.1f, 0.01f, 1.0f);
		ImGui::Separator();
		ImGui::DragFloat4("Camera Location", m_transform[3].data, 1.1f, -(float)INT_MAX, (float)INT_MAX);
		ImGui::DragFloat4("Location 1", glm::value_ptr(v4Location1), 1.1f, -(float)INT_MAX, (float)INT_MAX);
		ImGui::DragFloat4("Location 2", glm::value_ptr(v4Location2), 1.1f, -(float)INT_MAX, (float)INT_MAX);
		ImGui::DragFloat4("Location 3", v4Location3.data, 1.1f, -(float)INT_MAX, (float)INT_MAX);
		ImGui::DragFloat4("Location 4", v4Location4.data, 1.1f, -(float)INT_MAX, (float)INT_MAX); */
}
