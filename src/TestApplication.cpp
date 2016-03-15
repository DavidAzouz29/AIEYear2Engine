/// <summary>
/// File Name:		TestApplication.cpp
/// Author: 		David Azouz
/// Date Created: 	29/02/16
/// Date Modified: 	8/03/16
///----------------------------------------------------------
/// Brief: A TestApplication Class that Updates and Draws
/// viewed: 
/// Source: https://github.com/DavidAzouz29/AIEYear2Engine
/// Lib and related files: https://drive.google.com/folderview?id=0B1wViLeuTDL8TF9feHl2RThFR0E&usp=sharing
/// Invoke http://en.cppreference.com/w/cpp/utility/functional/invoke
/// 
/// ***EDIT***
/// - Render Targets Working	- David Azouz 7/03/16
/// - Camera States implemented	- David Azouz 7/03/16
/// - FBX Animation working	 	- David Azouz 8/03/16
/// - Camera Cycle implemented	- David Azouz /03/16
/// 
/// TODO: remove m_pCamera and replace with m_pCameraStateMachine
/// Invoke and cycle camera
/// </summary>
/// ----------------------------------------------------------
#include "TestApplication.h"
#include "gl_core_4_4.h"
#include "FBXFile.h"
#include "Camera.h"
#include "CameraStateMachine.h"
#include "Gizmos.h"
#include "ParticleEmitter.h"
#include "GPUParticleEmitter.h"
#include "MathCollision.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <assert.h>

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

using glm::vec3;
using glm::vec4;
using glm::mat4;

//--------------------------------------------------------------------------------------
// Default Destructor
//--------------------------------------------------------------------------------------
TestApplication::TestApplication()
	:// m_pCamera(nullptr),
	m_timer(0),
	m_v4StartColor(1, 0, 0, 1),
	m_v4EndColor(1, 1, 0, 1),
	m_eCurrentDrawState(E_DRAW_STATE_FILL), 
	m_bDrawGizmoGrid(true)
{

}

TestApplication::~TestApplication() {
	glDeleteProgram(m_program_ID);
	glDeleteProgram(m_FBX_program_ID);
}

bool TestApplication::startup() {

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic shapes
	Gizmos::create();

	// create a camera
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
	m_pRender = std::make_shared<Render>();
	// -----------------------
	// Creates Grid
	// -----------------------
	unsigned int uiGrid = 21;
	//m_pRender->generateGrid(uiGrid, uiGrid);
	//m_pRender->InitGeometry();
	// -----------------------

	// -----------------------
	// FBX
	// -----------------------
	m_pFbx = std::make_shared<FBXFile>();
	//m_pFbx->load("./data/models/stanford/Bunny.fbx");
	//m_pFbx->load("./data/models/soulspear/soulspear.fbx");
	m_pFbx->load("./data/models/characters/Pyro/pyro.fbx", FBXFile::UNITS_METER);
	//m_pFbx->load("./data/models/characters/Pyro/pyro.fbx", m_pFbx->UNITS_METER);
	//FBXLoader(); // Needed if FBX without Animation
	FBXSkeletonLoader();

	CreateOpenGLBuffers(m_pFbx.get());

	//m_pRender->TextureLoader();

	//unsigned int id = m_pRender->TextureInit("./data/textures/crate.png");
	//m_pRender->AddTexture("crate", id);

	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, id); */

	unsigned int id = m_pRender->TextureInit("./data/models/soulspear/soulspear_diffuse.tga");
	m_pRender->AddTexture("soulspear_d", id);

	id = m_pRender->TextureInit("./data/models/soulspear/soulspear_normal.tga");
	m_pRender->AddTexture("soulspear_n", id);

	id = m_pRender->TextureInit("./data/models/characters/Pyro/Pyro_D.tga");
	m_pRender->AddTexture("Pyro_D", id);

	id = m_pRender->TextureInit("./data/models/characters/Pyro/Pyro_N.tga");
	m_pRender->AddTexture("Pyro_N", id);

	id = m_pRender->TextureInit("./data/models/characters/Pyro/Pyro_S.tga");
	m_pRender->AddTexture("Pyro_S", id);

	//m_pRender->RenderTexture(); //For the Render Target?

	//=======================================================
	m_pRender.get()->RenderTargetLoader();
	m_pRender->GetSharedPointer()->CreateRenderTargetQuad();
	//=======================================================
 	m_pMath = std::make_shared<MathCollision>();
	///------------------------------------------------------
#pragma region Particles
	///----------------------------------------------------------
	m_pParticleEmitterA = std::make_shared<ParticleEmitter>();
	m_pParticleEmitterB = std::make_shared<ParticleEmitter>();
	///----------------------------------------------------------
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

	if (!m_pParticleEmitterA->create(configA)) return -4;

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

	if (!m_pParticleEmitterB->create(configB)) return -5;
#pragma endregion
	///------------------------------------------------------
#pragma region GPUParticles
	m_pGPUEmitter = std::make_shared<GPUParticleEmitter>();
	const GLuint MAX_PARTICLES = 100; //100000
	m_pGPUEmitter.get()->Initalise(MAX_PARTICLES, 0.1f, 5.0f, 5, 20, 1, 0.1f,
		m_v4StartColor, m_v4EndColor);

#pragma endregion
	///------------------------------------------------------
	m_v3ClearColor = glm::vec3(0.25f);
	//////////////////////////////////////////////////////////////////////////
	m_pickPosition = glm::vec3(0);

	return true;
}

void TestApplication::shutdown() {

	//////////////////////////////////////////////////////////////////////////
	// YOUR SHUTDOWN CODE HERE
	// -----------------------
	// FBX
	// -----------------------
	CleanupOpenGLBuffers(m_pFbx.get());
	//////////////////////////////////////////////////////////////////////////

	// delete our camera and cleanup gizmos
	//delete m_pCamera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool TestApplication::update(float deltaTime) {
	
	// close the application if the window closes
	if (glfwWindowShouldClose(m_pWindow) ||
		glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}

	//Camera Mode: Static, FlyCamera, Orbit
#pragma region Camera Mode
	int iPrevTime = (int)m_timer;
	// TODO: get camera cycling and lerp/ slerping/ squad working
	// Cycles between various Cameras during run time
	if (glfwGetKey(m_pWindow, GLFW_KEY_GRAVE_ACCENT) || glfwGetKey(m_pWindow, GLFW_KEY_KP_0))
	{
		//std::invoke(
		E_CAMERA_MODE_STATE eCurrentCameraMode = m_pCameraStateMachine->GetCurrentCameraMode();
		if (iPrevTime > 5)
		{
			eCurrentCameraMode = (E_CAMERA_MODE_STATE)(eCurrentCameraMode + 1);
			if (eCurrentCameraMode > E_CAMERA_MODE_STATE_COUNT - 1)
			{
				eCurrentCameraMode = (E_CAMERA_MODE_STATE)0;
			}
			m_pCameraStateMachine->ChangeState((E_CAMERA_MODE_STATE)(eCurrentCameraMode));
			iPrevTime = 0;
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
	//m_pCamera->Update(deltaTime);
	m_pCameraStateMachine->Update(deltaTime);
	// clear the gizmos out for this frame
	Gizmos::clear();

	//TODO: ImGui here?

	//////////////////////////////////////////////////////////////////////////
	// YOUR UPDATE CODE HERE

	// FBX Skeleton and Animation
	FBXUpdate();

	//m_pMath->Update(m_pCamera.get());
	m_pCamState = m_pCameraStateMachine.get()->GetCurrentCamera();

	m_pMath->Update(m_pCamState);
	///----------------------------------------------------------
	// Particles
	//m_pParticleEmitterA->update(deltaTime, m_pCamera->getTransform());
	m_pParticleEmitterA->update(deltaTime, m_pCamState->getTransform());
	m_pParticleEmitterB->update(deltaTime, m_pCamState->getTransform());
	//////////////////////////////////////////////////////////////////////////

	// an example of mouse picking
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		double x = 0, y = 0;
		glfwGetCursorPos(m_pWindow, &x, &y);

		// plane represents the ground, with a normal of (0,1,0) and a distance of 0 from (0,0,0)
		glm::vec4 plane(0, 1, 0, 0);
		//m_pickPosition = m_pCamera->pickAgainstPlane((float)x, (float)y, plane);
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

void TestApplication::draw() 
{
	//
	glBindFramebuffer(GL_FRAMEBUFFER, m_pRender->GetSharedPointer()->GetFBO());
	//printf("%d\n", m_pRender->GetSharedPointer());
	glViewport(0, 0, 512, 512); // 265 lower quarter of the texture

	// ----------------------------------------------------------
	glClearColor(m_v3ClearColor.r, m_v3ClearColor.g, m_v3ClearColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear ImGui
	ImGui_ImplGlfwGL3_NewFrame();
	// ----------------------------------------------------------
	glBindVertexArray(m_pRender->GetSharedPointer()->GetVAO());
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(m_pRender->GetProgramID());
	DrawApp();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
	//Render ImGui over everything
	ImGui::Render();
}

void TestApplication::DrawApp()
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
	int loc = glGetUniformLocation(m_pRender->GetProgramID(), "ProjectionView"); //m_program_ID
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projView));

	// Set texture slots
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pRender->GetSharedPointer()->GetFboTexture());

	// tell the shader where it is
	int diffLoc = glGetUniformLocation(m_pRender->GetProgramID(), "diffuse"); // m_program_ID
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
	//m_pRender->DrawTexture(m_pCamera.get());
	//m_pRender->DrawTexture(m_pCamState); //TODO: needed for Soulspear

	glBindVertexArray(m_pRender->GetSharedPointer()->GetVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	m_pRender->DrawTextureP(m_pCamState); // TODO: FBX Texture - Needed for Render Target
	
	Gizmos::addSphere(glm::vec3(0, 7, 0), 0.5f, 8, 8, m_v4EndColor);
	//
	//m_pVertexColoredGrid->draw(projView);
	//m_pSpriteSheetQuad->draw(projView);
	//m_pFBXMesh->draw(projView);

	// FBX
	//RenderFBX(m_pCamState); // Need this for FBX
	FBXDraw();
	//FBXSkeletonRender();

	// Particles
	m_pParticleEmitterA->draw(projView);
	m_pParticleEmitterB->draw(projView);

	// GPU Particles
	m_pGPUEmitter.get()->Draw((GLfloat)glfwGetTime(),
		m_pCamState->getTransform(),
		m_pCamState->getProjectionView());
	//////////////////////////////////////////////////////////////////////////

	//draw our meshes, or gizmos, to the render target
	// display the 3D gizmos
	Gizmos::draw(projView);

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_pWindow, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(projView);

	ImGui::ShowTestWindow();
	ImGui::Begin("My rendering options");
		ImGui::ColorEdit3("clear color", glm::value_ptr(m_v3ClearColor));
		ImGui::ColorEdit3("Particle Start Colour", glm::value_ptr(m_v4StartColor));
		ImGui::ColorEdit3("Particle End Colour", glm::value_ptr(m_v4EndColor));
		ImGui::Checkbox("Should render Gizmo grid", &m_bDrawGizmoGrid);
		ImGui::Separator();

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

	ImGui::End();

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
	///-----------------------------------------------------------------------------------------------------------
}

#pragma region FBX
/// ----------------------------------------------------------
/// FBXLoader
///-----------------------------------------------------------------------------------------------------------
/// <summary> 
/// <para>Loop through the meshes in the scene and add 3 uint variables to their m_userData.</para>
/// <para>Initialises these variables to contain a VAO, VBO, IBO, based off the FBXMeshdata.</para>
/// <para><param name="fbx" type ="FBXFile*"> P1: A fbx file.</param></para></summary>
///-----------------------------------------------------------------------------------------------------------
void TestApplication::CreateOpenGLBuffers(FBXFile* fbx)
{
	// create the GL VAO/VBO/IBO data for each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);
		//FBXTexture* texture = fbx->getTextureByName("./data/soulspear/soulspear.obj");// , &imageWidth, &imageHeight, &imageFormat, STBI_default);

		// storage for the opengl data in 3 unsigned int
		unsigned int* glData = new unsigned int[3];

		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);

		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);

		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

		glBufferData(GL_ARRAY_BUFFER,
			mesh->m_vertices.size() * sizeof(FBXVertex),
			mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh->m_indices.size() * sizeof(unsigned int),
			mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normals

		glEnableVertexAttribArray(2); //tangents
		glEnableVertexAttribArray(3); //textcoords Texture on FBX model via coordinates
		glEnableVertexAttribArray(4); //weights
		glEnableVertexAttribArray(5); //indices

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,  sizeof(FBXVertex), (void*)FBXVertex::NormalOffset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE,  sizeof(FBXVertex), (void*)FBXVertex::TangentOffset);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::WeightsOffset);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::IndicesOffset);


		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;
	}
}
///-----------------------------------------------------------------------------------------------------------
/// <summary>Loop through and destroy all the OpenGL buffers for the FBXMeshNodes.</summary>
///-----------------------------------------------------------------------------------------------------------
void TestApplication::CleanupOpenGLBuffers(FBXFile* fbx)
{
	// clean up the vertex data attached to each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);

		delete[] glData;
	}
}
void TestApplication::FBXLoader()
{
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Normal; \
							layout(location=2) in vec2 TexCoord; \
							out vec4 vNormal; \
							out vec2 vTexCoord; \
							uniform mat4 ProjectionView; \
							uniform mat4 WorldTransform; \
							void main() { vNormal = Normal; vTexCoord = TexCoord; \
							gl_Position = ProjectionView * WorldTransform * Position; }";

	const char* fsSource = "#version 410\n \
							in vec4 vNormal; \
							in vec2 vTexCoord; \
							out vec4 FragColor; \
							uniform sampler2D diffuse; \
							void main() { FragColor = texture(diffuse, vTexCoord); }";

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	//int success = GL_FALSE;
	unsigned int iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(iVertexShader);

	unsigned int iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(iFragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_program_ID = glCreateProgram();
	glAttachShader(m_program_ID, iVertexShader);
	glAttachShader(m_program_ID, iFragmentShader);
	glLinkProgram(m_program_ID);

	/*glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
	int infoLogLength = 0;
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	char* infoLog = new char[infoLogLength];

	glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
	printf("Error: Failed to link shader program!\n");
	printf("%s\n", infoLog);
	delete[] infoLog;
	}*/

	glDeleteShader(iVertexShader);
	glDeleteShader(iFragmentShader);
}
/// ----------------------------------------------------------
/// FBXLoader
///-----------------------------------------------------------------------------------------------------------
/// <summary> 
/// <para>Bind the shader and send across the virtual camera's projection and view matrices combined,</para>
/// <para>then we loop through the meshes and render them.</para>
/// <para><param name="cam" type ="Camera*"> P1: A virtual camera.</param></para></summary>
///-----------------------------------------------------------------------------------------------------------
void TestApplication::RenderFBX(Camera* cam)
{
	glUseProgram(m_program_ID);

	// bind the camera
	int loc = glGetUniformLocation(m_program_ID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(cam->getProjectionView()[0][0])); //m_projectionViewMatrix

	GLuint id = m_pRender->GetTextureByName("soulspear_d"); //TODO: soulspear
	//const unsigned int id = m_pRender->GetTextureByName("Pyro_D");
	glBindTexture(GL_TEXTURE_2D, id);

	// Scale the FBX
	glm::mat4 m4WorldTransform(1);
	GLfloat fScale = 0.003f;
	m4WorldTransform = glm::scale(glm::vec3(fScale));
	id = glGetUniformLocation(m_program_ID, "WorldTransform");
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(m4WorldTransform));

	loc = glGetUniformLocation(m_program_ID, "diffuse");
	glUniform1i(loc, 0);

	// bind our vertex array object and draw the mesh
	for (unsigned int i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glBindVertexArray(glData[0]); //TODO: replace m_VAO with VAO
		//glBindVertexArray(Geom->GetVAO()); //TODO: replace m_VAO with VAO
		//unsigned int indexCount = (a_iRows - 1) * (a_iCols - 1) * 6; //TODO: m_iIndexCount = this formula
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
}

// skeleton/ bones vertex shader
void TestApplication::FBXSkeletonLoader()
{
	/// ----------------------------------------------------------
	/// Create shaders 
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------

	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Normal; \
							layout(location=2) in vec4 Tangent; \
							layout(location=3) in vec2 TexCoord; \
							layout(location=4) in vec4 Weights; \
							layout(location=5) in vec4 Indices; \
							/* frag */\
							out vec3 frag_normal; \
							out vec3 frag_position; \
							out vec3 frag_tangent; \
							out vec3 frag_bitangent; \
							out vec2 frag_texcoord; \
							/**/\
							uniform mat4 ProjectionView; \
							uniform mat4 WorldTransform; \
							/* We need to give our bone array a limit */\
							const int MAX_BONES = 128; \
							uniform mat4 bones[MAX_BONES]; \
							void main() { frag_position = Position.xyz;\
							frag_normal = Normal.xyz; \
							frag_tangent = Tangent.xyz; \
							frag_bitangent = cross(Normal.xyz, Tangent.xyz); \
							frag_texcoord = TexCoord; \
							/* Cast the indices to integer's so they can index an array */\
							ivec4 index = ivec4(Indices); \
							/* Sample bones and blend up to 4 */\
							vec4 P = bones[index.x] * Position * Weights.x; \
							P += bones[index.y] * Position * Weights.y; \
							P += bones[index.z] * Position * Weights.z; \
							P += bones[index.w] * Position * Weights.w; \
							gl_Position = ProjectionView * WorldTransform * P; }";

	const char* fsSource = "#version 410\n \
							in vec3 frag_normal; \
							in vec3 frag_position; \
							in vec2 frag_texcoord; \
							in vec3 frag_tangent; \
							in vec3 frag_bitangent; \
							out vec4 FragColor; \
							uniform vec3 LightDir; \
							uniform vec3 LightColour; \
							uniform vec3 CameraPos; \
							uniform sampler2D diffuse; \
							uniform sampler2D normal; \
							uniform float SpecPow; \
							void main() { \
							mat3 TBN = mat3( \
							normalize( frag_tangent ), \
							normalize( frag_bitangent ), \
							normalize( frag_normal )); \
							vec3 N = texture( normal, \
							frag_texcoord).xyz * 2 - 1; \
							float d = max(0, dot( normalize(TBN * N), normalize(LightDir) ) ); \
							vec3 E = normalize( CameraPos - frag_position ); \
							vec3 R = reflect( -LightDir, frag_normal ); \
							float s = max( 0, dot( E, R ) ); \
							s = pow( s, SpecPow ); \
							FragColor = texture(diffuse, frag_texcoord); \
							FragColor.rgb = FragColor.rgb * LightColour * d + LightColour * s * 0.1; }";

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	int success = GL_FALSE;
	unsigned int iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(iVertexShader);

	unsigned int iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(iFragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_FBX_program_ID = glCreateProgram();
	glAttachShader(m_FBX_program_ID, iVertexShader);
	glAttachShader(m_FBX_program_ID, iFragmentShader);
	glLinkProgram(m_FBX_program_ID);

	glGetProgramiv(m_FBX_program_ID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_FBX_program_ID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_FBX_program_ID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(iVertexShader);
	glDeleteShader(iFragmentShader);
}

// Render
void TestApplication::FBXSkeletonRender()
{
	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //normals
	glEnableVertexAttribArray(2); //tangents
	glEnableVertexAttribArray(3); //textcoords
	glEnableVertexAttribArray(4); //weights
	glEnableVertexAttribArray(5); //indices

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::PositionOffset);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE , sizeof(FBXVertex), (void*)FBXVertex::NormalOffset);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE , sizeof(FBXVertex), (void*)FBXVertex::TangentOffset);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::TexCoord1Offset);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::WeightsOffset);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::IndicesOffset);
}

// Used for FBX Skeleton and Animation
void TestApplication::FBXUpdate()
{
	// Grab the skeleton and animation we want to use
	FBXSkeleton* skeleton = m_pFbx->getSkeletonByIndex(0);
	FBXAnimation* animation = m_pFbx->getAnimationByIndex(0);

	m_timer = (GLfloat)glfwGetTime(); //currentTime 

	// Evaluate the animation to update bones
	skeleton->evaluate(animation, m_timer);

	for (GLuint uiBone_index = 0;
	uiBone_index < skeleton->m_boneCount;
		++uiBone_index)
	{
		skeleton->m_nodes[uiBone_index]->updateGlobalTransform();
	}
}

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
	glBindTexture(GL_TEXTURE_2D, m_pRender->GetTextureByName("Pyro_D")); // m_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pRender->GetTextureByName("Pyro_N")); // m_normal);

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

void TestApplication::FBXDraw()
{
	glUseProgram(m_FBX_program_ID);

	int loc = glGetUniformLocation(m_FBX_program_ID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m_pCamState->getProjectionView()));

	/*int light_dir_uniform = glGetUniformLocation(m_programID, "LightDir");
	glUniform3f(light_dir_uniform, 0, 1, 0);*/

	vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_FBX_program_ID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	int light_colour_uniform = glGetUniformLocation(m_FBX_program_ID, "LightColour");
	glUniform3f(light_colour_uniform, 1, 1, 1);

	mat4 camera_matrix = m_pCamState->getTransform();
	int camera_pos_uniform = glGetUniformLocation(m_FBX_program_ID, "CameraPos");
	glUniform3f(camera_pos_uniform, camera_matrix[3][0], camera_matrix[3][1], camera_matrix[3][2]);

	int specular_uniform = glGetUniformLocation(m_FBX_program_ID, "SpecPow");
	glUniform1f(specular_uniform, 12);

	// Scale the FBX
	glm::mat4 m4WorldTransform(1);
	GLfloat fScale = 0.003f;
	m4WorldTransform = glm::scale(glm::vec3(fScale));
	GLuint worldTransform = glGetUniformLocation(m_FBX_program_ID, "WorldTransform");
	glUniformMatrix4fv(worldTransform, 1, GL_FALSE, glm::value_ptr(m4WorldTransform));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pRender->GetTextureByName("Pyro_D")); // m_texture); 
	loc = glGetUniformLocation(m_FBX_program_ID, "diffuse");
	glUniform1i(loc, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pRender->GetTextureByName("Pyro_N")); // m_normal);
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
}
#pragma endregion