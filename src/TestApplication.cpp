/// <summary>
/// File Name:		TestApplication.cpp
/// Author: 		David Azouz
/// Date Created: 	29/02/16
/// Date Modified: 	8/03/16
///----------------------------------------------------------
/// Brief: A TestApplication Class that Updates and Draws
/// Source: https://github.com/DavidAzouz29/AIEYear2Engine
/// John's Engine: https://github.com/johnsietsma/RefEngine/blob/102a07439ebf40182c4cab27df3001f0607234cc/Engine/src/Engine.cpp
/// Lib and related files: https://drive.google.com/folderview?id=0B1wViLeuTDL8TF9feHl2RThFR0E&usp=sharing
/// viewed: 
/// Invoke http://en.cppreference.com/w/cpp/utility/functional/invoke
/// Vector Insert http://www.cplusplus.com/reference/vector/vector/insert/
/// Error checking https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
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
/// change to #include <gl_core_4_4.h, <Gizmos.h, and to GLvoid
/// Fix this whole pRender & m_pRender business
/// </summary>
/// ----------------------------------------------------------
#include "TestApplication.h"
#include "Entity\Entity.h"
#include "Entity\FBXModel.h"
#include "Entity\ParticleEmitter.h"
#include "Entity\GPUParticleEmitter.h"
#include "Camera\CameraStateMachine.h"
#include "Camera\Camera.h"
#include "Grid.h"
#include "Mesh.h"  // | TODO: remove if Draw is removed?
#include "RenderTarget.h"
#include "Helpers.h"
#include "MathCollision.h"
#include "Texture.h"
#include "TextureManager.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <iostream>
#include <Gizmos.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp> //needed for pi

#include <assert.h>

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

//struct ParticleEmitterConfig;

using glm::vec3;
using glm::vec4;
using glm::mat4;
using namespace std;

bool TestApplication::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);
	
	TurnOnOpenGLDebugLogging();

	// start the gizmo system that can draw basic shapes
	Gizmos::create();

	// create a camera - may need later
	//m_pCamera = std::make_shared<Camera>(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	//m_pCamera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	/// ------------------------------------------------------------
	/// <summary>
	/// Camera Perspective
	/// ------------------------------------------------------------
	/// <para>P1: FoVY: Field of View Y.</para>
	/// <para>P2: Aspect Ratio.</para>
	/// <para>P3: Near: nearest clipping plane to render to.</para>
	/// <para>P4: Far: furthest clipping plane to render to.</para>
	/// </summary>
	/// ------------------------------------------------------------
	vec4 v4Perspective(glm::pi<GLfloat>() * 0.25f, 16 / 9.0f, 0.1f, 10000.f);
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

	//m_pEntity = std::make_shared<Entity>();
#pragma region CPU Particles Config(s)
	GLuint uiAmount = 20; //TODO: 3000
	ParticleEmitterConfig configA;
	configA.particleCount = uiAmount; //1000 
	configA.emitRate = (float)configA.particleCount / 2; //500
	configA.startColor = glm::vec4(1.56f, 0, 1.25f, 0.8f); //glm::vec4(1, 0, 0, 1); <-RED
	configA.endColor = glm::vec4(0, 0.07f, 0.3f, 1);  //vec4(0, 0, 1, 0.8f); //vec4(0, 0, 1, 0.8f);//vec4(1, 1, 0, 1);
	configA.lifespanMin = 0.1f;
	configA.lifespanMax = 5; //5
	configA.startSize = 0.4f; // 1
	configA.endSize = 0.1f;
	configA.velocityMin = 0.1f;
	configA.velocityMax = 1.0f;
	configA.v3ParticlePosition = glm::vec3(-3, 5, 0);

	//if (!ParticleLoader(configA)) return -4;

	ParticleEmitterConfig configB;
	configB.particleCount = uiAmount; //5000
	configB.emitRate = (float)configB.particleCount / 2; //500
	configB.startColor = glm::vec4(1, 0, 1, 0.8f); //glm::vec4(1, 0, 0, 1); <-RED
	configB.endColor = glm::vec4(0.3, 0, 0.07f, 1);  //vec4(0, 0, 1, 0.8f); //vec4(0, 0, 1, 0.8f);//vec4(1, 1, 0, 1);
	configB.lifespanMin = 0.9f;
	configB.lifespanMax = 3; //5
	configB.startSize = 0.4f; // 1
	configB.endSize = 0.2f;
	configB.velocityMin = 0.1f;
	configB.velocityMax = 2.0f;
	configB.v3ParticlePosition = glm::vec3(3, 5, 0);
#pragma endregion

	//m_pEntity = std::make_unique<Entity>();
	// Adds our inherited classes to the vector.
	m_entities.push_back(std::make_shared<Grid>());
	m_entities.push_back(std::make_shared<FBXModel>("./data/models/soulspear/soulspear.fbx"));
	m_entities.push_back(std::make_shared<FBXModel>("./data/models/characters/Pyro/pyro.fbx"));
	m_entities.push_back(std::make_shared<ParticleEmitter>(configA));
	m_entities.push_back(std::make_shared<ParticleEmitter>(configB));
	m_entities.push_back(std::make_shared<GPUParticleEmitter>());

	m_pMath = std::make_shared<MathCollision>();

	// Loops through each entity and calls their respected Create functions.
	for (auto &pEntity : m_entities)
	{
		if (!pEntity->Create())
		{
			return false;
		}
	}

	// ----------------------------------------------------------
	// Render Target(s)
	// ----------------------------------------------------------
	glm::ivec2 iv2RenderTSize = glm::ivec2(512);
	m_pRenderTarget = std::make_shared<RenderTarget>(m_pCamState, iv2RenderTSize); //TODO: fix fbo size
	if (!m_pRenderTarget->Create()) 
	{ 
		return false; 
	}
	//////////////////////////////////////////////////////////////////////////
	m_pickPosition = glm::vec3(0);

	check_gl_error();

	return true;
}

GLvoid TestApplication::shutdown() 
{
	//////////////////////////////////////////////////////////////////////////
	// YOUR SHUTDOWN CODE HERE
	// -----------------------
	for (auto &pEntity : m_entities)
	{
		pEntity->Destroy();
	}
	//////////////////////////////////////////////////////////////////////////

	// cleanup gizmos
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool TestApplication::Update(GLfloat deltaTime) 
{
	check_gl_error();

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

	m_pCamState = m_pCameraStateMachine->GetCurrentCamera();

#pragma endregion

	// update the camera's movement
	m_pCameraStateMachine->Update(deltaTime);
	// clear the gizmos out for this frame
	Gizmos::clear();

	//////////////////////////////////////////////////////////////////////////
	// YOUR UPDATE CODE HERE
	for (auto &pEntity : m_entities)
	{
		pEntity->Update();
	}
	// FBX Skeleton and Animation
	//FBXUpdate();

	// TODO: does this break things - m_pCamState = m_pCameraStateMachine.get()->GetCurrentCamera();
	m_pMath->Update(m_pCamState);

	///----------------------------------------------------------
	//////////////////////////////////////////////////////////////////////////

	// an example of mouse picking
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		GLdouble x = 0, y = 0;
		glfwGetCursorPos(m_pWindow, &x, &y);

		// plane represents the ground, with a normal of (0,1,0) and a distance of 0 from (0,0,0)
		glm::vec4 plane(0, 1, 0, 0);
		m_pickPosition = m_pCamState->pickAgainstPlane((GLfloat)x, (GLfloat)y, plane);
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

	check_gl_error();

	// return true, else the application closes
	return true;
}

GLvoid TestApplication::Draw()
{
	check_gl_error();

	// Clear ImGui
	ImGui_ImplGlfwGL3_NewFrame();
	check_gl_error();

	// For the render target
	//glBindFramebuffer(GL_FRAMEBUFFER, m_pRenderTarget->GetSharedPointer()->GetFBO());
	glBindFramebuffer(GL_FRAMEBUFFER, m_pRenderTarget.get()->GetFBO());
	//printf("%d\n", m_pRenderApp->GetSharedPointer());
	glViewport(0, 0, 512, 512); // 265 lower quarter of the texture

	// ----------------------------------------------------------
	//glClearColor(m_v3ClearColor.r, m_v3ClearColor.g, m_v3ClearColor.b, 1);
	glClearColor(0.75f, 0.75f, 0.75f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear ImGui
	//ImGui_ImplGlfwGL3_NewFrame();
	// ----------------------------------------------------------
	//glBindVertexArray(m_pRenderApp->GetSharedPointer()->GetVAO());
	glBindVertexArray(m_pRenderTarget.get()->GetMesh().GetVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Draw Captured Objects Here
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
	//glUseProgram(m_pRenderApp->GetProgramID()); //*/
	glUseProgram(m_pRenderTarget->GetFBO()); //*/
	//TODO: VV needed?
	DrawApp();
	check_gl_error();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
	//Render ImGui over everything
	ImGui::Render();
	check_gl_error();
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

	glm::mat4 projView = m_pCamState->getProjectionView();

	// For our RenderTarget/ Camera View on Render Target
	m_pRenderTarget->RenderRenderTargetQuad(projView);

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

	/*m_texture.DrawTexture(*m_pCamState,
		TextureManager::GetSingleton()->GetTextureByName("soulspear_d"),  //m_texture.GetTextureByName("soulspear_d"),
		TextureManager::GetSingleton()->GetTextureByName("soulspear_n")); //m_texture.GetTextureByName("soulspear_n")); */

	for (auto &pEntity : m_entities)
	{
		pEntity->Draw(*m_pCamState);
	}

	m_pRenderTarget->BindDraw();

	// TODO: Get Pyro Textures working again
	//m_render.DrawTextureP(m_pCamState); // TODO: FBX Texture - Needed for Render Target
	/*m_texture.DrawTexture(*m_pCamState,
		TextureManager::GetSingleton()->GetTextureByName("Pyro_D"),  //m_texture.GetTextureByName("Pyro_D"), 
		TextureManager::GetSingleton()->GetTextureByName("Pyro_N"));  //m_texture.GetTextureByName("Pyro_N")); */

	// Old draw items
	Gizmos::addSphere(glm::vec3(0, 7, 0), 0.5f, 8, 8, glm::vec4(1, 1, 0, 1));
	//////////////////////////////////////////////////////////////////////////

	//draw our meshes, or gizmos, to the render target
	// display the 3D gizmos
	Gizmos::draw(projView);

	// get a orthographic projection matrix and draw 2D gizmos
	GLint width = 0, height = 0;
	glfwGetWindowSize(m_pWindow, &width, &height);
	mat4 guiMatrix = glm::ortho<GLfloat>(0, 0, (GLfloat)width, (GLfloat)height);

	Gizmos::draw2D(projView);

#pragma region ImGui
#pragma region My Rendering Options
	// Shows a demonstration on how to use elements of ImGui
	ImGui::ShowTestWindow();
	ImGui::Begin("My rendering options");
	if (ImGui::CollapsingHeader("Test"))
	{
		ImGui::ColorEdit3("clear color", glm::value_ptr(m_v3ClearColor));
		ImGui::Checkbox("Should render Gizmo grid", &m_bDrawGizmoGrid);
		ImGui::Separator();

		// Locations in Grid format
		if (ImGui::TreeNode("Locations"))
		{
			// If button 'A' is pressed...
			if (ImGui::Button("Dingo", ImVec2(40, 40)))
			{
				printf("Helllooo Dingo");
			}
			ImGui::TreePop();
		}
	}

	// Camera State
	ImGui::TextWrapped("Camera Mode");

	//static int selected_camera = -1;
	static E_CAMERA_MODE_STATE selected_camera = m_pCameraStateMachine->GetCurrentCameraMode();
	//const char* c_camera = "Camera"; //TODO:
	const GLchar* names[] = { "Static ", "Fly ", "Orbit ", "Travel ", "Location " };

	if (ImGui::Button("Select.."))
		ImGui::OpenPopup("select");
	ImGui::SameLine();
	ImGui::Text(selected_camera == (E_CAMERA_MODE_STATE)-1 ? "<None>" : names[(GLint)selected_camera]);
	if (ImGui::BeginPopup("select"))
	{
		ImGui::Text("Modes: ");
		ImGui::Separator();
		for (GLint i = 0; i < IM_ARRAYSIZE(names); i++)
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
	static GLint iCurrentCameraMode = m_pCameraStateMachine->GetCurrentCameraMode();
	static GLint eRad = 0;
	//for (GLushort i = 0; i < IM_ARRAYSIZE(names); i++) { //TODO: is this doable?
	// replace numbers in names to be names[i] and    V here
	ImGui::RadioButton(names[0], &eRad, 0); ImGui::SameLine();
	ImGui::RadioButton(names[1], &eRad, 1); ImGui::SameLine();
	ImGui::RadioButton(names[2], &eRad, 2); ImGui::SameLine();
	ImGui::RadioButton(names[3], &eRad, 3); ImGui::SameLine();
	ImGui::RadioButton(names[4], &eRad, 4);
	//}
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
	m_pCameraStateMachine->GetCurrentCamera()->RenderUI(); //m_pCamState->RenderUI();
	ImGui::Separator();

	for (auto &pEntity : m_entities)
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
	glUseProgram(m_program_FBXAnimation_ID);

	int loc = glGetUniformLocation(m_program_FBXAnimation_ID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m_pCameraStateMachine->GetCurrentCamera()->getProjectionView()));

	/*int light_dir_uniform = glGetUniformLocation(m_programID, "LightDir");
	glUniform3f(light_dir_uniform, 0, 1, 0); //* /

	vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_program_FBXAnimation_ID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	int light_colour_uniform = glGetUniformLocation(m_program_FBXAnimation_ID, "LightColour");
	glUniform3f(light_colour_uniform, 1, 1, 1);

	mat4 camera_matrix = m_pCameraStateMachine->GetCurrentCamera()->getTransform();
	int camera_pos_uniform = glGetUniformLocation(m_program_FBXAnimation_ID, "CameraPos");
	glUniform3f(camera_pos_uniform, camera_matrix[3][0], camera_matrix[3][1], camera_matrix[3][2]);

	int specular_uniform = glGetUniformLocation(m_program_FBXAnimation_ID, "SpecPow");
	glUniform1f(specular_uniform, 12);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pRender->GetTextureByName("Pyro_D")); // m_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pRender->GetTextureByName("Pyro_N")); // m_normal);

	loc = glGetUniformLocation(m_program_FBXAnimation_ID, "diffuse");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(m_program_FBXAnimation_ID, "normal");
	glUniform1i(loc, 1);

	FBXSkeleton* skeleton = m_pFbx->getSkeletonByIndex(0);
	skeleton->updateBones();

	int bones_location = glGetUniformLocation(m_program_FBXAnimation_ID, "bones");
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