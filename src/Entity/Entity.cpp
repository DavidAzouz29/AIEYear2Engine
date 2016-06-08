/// <summary>
/// File Name:		Entity.cpp
/// Author: 		David Azouz
/// Date Created: 	18/03/16
/// Date Modified: 	18/03/16
///----------------------------------------------------------
/// Brief: An Entity Class that handles entities/ objects within our scene.
/// viewed: 
/// Singletons using smart pointers http://www.cplusplus.com/forum/general/37113/
/// (Entity) Design Pattern http://gameprogrammingpatterns.com/component.html
/// ***EDIT***
/// - Created: Classes inheriting from Entity	- David Azouz 18/03/16
/// - Classes inheriting from Entity			- David Azouz /03/16 //TODO: date
/// 
/// TODO: View the link above
/// 
/// </summary>
/// ----------------------------------------------------------
#include "Entity\Entity.h"
//#include "Render.h"
//#include "Mesh.h"
//#include "RenderTarget.h"
//#include "MathCollision.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

//#include <Gizmos.h>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;

//--------------------------------------------------------------------------------------
// Default Constructor with Initializer list
//--------------------------------------------------------------------------------------
/*Entity::Entity() :
	m_v3ClearColor(glm::vec3(0.25f)),
	m_v4StartColor(1, 0, 0, 1),
	m_v4EndColor(1, 1, 0, 1),
	/*m_m4Colors[0](m_v4StartColor),
	m_m4Colors[1](m_v4EndColor), * /
	m_bDrawGizmoGrid(true)
{
//TODO: Create Render and Meshes
	m_pRender = std::make_shared<Render>();
	m_pMesh = std::make_shared<Mesh>();
} */


/*GLvoid Entity::Update()
{
} */

//TODO: remove and make pure - no contents
/*GLvoid Entity::Draw(const Camera& m_pCamState)
{
	// For the render target
	glBindFramebuffer(GL_FRAMEBUFFER, m_renderTarget.GetFBO());
	//printf("%d\n", m_pRenderApp->GetSharedPointer());
	glViewport(0, 0, 512, 512); // 265 lower quarter of the texture

	// ----------------------------------------------------------
	glClearColor(m_v3ClearColor.r, m_v3ClearColor.g, m_v3ClearColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	// ----------------------------------------------------------
	glBindVertexArray(m_pMesh->GetVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Draw Captured Objects Here
	bool m_bDrawGizmoGrid = true; //TODO: remove - found in Entity
	if (m_bDrawGizmoGrid)
	{
		// ...for now let's add a grid to the gizmos
		for (GLint i = 0; i < 21; ++i) {
			Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
				i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));

			Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
				i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
		}
	}

	Gizmos::draw(m_pCamState.getProjectionView());
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


