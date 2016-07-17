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
/// Number of Threads available http://en.cppreference.com/w/cpp/thread/thread/hardware_concurrency
/// Dev guide https://developer.mozilla.org/en-US/docs/Mozilla/Developer_guide/Coding_Style#C.2FC.2B.2B_practices
/// 
/// ***EDIT***
/// - Render Targets Working	- David Azouz 7/03/16
/// - Camera States implemented	- David Azouz 7/03/16
/// - FBX Animation working	 	- David Azouz 8/03/16
/// - Removed m_pCamera and replaced with m_pCameraStateMachine
/// - Set up Entity/ Game Object system	- David Azouz 18/03/16
/// - pRender & m_pRender business somewhat fixed - David Azouz 14/06/16
/// 
/// Notes:
/// Threads: Windows IOCP
/// static std::mutex myMutex;
/// std::lock_guard std::mutex> guard(myMutex);
///
/// TODO: 
/// Invoke and cycle camera
/// change to #include <gl_core_4_4.h, <Gizmos.h, and to void
/// change all (float)INT_MAX to FLT_MAX
/// Location camera ++i and replace i+1 with i in sLocation
/// explicit keyword for Constructors that take one var
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
#include "RenderTarget.h"
#include "Helpers.h"
#include "MathCollision.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <assert.h>
#include <Gizmos.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp> //needed for pi
#include <iostream>
#include <mutex> //locking/ unlocking threads
#include <thread>
//#include <vld.h> // TODO: Visual Leak Detecter

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

using glm::vec3;
using glm::vec4;
using glm::mat4;
using namespace std;

bool TestApplication::startup() 
{
	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);
	//if(_DEBUG) //TODO: 
	TurnOnOpenGLDebugLogging();

	// start the gizmo system that can draw basic shapes
	Gizmos::create();

#pragma region Camera
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
#pragma endregion

	//////////////////////////////////////////////////////////////////////////
	// YOUR STARTUP CODE HERE
	// -----------------------
	///<summary>
	/// Threading
	/// may return 0 when not able to detect 
	/// TODO: Threads seek work from a queue if their spin count is greater than 'x'
	/// Research Windows IOCP
	///</summary>
	// -----------------------
	// this is to stop chunkLength equalling to 0
	if (GetNumOfThreadsAvailable() == 1) //<= 2 && concurentThreadsSupported > 0
	{
		chunkLength = 1;
	}
	// If we have more than one available thread...
	else if (GetNumOfThreadsAvailable() > 1)
	{
		// ... Distribute the "weight" in chunks
		chunkLength = (int)m_entities.size() / GetNumOfThreadsAvailable();
	}
	else
	{
		chunkLength = 0;
		printf("Buy a new PC, cannot detect any threads.");
	}
	// -----------------------
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

	// Adds our inherited classes to the vector.
	m_entities.push_back(std::make_shared<Grid>());
	m_entities.push_back(std::make_shared<FBXModel>("./data/models/soulspear/soulspear.fbx"));
	m_entities.push_back(std::make_shared<FBXModel>("./data/models/characters/Pyro/pyro.fbx"));
	m_entities.push_back(std::make_shared<ParticleEmitter>(configA));
	m_entities.push_back(std::make_shared<ParticleEmitter>(configB));
	m_entities.push_back(std::make_shared<GPUParticleEmitter>()); //TODO: uncomment to add GPU Particles again

	//m_pMath = std::make_shared<MathCollision>();
	//m_pPhysics = std::make_shared<Physics>();

	//int i = 0;
	// Loops through each entity and calls their respected Create functions.
	for (auto &pEntity : m_entities)
	{
		/*// Making use of a Lambda function //	qThreads.push(std::thread([&pEntity]()
		vThreads.push_back(std::thread([&pEntity](int low, int high)
		{
			// Only "process" as much data our threads can handle
			for (int j = low; j < high; j++)
			{ */
		if (!pEntity->Create())
		{
			return false;
		}
		/*	}
		}, i * chunkLength, (i + 1) * chunkLength
			));
		i++; */
	}
	//m_pMath->Create(); // so far does nothing
	//TODO: threading
	//vThreads.push_back(std::thread([this]() {
	if (!m_pPhysics->Create())
	{
		return false;
	} //}));
	// ----------------------------------------------------------
	// Render Target(s)
	// ----------------------------------------------------------
	glm::ivec2 iv2RenderTSize = glm::ivec2(512);
	m_pRenderTarget = std::make_shared<RenderTarget>(m_pCamState, iv2RenderTSize); //TODO: fix fbo size
	if (!m_pRenderTarget->Create()) 
	{ 
		return false; 
	}

	/*for (auto &thread : vThreads)
	{
		thread.join();
	}
	// clear the data in vThreads for our 'Update' version of threads
	vThreads.clear();*/
	//////////////////////////////////////////////////////////////////////////
	//m_pickPosition = glm::vec3(0);

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
	m_pPhysics->Shutdown();
	//////////////////////////////////////////////////////////////////////////

	// cleanup gizmos
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool TestApplication::Update(GLfloat a_deltaTime) 
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
	m_fPrevTime += a_deltaTime;
	// Allow the character to be moved
	if (glfwGetKey(m_pWindow, GLFW_KEY_0) == GLFW_PRESS && !m_hasSpaceBeenPressed 
		|| glfwGetKey(m_pWindow, GLFW_KEY_KP_0) == GLFW_PRESS && !m_hasSpaceBeenPressed)
	{
		m_isCharacterControlled = !m_isCharacterControlled; // Toggle movement for the character
		m_hasSpaceBeenPressed = true;
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_0) == GLFW_RELEASE && m_hasSpaceBeenPressed
		|| glfwGetKey(m_pWindow, GLFW_KEY_KP_0) == GLFW_RELEASE && m_hasSpaceBeenPressed)
	{
		m_hasSpaceBeenPressed = false;
	}
	// Helps prevent the character and camera moving at the same time.
	if (m_isCharacterControlled)
	{
		//Character controller
		if (!m_pPhysics->UpdatePlayerController(a_deltaTime))
		{
			return false;
		}
		//TODO: one day - parent the camera to the character controller/ FBX
		glm::vec4 v4PlayerCameraPos = glm::vec4(0, -8, 20, 0);
		glm::vec4 v4From = m_entities[2]->GetPosition() - v4PlayerCameraPos;
		glm::vec4 v4To = m_entities[2]->GetPosition();
		// Set to static cam to prevent the camera from moving
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_STATIC);
		m_pCameraStateMachine->GetCurrentCamera()->setLookAtFrom(v4From, v4To);
		//m_entities[2]->SetPosition(m_entities[2]->GetPosition() + v4PlayerCameraPos);
	}
	// Camera cycling and lerping // slerping/ squad.
	// Cycles between various Cameras during run time
	else if (glfwGetKey(m_pWindow, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS && !m_hasSpaceBeenPressed)
	{
		//std::invoke(
		E_CAMERA_MODE_STATE eCurrentCameraMode = m_pCameraStateMachine->GetCurrentCameraMode();
		if (m_fPrevTime > 1)
		{
			eCurrentCameraMode = (E_CAMERA_MODE_STATE)(eCurrentCameraMode + 1);
			if (eCurrentCameraMode > E_CAMERA_MODE_STATE_COUNT - 1)
			{
				eCurrentCameraMode = (E_CAMERA_MODE_STATE)0;
			}
			m_pCameraStateMachine->ChangeState((E_CAMERA_MODE_STATE)(eCurrentCameraMode));
			m_fPrevTime = 0;
		}
		m_hasSpaceBeenPressed = true;
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE && m_hasSpaceBeenPressed)
	{
		m_hasSpaceBeenPressed = false;
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
	m_pCameraStateMachine->Update(a_deltaTime);
	// clear the gizmos out for this frame
	Gizmos::clear();

	//////////////////////////////////////////////////////////////////////////
	// YOUR UPDATE CODE HERE
	//int i = 0;
	for (auto &pEntity : m_entities)
	{
		/*// Making use of a Lambda function //	qThreads.push(std::thread([&pEntity]()
		vThreads.push_back(std::thread([&pEntity, &deltaTime](int low, int high)
		{
			// Only "process" as much data our threads can handle
			for (int j = low; j < high; j++)
			{ */
		if (!pEntity->Update(a_deltaTime))
		{
			return false;
		}
			/*}
		}, i * chunkLength, (i + 1) * chunkLength
			));
		i++; */
	}

	// TODO: does this break things - m_pCamState = m_pCameraStateMachine.get()->GetCurrentCamera();
	m_pMath->Update(*m_pCamState);

	//vThreads.push_back(std::thread([&]() {
	m_pPhysics->Update(a_deltaTime, *m_pCamState);// })); //TODO: thread
	/*for (auto &thread : vThreads)
	{
		thread.join();
	}
	vThreads.clear(); */

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
	if (glfwGetKey(m_pWindow, GLFW_KEY_9))
	{
		m_eCurrentDrawState = E_DRAW_STATE_FILL;
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_8))
	{
		m_eCurrentDrawState = E_DRAW_STATE_POLY;
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_7))
	{
		m_eCurrentDrawState = E_DRAW_STATE_DOT;
	}
	// Cycle Draw State
	if (glfwGetKey(m_pWindow, GLFW_KEY_MINUS) == GLFW_PRESS && !m_hasSpaceBeenPressed)
	{
		if (m_fPrevTime > 1)
		{
			if (m_eCurrentDrawState < 0)
			{
				m_eCurrentDrawState = (E_DRAW_STATE)(E_DRAW_STATE_COUNT - 1);
			}
			//effectively m_eCurrentDrawState--;
			m_eCurrentDrawState = (E_DRAW_STATE)(m_eCurrentDrawState - 1);
			m_hasSpaceBeenPressed = true;
			m_fPrevTime = 0;
		}
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_MINUS) == GLFW_RELEASE && m_hasSpaceBeenPressed)
	{
		m_hasSpaceBeenPressed = false;
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_EQUAL) == GLFW_PRESS && !m_hasSpaceBeenPressed) // == GLFW_RELEASE
	{
		if (m_fPrevTime > 1)
		{
			if (m_eCurrentDrawState > E_DRAW_STATE_COUNT)
			{
				m_eCurrentDrawState = (E_DRAW_STATE)0;
			}
			//effectively m_eCurrentDrawState++;
			m_eCurrentDrawState = (E_DRAW_STATE)(m_eCurrentDrawState + 1);
			m_hasSpaceBeenPressed = true;
			m_fPrevTime = 0;
		}
	}
	else if (glfwGetKey(m_pWindow, GLFW_KEY_EQUAL) == GLFW_RELEASE && m_hasSpaceBeenPressed)
	{
		m_hasSpaceBeenPressed = false;
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
	//check_gl_error(); //TODO: restore check

#pragma region For the render target
	//glBindFramebuffer(GL_FRAMEBUFFER, m_pRenderTarget.get()->GetFBO());
	//printf("%d\n", m_pRenderApp->GetSharedPointer());
	//glViewport(0, 0, 512, 512); // 265 lower quarter of the texture

	// ----------------------------------------------------------
	// Clear ImGui
	//ImGui_ImplGlfwGL3_NewFrame();
	// ----------------------------------------------------------
	//glBindVertexArray(m_pRenderApp->GetSharedPointer()->GetVAO());
	//glBindVertexArray(m_pRenderTarget->GetRenderable()->mesh.GetVAO()); //.get()->GetMesh().GetVAO());
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// TODO: Render to texture
	//glBindFramebuffer(m_pRenderTarget->GetFBO());
	//Gizmos::draw(m_pCamState->getProjectionView());
	//DrawApp();

	// unbind the FBO so that we can render to the back buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glUseProgram(m_pRenderApp->GetProgramID()); //*/
	DrawApp();
	//check_gl_error(); //TODO: restore this for error checking
#pragma endregion

	//Render ImGui over everything
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill
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
	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// use our texture program

	glm::mat4 projView = m_pCamState->getProjectionView();

	// For our RenderTarget/ Camera View on Render Target
	// TODO: Render to texture
	//m_pRenderTarget->RenderRenderTargetQuad(projView);

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
		TextureManager::GetSingleton()->GetTextureByName("soulspear_d"),  //m_texture.GetTextureByName("soulspear_d"), //Pyro_D
		TextureManager::GetSingleton()->GetTextureByName("soulspear_n")); //m_texture.GetTextureByName("soulspear_n")); Pyro_N */

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

	for (auto &pEntity : m_entities)
	{
		pEntity->Draw(*m_pCamState);
	}

	// TODO: Render to texture
	//m_pRenderTarget->BindDraw();

	m_pPhysics->Draw(*m_pCamState);

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
	// My ImGui features
	ImGui::Begin("My rendering options");
	if (ImGui::CollapsingHeader("Test"))
	{
		ImGui::ColorEdit3("clear color", glm::value_ptr(m_v3ClearColor));
		ImGui::Checkbox("Character Controllerable?", &m_isCharacterControlled);
		ImGui::Checkbox("Should render Gizmo grid", &m_bDrawGizmoGrid);
		ImGui::Separator(); 

		/* TODO: delete me // Locations in Grid format
		if (ImGui::TreeNode("Locations"))
		{
			// If button 'A' is pressed...
			if (ImGui::Button("Dingo", ImVec2(40, 40)))
			{
				printf("Helllooo Dingo");
			}
			ImGui::TreePop();
		} */
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

	m_pPhysics->RenderUI();

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