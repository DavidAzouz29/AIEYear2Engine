/// <summary>
/// File Name:		TestApplication.cpp
/// Author: 		David Azouz
/// Date Created: 	29/02/16
/// Date Modified: 	8/03/16
///----------------------------------------------------------
/// Brief: A TestApplication Class that Updates and Draws
/// Source: https://github.com/DavidAzouz29/AIEYear2Engine
/// Lib and related files: https://drive.google.com/folderview?id=0B1wViLeuTDL8TF9feHl2RThFR0E&usp=sharing
/// viewed: 
/// Invoke http://en.cppreference.com/w/cpp/utility/functional/invoke
/// Vector Insert http://www.cplusplus.com/reference/vector/vector/insert/
/// 
/// ***EDIT***
/// - Render Targets Working	- David Azouz 7/03/16
/// - Camera States implemented	- David Azouz 7/03/16
/// - FBX Animation working	 	- David Azouz 8/03/16
/// - Removed m_pCamera and replaced with m_pCameraStateMachine
/// - Set up Entity/ Game Object system	- David Azouz 18/03/16
/// 
/// TODO: 
/// Invoke and cycle camera
/// Fix this whole pRender & m_pRender business
/// </summary>
/// ----------------------------------------------------------
#include "TestApplication.h"
#include "Entity\Entity.h"
#include "Entity\FBXModel.h"
#include "Camera\CameraStateMachine.h"
#include "Camera\Camera.h"
#include "Render.h"// | TODO: remove if Draw is removed?
#include "Mesh.h"  // | TODO: remove if Draw is removed?
#include "RenderTarget.h"
#include "ParticleEmitter.h"
#include "GPUParticleEmitter.h"
#include "MathCollision.h"
#include "Gizmos.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp> //needed for pi

#include <assert.h>

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

using glm::vec3;
using glm::vec4;
using glm::mat4;

//--------------------------------------------------------------------------------------
// Default Constructor with Initializer list
//--------------------------------------------------------------------------------------
TestApplication::TestApplication() : 
	m_eCurrentDrawState(E_DRAW_STATE_FILL),
	//m_pRenderApp(nullptr),
	m_fPrevTime(0)
{
	//m_pRenderApp = std::make_shared<Render>();
	// Adds our inherited classes to the vector.
	//m_entities.push_back(std::make_shared<Render>());
	m_entities.push_back(std::make_shared<FBXModel>());
	m_entities.push_back(std::make_shared<ParticleEmitter>());
	m_entities.push_back(std::make_shared<GPUParticleEmitter>());

	m_pMath = std::make_shared<MathCollision>();

	//m_entities.resize(m_entities.size()); //TODO: remove?

} //: m_pCamera(nullptr),

TestApplication::~TestApplication() {}

bool TestApplication::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic shapes
	Gizmos::create();

	// create a camera - may need later
	//m_pCamera = std::make_shared<Camera>(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	//m_pCamera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	/// <summary>
	/// Camera Perspective
	/// <para>P1: FoVY: Field of View Y.</para>
	/// <para>P2: Aspect Ratio.</para>
	/// <para>P3: Near: nearest clipping plane to render to.</para>
	/// <para>P4: Far: furthest clipping plane to render to.</para>
	/// </summary>
	vec4 v4Perspective(glm::pi<float>() * 0.25f, 16 / 9.0f, 0.1f, 10000.f);
	m_pCameraStateMachine = std::make_shared<CameraStateMachine>(v4Perspective);

	m_pCamState = m_pCameraStateMachine->GetCurrentCamera();
	// Repeating code: TODO: change
	/*if (m_pCamState == nullptr)
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_FLYCAMERA);
	}*/
	//////////////////////////////////////////////////////////////////////////
	// YOUR STARTUP CODE HERE
	// -----------------------
	//Entity::CreateSingleton();

	// Loops through each entity and calls their respected Create functions.
	for (auto &pEntity : m_entities)
	{
		pEntity->Create();
		//Entity::GetSingleton()->Create();
	}
	m_pRenderTarget = std::make_shared<RenderTarget>();

	//////////////////////////////////////////////////////////////////////////
	m_pickPosition = glm::vec3(0);

	return true;
}

GLvoid TestApplication::shutdown() 
{
	//////////////////////////////////////////////////////////////////////////
	// YOUR SHUTDOWN CODE HERE
	// -----------------------
	for (auto pEntity : m_entities)
	{
		pEntity->Destroy();
	}
	//////////////////////////////////////////////////////////////////////////

	// delete our camera and cleanup gizmos
	//delete m_pCamera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool TestApplication::Update(float deltaTime) 
{
	// close the application if the window closes
	if (glfwWindowShouldClose(m_pWindow) ||
		glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}

	// Camera Mode: Static, FlyCamera, Orbit
#pragma region Camera Mode
	m_fPrevTime += deltaTime;
	// Camera cycling and lerping // slerping/ squad.
	// Cycles between various Cameras during run time
	if (glfwGetKey(m_pWindow, GLFW_KEY_GRAVE_ACCENT) || glfwGetKey(m_pWindow, GLFW_KEY_KP_0))
	{
		//std::invoke(
		E_CAMERA_MODE_STATE eCurrentCameraMode = m_pCameraStateMachine->GetCurrentCameraMode();
		if (m_fPrevTime > 5)
		{
			eCurrentCameraMode = (E_CAMERA_MODE_STATE)(eCurrentCameraMode + 1);
			if (eCurrentCameraMode > E_CAMERA_MODE_STATE_COUNT - 1)
			{
				eCurrentCameraMode = (E_CAMERA_MODE_STATE)0;
			}
			m_pCameraStateMachine->ChangeState((E_CAMERA_MODE_STATE)(eCurrentCameraMode));
			m_fPrevTime = 0;
		}
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_1) || glfwGetKey(m_pWindow, GLFW_KEY_KP_1))
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_STATIC);
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_2) || glfwGetKey(m_pWindow, GLFW_KEY_KP_2))
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_FLYCAMERA);
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_3) || glfwGetKey(m_pWindow, GLFW_KEY_KP_3))
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_ORBIT);
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_4) || glfwGetKey(m_pWindow, GLFW_KEY_KP_4))
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_TRAVEL);
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_5) || glfwGetKey(m_pWindow, GLFW_KEY_KP_5))
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_LOCATION);
	}

#pragma endregion

	// update the camera's movement
	m_pCameraStateMachine->Update(deltaTime);
	// clear the gizmos out for this frame
	Gizmos::clear();

	//TODO: ImGui here?

	//////////////////////////////////////////////////////////////////////////
	// YOUR UPDATE CODE HERE
	for (auto pEntity : m_entities)
	{
		pEntity->Update();
	}
	// FBX Skeleton and Animation
	//FBXUpdate();

	m_pCamState = m_pCameraStateMachine.get()->GetCurrentCamera();
	m_pMath->Update(m_pCamState);

/*	m_pMath->Update(m_pCamState);
	///----------------------------------------------------------
	// Particles
	//m_pParticleEmitterA->update(deltaTime, m_pCamera->getTransform());
	m_pParticleEmitterA->update(deltaTime, m_pCamState->getTransform());
	m_pParticleEmitterB->update(deltaTime, m_pCamState->getTransform()); */
	//////////////////////////////////////////////////////////////////////////

	// an example of mouse picking
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		double x = 0, y = 0;
		glfwGetCursorPos(m_pWindow, &x, &y);

		// plane represents the ground, with a normal of (0,1,0) and a distance of 0 from (0,0,0)
		glm::vec4 plane(0, 1, 0, 0);
		m_pickPosition = m_pCamState->pickAgainstPlane((float)x, (float)y, plane);
	}
	Gizmos::addTransform(glm::translate(m_pickPosition));

	// Draw Mode: Filled, Poly, Dot
#pragma region Choose Draw state
	if (glfwGetKey(m_pWindow, GLFW_KEY_0))
	{
		m_eCurrentDrawState = E_DRAW_STATE_FILL;
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_9))
	{
		m_eCurrentDrawState = E_DRAW_STATE_POLY;
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_8))
	{
		m_eCurrentDrawState = E_DRAW_STATE_DOT;
	}
	// Cycle Draw State
	if (glfwGetKey(m_pWindow, GLFW_KEY_MINUS))
	{
		if (m_eCurrentDrawState < 0)
		{
			m_eCurrentDrawState = (E_DRAW_STATE)(E_DRAW_STATE_COUNT - 1);
		}
		//m_eCurrentDrawState--;
		m_eCurrentDrawState = (E_DRAW_STATE)(m_eCurrentDrawState - 1);
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_EQUAL)) // == GLFW_RELEASE
	{
		//m_eCurrentDrawState++;
		if (m_eCurrentDrawState > E_DRAW_STATE_COUNT)
		{
			m_eCurrentDrawState = (E_DRAW_STATE)0;
		}
		m_eCurrentDrawState = (E_DRAW_STATE)(m_eCurrentDrawState + 1);
	}
#pragma endregion

	// return true, else the application closes
	return true;
}

GLvoid TestApplication::Draw()
{
/*	// For the render target
	glBindFramebuffer(GL_FRAMEBUFFER, m_pRenderApp->GetSharedPointer()->GetFBO());
	//printf("%d\n", m_pRenderApp->GetSharedPointer());
	glViewport(0, 0, 512, 512); // 265 lower quarter of the texture

	// ----------------------------------------------------------
	glClearColor(m_v3ClearColor.r, m_v3ClearColor.g, m_v3ClearColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear ImGui
	ImGui_ImplGlfwGL3_NewFrame();
	// ----------------------------------------------------------
	glBindVertexArray(m_pRenderApp->GetSharedPointer()->GetVAO());
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
	glUseProgram(m_pRenderApp->GetProgramID()); */
	DrawApp();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
	//Render ImGui over everything
	ImGui::Render();
}

GLvoid TestApplication::DrawApp()
{
	// Change background colour based off Camera mode
	switch (m_pCameraStateMachine->GetCurrentCameraMode())
	{
	case E_CAMERA_MODE_STATE_STATIC:
	{
		glClearColor(0.25f, 0.55f, 0.75f, 1);
		break;
	}
	case E_CAMERA_MODE_STATE_FLYCAMERA:
	{
		glClearColor(0.55f, 0.75f, 0.25f, 1);
		break;
	}
	case E_CAMERA_MODE_STATE_ORBIT:
	{
		glClearColor(0.75f, 0.25f, 0.55f, 1);
		break;
	}
	case E_CAMERA_MODE_STATE_TRAVEL:
	{
		glClearColor(0.75f, 1.0f, 0.55f, 1);
		break;
	}
	case E_CAMERA_MODE_STATE_LOCATION:
	{
		glClearColor(0.55f, 0.75f, 1.0f, 1);
		break;
	}
	default:
	{
		glClearColor(0.25f, 0.25f, 0.25f, 1);
		//TODO: new Cam//glClearColor(1.0f, 0.55f, 0.75f, 1);
		//glClearColor(0.75f, 0.75f, 0.75f, 1);
		break;
	}
	}

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 1280, 720);
	// use our texture program

	//glm::mat4 projView = m_pCameraStateMachine->GetCurrentCamera()->getProjectionView();
	glm::mat4 projView = m_pCamState->getProjectionView();

	// Render Target
	// bind the camera
	int loc = glGetUniformLocation(m_render.GetProgramID(), "ProjectionView"); //m_program_ID
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projView));

	// Set texture slots
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_pRenderApp->GetSharedPointer()->GetFboTexture());
	glBindTexture(GL_TEXTURE_2D, m_pRenderTarget->GetFboTexture());

	// tell the shader where it is
	int diffLoc = glGetUniformLocation(m_render.GetProgramID(), "diffuse"); // m_program_ID
	glUniform1i(diffLoc, 0);

	// Rendering mode
	switch (m_eCurrentDrawState)
	{
	case E_DRAW_STATE_FILL:
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
		break;
	}
	case E_DRAW_STATE_POLY:
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Poly
		break;
	}
	case E_DRAW_STATE_DOT:
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // Dot
		break;
	}
	default:
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
		break;
	}
	}

	//////////////////////////////////////////////////////////////////////////
	// DRAW YOUR THINGS HERE
	// 
	//pRender->DrawTexture(m_pCamera.get());
	//pRender->DrawTexture(m_pCamState); //TODO: needed for Soulspear

	// TODO:
	for (auto pEntity : m_entities)
	{
		pEntity->Draw();
	}

	glBindVertexArray(m_mesh.GetVAO()); // mesh Vertex Array Object
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	m_render.DrawTextureP(m_pCamState); // TODO: FBX Texture - Needed for Render Target
	
	// Old draw items
/*	//Gizmos::addSphere(glm::vec3(0, 7, 0), 0.5f, 8, 8, m_v4EndColor);
	//
	//m_pVertexColoredGrid->draw(projView);
	//m_pSpriteSheetQuad->draw(projView);
	//m_pFBXMesh->draw(projView);

	// FBX
	////RenderFBX(m_pCamState); // Need this for FBX
	//FBXDraw(); //TODO: this
	////FBXSkeletonRender();

	// Particles
//	m_pParticleEmitterA->draw(projView);
//	m_pParticleEmitterB->draw(projView);

	// GPU Particles
	m_pGPUEmitter.get()->Draw((GLfloat)glfwGetTime(),
		m_pCamState->getTransform(),
		m_pCamState->getProjectionView()); */
	//////////////////////////////////////////////////////////////////////////

	//draw our meshes, or gizmos, to the render target
	// display the 3D gizmos
	Gizmos::draw(projView);

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_pWindow, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(projView);

#pragma region ImGui
#pragma region My Rendering Options
	ImGui::ShowTestWindow();
	ImGui::Begin("My rendering options");
		// Camera State
		ImGui::TextWrapped("Camera Mode");

		//static int selected_camera = -1;
		static E_CAMERA_MODE_STATE selected_camera = m_pCameraStateMachine->GetCurrentCameraMode();
		//const char* c_camera = "Camera"; //TODO:
		const char* names[] = { "Static ", "Fly ", "Orbit ", "Travel ", "Location "};

		if (ImGui::Button("Select.."))
			ImGui::OpenPopup("select");
		ImGui::SameLine();
		ImGui::Text(selected_camera == (E_CAMERA_MODE_STATE)-1 ? "<None>" : names[(int)selected_camera]);
		if (ImGui::BeginPopup("select"))
		{
			ImGui::Text("Modes: ");
			ImGui::Separator();
			for (int i = 0; i < IM_ARRAYSIZE(names); i++)
			{
				if (ImGui::Selectable(names[i]))
				{
					selected_camera = (E_CAMERA_MODE_STATE)i;
					m_pCameraStateMachine->ChangeState(selected_camera);
				}
			}
			ImGui::EndPopup();
		}

		///-----------------------------------------------------------------------------------------------------------
		//static int e = 0; // E_CAMERA_MODE_STATE 
		//TODO: Make radio buttons select state
		static int iCurrentCameraMode = m_pCameraStateMachine->GetCurrentCameraMode();
		ImGui::RadioButton(names[0], &iCurrentCameraMode, 0); ImGui::SameLine();
		ImGui::RadioButton(names[1], &iCurrentCameraMode, 1); ImGui::SameLine();
		ImGui::RadioButton(names[2], &iCurrentCameraMode, 2); ImGui::SameLine();
		ImGui::RadioButton(names[3], &iCurrentCameraMode, 3); ImGui::SameLine();
		ImGui::RadioButton(names[4], &iCurrentCameraMode, 4);

		if (ImGui::Button("Camera State"))
		{
			E_CAMERA_MODE_STATE eCurrentCameraMode = m_pCameraStateMachine->GetCurrentCameraMode();
			eCurrentCameraMode = (E_CAMERA_MODE_STATE)(eCurrentCameraMode + 1);
			if (eCurrentCameraMode > E_CAMERA_MODE_STATE_COUNT - 1)
			{
				eCurrentCameraMode = (E_CAMERA_MODE_STATE)0;
			}
			m_pCameraStateMachine->ChangeState((E_CAMERA_MODE_STATE)(eCurrentCameraMode));
		}

		// Each Camera specific UI attributes
		m_pCamState->RenderUI();
		ImGui::Separator();

		// GPU Particles
		//m_pGPUEmitter.get()->RenderUI();
		for (auto pEntity : m_entities)
		{
			pEntity->RenderUI();
			ImGui::Separator();
		}

	ImGui::End();
#pragma endregion

#pragma region Overlay

	// Overlay FPS 
	static bool show_app_fixed_overlay = true;
	if (glfwGetKey(m_pWindow, GLFW_KEY_F1))
	{
		show_app_fixed_overlay = !show_app_fixed_overlay;		
	}
	if (show_app_fixed_overlay)
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		if (!ImGui::Begin("Example: Fixed Overlay", &show_app_fixed_overlay, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return;
		}
		ImGui::Text("Simple overlay\non the top-left side of the screen.");
		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("%d vertices, %d indices (%d triangles)", ImGui::GetIO().MetricsRenderVertices, ImGui::GetIO().MetricsRenderIndices, ImGui::GetIO().MetricsRenderIndices / 3);
		ImGui::Text("%d allocations", ImGui::GetIO().MetricsAllocs);
		ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
		ImGui::End();
		// for FPS and so on
		//ImGui::ShowMetricsWindow(&show_app_fixed_overlay);
	}
#pragma endregion
#pragma endregion

	///-----------------------------------------------------------------------------------------------------------
}

#pragma region FBX
/// ----------------------------------------------------------
/* void TestApplication::FBXRender()
{
	glUseProgram(m_FBX_program_ID);

	int loc = glGetUniformLocation(m_FBX_program_ID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m_pCameraStateMachine->GetCurrentCamera()->getProjectionView()));

	/*int light_dir_uniform = glGetUniformLocation(m_programID, "LightDir");
	glUniform3f(light_dir_uniform, 0, 1, 0); //* /

	vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_FBX_program_ID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	int light_colour_uniform = glGetUniformLocation(m_FBX_program_ID, "LightColour");
	glUniform3f(light_colour_uniform, 1, 1, 1);

	mat4 camera_matrix = m_pCameraStateMachine->GetCurrentCamera()->getTransform();
	int camera_pos_uniform = glGetUniformLocation(m_FBX_program_ID, "CameraPos");
	glUniform3f(camera_pos_uniform, camera_matrix[3][0], camera_matrix[3][1], camera_matrix[3][2]);

	int specular_uniform = glGetUniformLocation(m_FBX_program_ID, "SpecPow");
	glUniform1f(specular_uniform, 12);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pRender->GetTextureByName("Pyro_D")); // m_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pRender->GetTextureByName("Pyro_N")); // m_normal);

	loc = glGetUniformLocation(m_FBX_program_ID, "diffuse");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(m_FBX_program_ID, "normal");
	glUniform1i(loc, 1);

	FBXSkeleton* skeleton = m_pFbx->getSkeletonByIndex(0);
	skeleton->updateBones();

	int bones_location = glGetUniformLocation(m_FBX_program_ID, "bones");
	glUniformMatrix4fv(bones_location, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);

	// bind our vertex array object and draw the mesh
	for (unsigned int i = 0; i < m_pFbx->getMeshCount(); ++i) {
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;
		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES,
			(unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
} */
#pragma endregion