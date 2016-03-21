/// <summary>
/// File Name:		Render.cpp
/// Author: 		David Azouz
/// Date Created: 	8/02/16
/// Date Modified: 	8/02/16
///----------------------------------------------------------
/// Brief: A FlyCamera Class that enables movement/ navigation through the world
/// viewed: 
/// GLfloat http://r3dux.org/2011/05/simple-opengl-keyboard-and-mouse-fps-controls/
/// http://gamedev.stackexchange.com/questions/13436/glm-euler-angles-to-quaternion
/// https://open.gl/textures
/// http://www.cplusplus.com/reference/iterator/begin/
///
/// ***EDIT***
/// - Camera	 	- David Azouz 2/02/16
/// - Camera zoom 	- David Azouz 5/02/16
/// - Return 0 added at end 	- David Azouz 6/02/16
/// 
/// TODO: Clean up
/// Camera zoom with scroll
/// search for TODO:
/// DrawGeom(m_projectionViewMatrix or in this case m4ProjectionTrans
/// FBXTexture
///
/// would have textures and shaders
/// 
/// </summary>
/// ----------------------------------------------------------

#include "Render.h"
#include "Mesh.h"
#include "Camera\Camera.h"
#include "gl_core_4_4.h"
//#include "VertexData.h"

//#define STB_IMAGE_IMPLEMENTATION //TODO: delete me
#include <stb_image.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <GLFW\glfw3.h>

#include <cstdio>


Render::Render() :
	fHeightScale(2.0f),
	fTime(0.0f)
{
	//m_pMesh = std::make_shared<Mesh>();
}

Render::~Render()
{
	//CleanupOpenGLBuffers(m_fbx);

	//glDeleteProgram(m_programID);
}

bool Render::Create()
{
	//m_pRender = std::make_shared<Render>(); //TODO: I need this somewhere
	// -----------------------
	// Creates Grid
	// -----------------------
	GLint uiGrid = 21;
	/*m_pRender->generateGrid(uiGrid, uiGrid);
	//m_pRender->InitGeometry();
	// -----------------------

	//m_pRender->TextureLoader();

	//unsigned int id = m_pRender->TextureInit("./data/textures/crate.png");
	//m_pRender->AddTexture("crate", id);

	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, id); */

	unsigned int id = TextureInit("./data/models/soulspear/soulspear_diffuse.tga");
	AddTexture("soulspear_d", id);

	id = TextureInit("./data/models/soulspear/soulspear_normal.tga");
	AddTexture("soulspear_n", id);

	id = TextureInit("./data/models/characters/Pyro/Pyro_D.tga");
	AddTexture("Pyro_D", id);

	id = TextureInit("./data/models/characters/Pyro/Pyro_N.tga");
	AddTexture("Pyro_N", id);

	id = TextureInit("./data/models/characters/Pyro/Pyro_S.tga");
	AddTexture("Pyro_S", id);

	//m_pRender->RenderTexture(); //For the Render Target?

	//=======================================================
	//m_pRender.get()->RenderTargetLoader();
	RenderTargetLoader();
	GetSharedPointer()->CreateRenderTargetQuad();
	//=======================================================
	return false;
}

//function to create a grid
GLvoid Render::generateGrid(const unsigned int a_iRows, const unsigned int a_iCols)
{
	Vertex_PositionColor* aoVertices = new Vertex_PositionColor[a_iRows * a_iCols];
	for (unsigned short r = 0; r < a_iRows; ++r)
	{
		for (unsigned short c = 0; c < a_iCols; ++c)
		{
			aoVertices[r * a_iCols + c].position = glm::vec4((float)c, 0.0f, (float)r, 1.0f);

			//create some arbitary colour based off something
			//that might not be related to tiling a texture
			glm::vec3 colour = glm::vec3(sinf((c / (float)(a_iCols - 1.0f)) * (r / (float)(a_iRows - 1.0f))));
			aoVertices[r * a_iCols + c].color = glm::vec4(colour, 1.0f);
		}
	}

	/// ----------------------------------------------------------
	/// we'll do more here soon!
	/// ----------------------------------------------------------
	/// defining index count based off quad count (2 triangles per quad)
	/// ----------------------------------------------------------
	unsigned int* auiIndices = new unsigned int[(a_iRows - 1) * (a_iCols - 1) * 6];
	
	unsigned int index = 0;
	for (unsigned int r = 0; r < (a_iRows - 1); ++r)
	{
		for (unsigned int c = 0; c < (a_iCols - 1); ++c)
		{
			// triangle 1
			auiIndices[index++] = r * a_iCols + c;
			auiIndices[index++] = (r + 1) * a_iCols + c;
			auiIndices[index++] = (r + 1) * a_iCols + (c + 1);

			// triangle 2
			auiIndices[index++] = r * a_iCols + c;
			auiIndices[index++] = (r + 1) * a_iCols + (c + 1);
			auiIndices[index++] = r * a_iCols + (c + 1);
		}
	}

	m_pMesh->SetIndexCount((a_iRows - 1) * (a_iCols - 1) * 6);

#pragma region Big Old
#pragma region Old
	/// ----------------------------------------------------------
	/// create and bind buffers to a vertex array object
	/// Generates a VBO
	/// ----------------------------------------------------------
	/* TODO: Delete?  glGenBuffers(1, &m_VBO); */
	/*glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (a_iRows * a_iCols) * sizeof(Vertex_PositionColor), aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), (void*)(sizeof(glm::vec4)));*/

	/* TODO: Delete?  glBindBuffer(GL_ARRAY_BUFFER, 0); */

	/// ----------------------------------------------------------
	/// OpenGL Index Buffer
	/// ----------------------------------------------------------
	/* TODO: Delete?  glGenBuffers(1, &m_IBO); */

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (a_iRows - 1) * (a_iCols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	*//* TODO: Delete?  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); */
#pragma endregion

	/// ----------------------------------------------------------
	/// Thing
	/// ----------------------------------------------------------
/*	// Generate our GL Buffers
	// Let's move these so that they are all generated together
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Add the following line to generate a VertexArrayObject
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	//... Code Segment here to bind and fill VBO + IBO
	//
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (a_iRows * a_iCols) * sizeof(Vertex_PositionColor), aoVertices, GL_STATIC_DRAW);

	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (a_iRows - 1) * (a_iCols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), (void*)(sizeof(glm::vec4)));

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//
	//Render::VAO = m_VAO */
#pragma endregion

	delete[] aoVertices;
}

GLvoid Render::InitGeometry()
{
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	//uniform float time;
	//uniform float heightScale;

	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Colour; \
							out vec4 vColour; \
							uniform mat4 ProjectionView; \
							uniform float time; \
							uniform float heightScale; \
							void main() { vColour = Colour; vec4 P = Position; P.y += tan ( time + Position.x ) * heightScale; gl_Position = ProjectionView * P; }";

	const char* fsSource = "#version 410\n \
							in vec4 vColour; \
							out vec4 FragColor; \
							void main() { FragColor = vColour; }";

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	int success = GL_FALSE;
	unsigned int iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(iVertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(iVertexShader);
	glShaderSource(iFragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, iVertexShader);
	glAttachShader(m_programID, iFragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(iFragmentShader);
	glDeleteShader(iVertexShader);
}

GLvoid Render::DrawGeometry(Camera* cam)
{
	fTime = static_cast<float>(glfwGetTime());
	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(cam->getProjectionView())); //m_projectionViewMatrix

	unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	unsigned int uiTime = glGetUniformLocation(m_programID, "time");

	glUniform1f(uiHeightScale, fHeightScale);
	glUniform1f(uiTime, fTime);

	glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
							  //unsigned int indexCount = (a_iRows - 1) * (a_iCols - 1) * 6; //TODO: m_iIndexCount = this formula
	glDrawElements(GL_TRIANGLES, m_pMesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
}
/*
void Render::FBXLoader()
{
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Normal; \
							out vec4 vNormal; \
							uniform mat4 ProjectionView; \
							void main() { vNormal = Normal; \
							gl_Position = ProjectionView * Position; }";

	const char* fsSource = "#version 410\n \
							in vec4 vNormal; \
							out vec4 FragColor; \
							void main() { FragColor = vec4(1,1,1,1); }";

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

	m_programID = glCreateProgram();
	glAttachShader(m_programID, iVertexShader);
	glAttachShader(m_programID, iFragmentShader);
	glLinkProgram(m_programID);

	// glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	// if (success == GL_FALSE)
	// {
	// 	int infoLogLength = 0;
	// 	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	// 	char* infoLog = new char[infoLogLength];
	//
	// 	glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
	// 	printf("Error: Failed to link shader program!\n");
	// 	printf("%s\n", infoLog);
	// 	delete[] infoLog;
	// }

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
void Render::RenderFBX(Camera* cam)
{
	glUseProgram(m_programID);

	// bind the camera
	int loc = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(cam->GetProjectionView()[0][0])); //m_projectionViewMatrix

	// unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	// unsigned int uiTime = glGetUniformLocation(m_programID, "time");
	//
	// glUniform1f(uiHeightScale, fHeightScale);
	// glUniform1f(uiTime, fTime);

	// bind our vertex array object and draw the mesh
	for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);

		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glBindVertexArray(glData[0]); //TODO: replace m_VAO with VAO
								  //unsigned int indexCount = (a_iRows - 1) * (a_iCols - 1) * 6; //TODO: m_iIndexCount = this formula
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
} */

/// ----------------------------------------------------------
/// Texture
/// ----------------------------------------------------------
unsigned int Render::TextureInit(const char* name)
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	/// ----------------------------------------------------------
	// This call will read in the default texel data for the crate.png.
	// In the case the image stores RGB values at 515x512 resolution.
	/// ----------------------------------------------------------
	// Load diffuse map
	unsigned char* data = stbi_load(name, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	/// ----------------------------------------------------------
	unsigned int id;
	// Generate an OpenGL texture handle.
	glGenTextures(1, &id);
	// Bind the texture to the correct slot for the dimension, in this case 2-D.
	glBindTexture(GL_TEXTURE_2D, id);
	// Specify the data for the texture, including the format, resolution and variable type.
	// Out data is an unsigned char, therefor it should be GL_UNSIGNED_BYTE.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data); //TODO: delete texture.

	// load normal map
	data = stbi_load(name, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	/// ----------------------------------------------------------
	unsigned int uiNormal;
	// Generate an OpenGL texture handle.
	glGenTextures(1, &uiNormal);
	// Bind the texture to the correct slot for the dimension, in this case 2-D.
	glBindTexture(GL_TEXTURE_2D, uiNormal);
	// Specify the data for the texture, including the format, resolution and variable type.
	// Out data is an unsigned char, therefor it should be GL_UNSIGNED_BYTE.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data); //TODO: delete texture.

	return id;
}
GLvoid Render::TextureLoader()
{
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							layout(location=2) in vec4 Normal; \
							layout(location=3) in vec4 Tangent; \
							out vec2 vTexCoord; \
							out vec3 vNormal; \
							out vec3 vTangent; \
							out vec3 vBiTangent; \
							uniform mat4 ProjectionView; \
							void main() { \
							vTexCoord = TexCoord; \
							vNormal = Normal.xyz; \
							vTangent = Tangent.xyz; \
							vBiTangent = cross(vNormal, vTangent); \
							gl_Position = ProjectionView * Position; \
							}";

	// RGB x 2 - 1 to move it from RGB to XYZ, or 
	// XYZ x 0.5 + 0.5 to move it from XYZ to RGB.
	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							in vec3 vNormal; \
							in vec3 vTangent; \
							in vec3 vBiTangent; \
							out vec4 FragColor; \
							uniform vec3 LightDir; \
							uniform sampler2D diffuse; \
							uniform sampler2D normal; \
							void main() { \
							mat3 TBN = mat3( \
							normalize( vTangent ), \
							normalize( vBiTangent ), \
							normalize( vNormal )); \
							vec3 N = texture(normal, \
							vTexCoord.xyz * 2 - 1; \
							float d = max( 0, dot( \
							normalize( TBN * N ), \
							normalize( LightDir ))); \
							FragColor = texture(diffuse, vTexCoord); \
							FragColor.rgb = FragColor.rgb * d;}";

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

	m_programID = glCreateProgram();
	glAttachShader(m_programID, iVertexShader);
	glAttachShader(m_programID, iFragmentShader);
	glLinkProgram(m_programID);

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

struct Vertex
{
	float x, y, z, w;
	float nx, ny, nz, nw;
	float tx, ty, tz, tw;
	float s, t;
};

GLvoid Render::RenderTexture()
{
	/*float vertexData[] = {
		-5,0,5,1,0,1,
		5,0,5,1,1,1,
		5,0,-5,1,1,0,
		-5,0,-5,1,0,0 }; */

	// Recreates the textured quad, except that they now contain normal that point up (0,1,0)
	// and tangents to point in the direction of the texture's S axis (or U axis) which is position X (1,0,0).
	Vertex vertexData[] =
	{
		{ -5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
		{  5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
		{  5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ -5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 }
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};
	
	// Add the following line to generate a VertexArrayObject
	glGenVertexArrays(1, &m_pMesh->GetVAO());
	glBindVertexArray(m_pMesh->GetVAO());

	// ----------------------------------------------------------
	// Generate our GL Buffers
	// Let's move these so that they are all generated together
	// ----------------------------------------------------------
	glGenBuffers(1, &m_pMesh->GetVBO());
	//... Code Segment here to bind and fill VBO + IBO
	//
	glBindBuffer(GL_ARRAY_BUFFER, m_pMesh->GetVBO());
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_pMesh->GetIBO());
	// 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pMesh->GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 48);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 32);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//
	//Render::VAO = m_VAO
	//m_pMesh->SetVAO(m_VAO);
	//delete[] vertexData; //TODO: more clean up code?
}
GLvoid Render::DrawTexture(Camera* cam)
{
	// use our texture program
	glUseProgram(m_programID);

	// bind the camera
	int loc = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(cam->getProjectionView()[0][0]));

	// unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	// unsigned int uiTime = glGetUniformLocation(m_programID, "time");
	// 
	// glUniform1f(uiHeightScale, fHeightScale);
	// glUniform1f(uiTime, fTime);

	//glBindTexture(GL_TEXTURE_2D, m_textures["crate"]);

	// Set texture slots
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures["soulspear_d"]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textures["soulspear_n"]);
	//glBindTexture(GL_TEXTURE_2D, m_normalmap["soulspear_n"]);

	// tell the shader where it is
	loc = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(m_programID, "normal");
	glUniform1i(loc, 1);

	// bind the light
	glm::vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_programID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	// draw
	glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
	//glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

GLvoid Render::DrawTextureP(Camera* cam)
{
	// use our texture program
	glUseProgram(m_programID);

	// bind the camera
	int loc = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(cam->getProjectionView()[0][0]));

	//glBindTexture(GL_TEXTURE_2D, m_textures["crate"]);

	// Set texture slots
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures["Pyro_D"]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textures["Pyro_N"]);

	//glActiveTexture(GL_TEXTURE2); //TODO: Pyro
	//glBindTexture(GL_TEXTURE_2D, m_textures["Pyro_S"]);

	// tell the shader where it is
	loc = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(m_programID, "normal");
	glUniform1i(loc, 1);

	// bind the light
	glm::vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_programID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	// draw
	glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
										  //glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

GLvoid Render::AddTexture(const char* name, const unsigned int id)
{
	m_textures.insert(std::pair<const std::string, const unsigned int>(name, id));
}

unsigned int Render::GetTextureByName(const char* name)
{
	//std::vector<std::string>::iterator iter = m_textures.begin();
	return m_textures[name];
}

GLvoid Render::RenderTargetLoader()
{
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							out vec2 vTexCoord; \
							uniform mat4 ProjectionView; \
							void main() { \
							vTexCoord = TexCoord; \
							gl_Position = ProjectionView * Position; \
							}";

	// RGB x 2 - 1 to move it from RGB to XYZ, or 
	// XYZ x 0.5 + 0.5 to move it from XYZ to RGB.
	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							out vec4 FragColor; \
							uniform sampler2D diffuse; \
							void main() { \
							FragColor = texture(diffuse, vTexCoord);}";

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	//if (!m_program.create(vsSource, fsSource)) return false; //TODO:
	int success = GL_FALSE;
	unsigned int iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(iVertexShader);

	unsigned int iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(iFragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, iVertexShader);
	glAttachShader(m_programID, iFragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
	int infoLogLength = 0;
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	char* infoLog = new char[infoLogLength];

	glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
	printf("Error: Failed to link shader program!\n");
	printf("%s\n", infoLog);
	delete[] infoLog;
	} //*/

	glDeleteShader(iVertexShader);
	glDeleteShader(iFragmentShader);

	m_pMesh.get()->CreateFrame();
}