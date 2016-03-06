// TODO: remove m_pCamera and replace with m_pCameraStateMachine
/// Get Render Targets Working
/// Get Animation Working

#include "TestApplication.h"
#include "gl_core_4_4.h"
#include "FBXFile.h"
#include "Camera.h"
#include "CameraStateMachine.h"
#include "Gizmos.h"
#include "ParticleEmitter.h"
#include "MathCollision.h"

#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <assert.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

//--------------------------------------------------------------------------------------
// Default Destructor
//--------------------------------------------------------------------------------------
TestApplication::TestApplication()
	:// m_pCamera(nullptr),
	m_eCurrentDrawState(E_DRAW_STATE_FILL) {

}

TestApplication::~TestApplication() {

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
	vec4 v4Perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 100000.f);
	m_pCameraStateMachine = std::make_shared<CameraStateMachine>(v4Perspective);

	// Repeating code: TODO: change
	/*if (m_pCameraStateMachine->GetCurrentCamera() == nullptr)
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_FLYCAMERA);
	}*/
	//////////////////////////////////////////////////////////////////////////
	// YOUR STARTUP CODE HERE
	// -----------------------
	m_pRender	= std::make_shared<Render>();
	m_pMesh		= std::make_shared<Mesh>();
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
	m_pFbx->load("./data/models/characters/Pyro/pyro.fbx");// , FBXFile::UNITS_METER);
	//m_pFbx->load("./data/models/characters/Pyro/pyro.fbx", m_pFbx->UNITS_METER);
	FBXLoader();
	//FBXSkeletonLoader();
	//FBXSkeletonRender();

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

	m_pRender->RenderTexture(); 

	// -----------------------
	//m_pMesh->createFrame(); // Like init
	//===============================================================================
	m_pRender->RenderTargetLoader();
	m_pMesh->RenderRenderTarget();
	//===============================================================================

	///----------------------------------------------------------
 	m_pMath = std::make_shared<MathCollision>();
	///----------------------------------------------------------
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
	///--------------------------------------------------
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
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		return false;
	}

#pragma region Camera Mode

	if (glfwGetKey(m_window, GLFW_KEY_GRAVE_ACCENT) || glfwGetKey(m_window, GLFW_KEY_KP_0))
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_STATIC);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_1) || glfwGetKey(m_window, GLFW_KEY_KP_1))
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_FLYCAMERA);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_2) || glfwGetKey(m_window, GLFW_KEY_KP_2))
	{
		m_pCameraStateMachine->ChangeState(E_CAMERA_MODE_STATE_ORBIT);
	}
#pragma endregion

	// update the camera's movement
	//m_pCamera->Update(deltaTime);
	m_pCameraStateMachine->Update(deltaTime);
	// clear the gizmos out for this frame
	Gizmos::clear();

	//////////////////////////////////////////////////////////////////////////
	// YOUR UPDATE CODE HERE

	// FBX Skeleton and Animation
	//FBXUpdate();

	//m_pMath->Update(m_pCamera.get());
	m_pMath->Update(m_pCameraStateMachine->GetCurrentCamera());
	///----------------------------------------------------------
	// Particles
	//m_pParticleEmitterA->update(deltaTime, m_pCamera->getTransform());
	m_pParticleEmitterA->update(deltaTime, m_pCameraStateMachine->GetCurrentCamera()->getTransform());
	m_pParticleEmitterB->update(deltaTime, m_pCameraStateMachine->GetCurrentCamera()->getTransform());

	//////////////////////////////////////////////////////////////////////////

	// an example of mouse picking
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		double x = 0, y = 0;
		glfwGetCursorPos(m_window, &x, &y);

		// plane represents the ground, with a normal of (0,1,0) and a distance of 0 from (0,0,0)
		glm::vec4 plane(0, 1, 0, 0);
		//m_pickPosition = m_pCamera->pickAgainstPlane((float)x, (float)y, plane);
		m_pickPosition = m_pCameraStateMachine->GetCurrentCamera()->pickAgainstPlane((float)x, (float)y, plane);
	}
	Gizmos::addTransform(glm::translate(m_pickPosition));

	// ...for now let's add a grid to the gizmos
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));

		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	}

#pragma region Choose Draw state
	if (glfwGetKey(m_window, GLFW_KEY_0))
	{
		m_eCurrentDrawState = E_DRAW_STATE_FILL;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_9))
	{
		m_eCurrentDrawState = E_DRAW_STATE_POLY;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_8))
	{
		m_eCurrentDrawState = E_DRAW_STATE_DOT;
	}
	// Cycle Draw State
	if (glfwGetKey(m_window, GLFW_KEY_MINUS))
	{
		//m_eCurrentDrawState--;
		m_eCurrentDrawState = (E_DRAW_STATE)(m_eCurrentDrawState - 1);
		if (m_eCurrentDrawState < 0)
		{
			m_eCurrentDrawState = (E_DRAW_STATE)(E_DRAW_STATE_COUNT - 1);
		}
	}
	else if (glfwGetKey(m_window, GLFW_KEY_EQUAL)) // == GLFW_RELEASE
	{
		//m_eCurrentDrawState++;
		m_eCurrentDrawState = (E_DRAW_STATE)(m_eCurrentDrawState + 1);
		if (m_eCurrentDrawState >= E_DRAW_STATE_COUNT)
		{
			m_eCurrentDrawState = (E_DRAW_STATE)0;
		}
	}
#pragma endregion

	// return true, else the application closes
	return true;
}

void TestApplication::draw() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_pMesh->GetFBO());
	glViewport(0, 0, 512, 512); // 265 lower quarter of the texture
	
	//m_pMesh->createFrame(); //TODO: <- this breaks things?

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
	default:
	{
		glClearColor(0.25f, 0.25f, 0.25f, 1);
		//glClearColor(0.75f, 0.75f, 0.75f, 1);
		break;
	}
	}

	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	//glm::mat4 projView = m_pCamera->getProjectionView();
	glm::mat4 projView = m_pCameraStateMachine->GetCurrentCamera()->getProjectionView();

	//////////////////////////////////////////////////////////////////////////
	// DRAW YOUR THINGS HERE
	// 
	Camera* pCamState = m_pCameraStateMachine->GetCurrentCamera();

	//m_pRender->DrawTexture(m_pCamera.get());
	//m_pRender->DrawTexture(pCamState); //TODO: needed for Soulspear
	m_pRender->DrawTextureP(pCamState);
	Gizmos::addSphere(glm::vec3(0, 5, 0), 0.5f, 8, 8, glm::vec4(1, 1, 0, 1));
//	for (GLushort s = 0; s < 2; ++s)
//	{
	//m_pMesh->RenderRenderTarget();
	//m_pRender->DrawRenderTarget(m_pCamera.get());

	// TODO: V\/ Restore this one - causes crash (access violation) \/V
	//===============================================================================
	//m_pMesh->RenderRenderTarget();
	m_pRender->DrawRenderTarget(pCamState);
	//m_pMesh->RenderRenderTarget();
	//===============================================================================
//	}
	//m_pVertexColoredGrid->draw(projView);
	//m_pSpriteSheetQuad->draw(projView);
	//m_pFBXMesh->draw(projView);
	RenderFBX(pCamState);
	//FBXSkeletonRender();
	// FBX Skeleton and Animation
	//FBXUpdate();

	// Particles
	m_pParticleEmitterA->draw(projView);
	m_pParticleEmitterB->draw(projView);

	//////////////////////////////////////////////////////////////////////////

	//draw our meshes, or gizmos, to the render target
	// display the 3D gizmos
	Gizmos::draw(projView);

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(projView);
}

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

		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);

		glEnableVertexAttribArray(1); // normal
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);

		glEnableVertexAttribArray(2); // Texture on FBX model via coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::TexCoord1Offset);

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
							void main() { vNormal = Normal; vTexCoord = TexCoord; \
							gl_Position = ProjectionView * Position; }";

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

	const unsigned int id = m_pRender->GetTextureByName("soulspear_d"); //TODO: soulspear
	//const unsigned int id = m_pRender->GetTextureByName("Pyro_D");
	glBindTexture(GL_TEXTURE_2D, id);

	loc = glGetUniformLocation(m_program_ID, "diffuse");
	glUniform1i(loc, 0);

	/*unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	unsigned int uiTime = glGetUniformLocation(m_programID, "time");

	glUniform1f(uiHeightScale, fHeightScale);
	glUniform1f(uiTime, fTime); */

	// bind our vertex array object and draw the mesh
	for (unsigned int i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		//TODO: FBX texture
		//FBXTexture* textureC = m_fbx->getTextureByName("./data/textures/crate.png");// , &imageWidth, &imageHeight, &imageFormat, STBI_default);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glBindVertexArray(glData[0]); //TODO: replace m_VAO with VAO
		//glBindVertexArray(Geom->GetVAO()); //TODO: replace m_VAO with VAO
		//unsigned int indexCount = (a_iRows - 1) * (a_iCols - 1) * 6; //TODO: m_iIndexCount = this formula
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
}

// Skeletion/ bones vertex shader
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
							layout(location=2) in vec4 Tanget; \
							layout(location=3) in vec2 TexCoord; \
							layout(location=4) in vec4 Weights; \
							layout(location=5) in vec4 Indices; \
							\
							out vec3 frag_normal; \
							out vec3 frag_position; \
							out vec3 frag_tangent; \
							out vec3 frag_bitangent; \
							out vec2 frag_texcoord; \
							\
							uniform mat4 ProjectionView; \
							uniform mat4 Global; \
							// We need to give our bone array a limit \
							const int MAX_BONES = 128; \
							uniform mat4 bones[MAX_BONES]; \
							void main() { frag_position = Position.xyz;\
							frag_normal = Normal.xyz; \
							frag_tangent = Tangent.xyz; \
							frag_bitangent = cross(Normal.xyz, Tangent.xyz); \
							frag_texcoord = TexCoord; \
							\
							// Cast the indices to integer's so they can index an array \
							ivec4 index = ivec4(Indices); \
							// Sample bones and blend up to 4 \
							vec4 P = bones[Index.x] * Position * Weights.x; \
							P += bones[Index.y] * Position * Weights.y; \
							P += bones[Index.z] * Position * Weights.z; \
							P += bones[Index.w] * Position * Weights.w; \
							gl_Position = ProjectionView * Global * P; }";

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	int success = GL_FALSE;
	unsigned int iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(iVertexShader);

	m_program_ID = glCreateProgram();
	glAttachShader(m_program_ID, iVertexShader);
	glLinkProgram(m_program_ID);

	glGetProgramiv(m_program_ID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
	int infoLogLength = 0;
	glGetProgramiv(m_program_ID, GL_INFO_LOG_LENGTH, &infoLogLength);
	char* infoLog = new char[infoLogLength];

	glGetProgramInfoLog(m_program_ID, infoLogLength, 0, infoLog);
	printf("Error: Failed to link shader program!\n");
	printf("%s\n", infoLog);
	delete[] infoLog;
	}

	glDeleteShader(iVertexShader);
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
	FBXSkeleton* skeletion = m_pFbx->getSkeletonByIndex(0);
	skeletion->updateBones();

	GLint bones_location = glGetUniformLocation(m_program_ID, "bones");
	glUniformMatrix4fv(bones_location, skeletion->m_boneCount, GL_FALSE, (GLfloat*)skeletion->m_bones);

	FBXAnimation* animation = m_pFbx->getAnimationByIndex(0);

	m_timer = 2.0f;

	// Evaluate the animation to update bones
	skeletion->evaluate(animation, m_timer);

	for (GLuint uiBone_index = 0;
	uiBone_index < skeletion->m_boneCount;
		++uiBone_index)
	{
		skeletion->m_nodes[uiBone_index]->updateGlobalTransform();
	}
}