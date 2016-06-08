/// ----------------------------------------------------------
/// <summary>
/// File Name:		Texture.cpp
/// Author: 		David Azouz
/// Date Created: 	13/05/16
/// Date Modified: 	13/05/16
///----------------------------------------------------------
/// Brief: A Texture Class that enables more than one object to share the same texture.
/// Notes:
/// RGB x 2 - 1 to move it from RGB to XYZ, or 
/// XYZ x 0.5 + 0.5 to move it from XYZ to RGB.
/// viewed: 
/// 
///
/// ***EDIT***
/// - Texture class created	 	- David Azouz 13/05/16
/// -  	- David Azouz 5/02/16
/// -  	- David Azouz 6/02/16
/// 
/// TODO: texture class that FBX class has a copy of
/// 
/// </summary>
/// ----------------------------------------------------------

#include "Texture.h"
#include "Camera.h"
#include "Helpers.h"
#include "Mesh.h"

//#include <glm\glm.hpp>
//#include <glm\ext.hpp>
#include <GLFW\glfw3.h>

#include <cstdio>

const Texture Texture::Invalid = Texture();

/*
Texture::Texture()
{
	Create();
	//TODO: remove Create() and add these?
	//CreateBuffers();
	//CreateTextureShader();
}


Texture::~Texture()
{
//	glDeleteVertexArrays(2, m_vao);
//	glDeleteBuffers(2, m_vbo);

	// delete the shaders
	glDeleteProgram(m_textureID);
} */

bool Texture::Create()
{
	CreateBuffers();
	CreateTextureShader();

	//TODO: from Render: steps:
	/*GLvoid CreateBuffers(); //RenderTexture();
	TextureInit
	AddTexture
	GLvoid CreateBuffers(); //RenderTexture(); 	*/

	return false;
}

struct Vertex
{
	GLfloat x, y, z, w;
	GLfloat nx, ny, nz, nw;
	GLfloat tx, ty, tz, tw;
	GLfloat s, t;
};

GLvoid Texture::CreateBuffers()
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
		{ 5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
		{ 5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ -5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 }
	};
	GLuint indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Add the following line to generate a VertexArrayObject
	glGenVertexArrays(1, &m_mesh.GetVAO());
	glBindVertexArray(m_mesh.GetVAO());

	// ----------------------------------------------------------
	// Generate our GL Buffers
	// Let's move these so that they are all generated together
	// ----------------------------------------------------------
	glGenBuffers(1, &m_mesh.GetVBO());
	//... Code Segment here to bind and fill VBO + IBO
	//
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.GetVBO());
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_mesh.GetIBO());
	// 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((GLchar*)0) + 48);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((GLchar*)0) + 16);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((GLchar*)0) + 32);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//
	//Render::VAO = m_VAO
	//m_pMesh->SetVAO(m_VAO);
	//delete[] vertexData; //TODO: more clean up code?
}

GLvoid Texture::CreateTextureShader()
{
	GLint success = GL_FALSE;

	/// ----------------------------------------------------------
	/// Create shaders via loading files
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into shorts for processign by OpenGL.
	/// ----------------------------------------------------------
	GLushort vsSource = LoadShader(GL_VERTEX_SHADER,	"./data/shaders/texture.vert");
	GLushort fsSource = LoadShader(GL_FRAGMENT_SHADER,	"./data/shaders/texture.frag");

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	m_textureID = glCreateProgram();
	glAttachShader(m_textureID, vsSource);
	glAttachShader(m_textureID, fsSource);
	glLinkProgram(m_textureID);

	//TODO: clean up - look at Render.cpp "TextureLoader" and GPuPa..Em...cpp for tips
	/*int success = GL_FALSE;
	//GLuint iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const GLchar**)&vsSource, 0);
	glCompileShader(iVertexShader);

	GLuint iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(iFragmentShader, 1, (const GLchar**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, iVertexShader);
	glAttachShader(m_programID, iFragmentShader);
	glLinkProgram(m_programID); */

	/// ----------------------------------------------------------
	/// Error checking
	/// ----------------------------------------------------------
	glGetProgramiv(m_textureID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_textureID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_textureID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

//	glDeleteShader(iVertexShader);
//	glDeleteShader(iFragmentShader);
	glDeleteShader(vsSource);
	glDeleteShader(fsSource);
}

GLvoid Texture::Draw(const Camera& m_pCamState)
{
	//TODO: HELP - is this where this should be done?
	DrawTexture(m_pCamState, m_textures["soulspear_d"], m_textures["soulspear_n"]);
	DrawTexture(m_pCamState, m_textures["Pyro_D"], m_textures["Pyro_N"]);
}

/// --------------------------------------------------------------------
/// <summary>
/// Draws Textures 
/// <para><param>P1: Cam</param></para>
/// <para><param>P2: Texture 1 - generally the diffuse.</param></para>
/// <para><param>P3: Texture 2 - generally the normal.</param></para>
/// <para><param>P3: Texture 3 - generally the specular.</param></para>
/// <example> m_textures["soulspear_d"] </example>
/// </summary>
/// --------------------------------------------------------------------
GLvoid Texture::DrawTexture(const Camera& m_pCamState, GLuint a_uiTexture1, GLuint a_uiTexture2)
{
	// use our texture program
	glUseProgram(m_textureID);

	// bind the camera
	GLint loc = glGetUniformLocation(m_textureID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_pCamState.getProjectionView()[0][0]));

	/* unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	// unsigned int uiTime = glGetUniformLocation(m_programID, "time");
	// 
	// glUniform1f(uiHeightScale, fHeightScale);
	// glUniform1f(uiTime, fTime);

	//glBindTexture(GL_TEXTURE_2D, m_textures["crate"]); */

	// Set texture slots
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, a_uiTexture1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, a_uiTexture2);
	//glBindTexture(GL_TEXTURE_2D, m_normalmap["soulspear_n"]);

	// tell the shader where it is
	loc = glGetUniformLocation(m_textureID, "diffuse");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(m_textureID, "normal");
	glUniform1i(loc, 1);

	// bind the light
	glm::vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_textureID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	// draw
	glBindVertexArray(m_mesh.GetVAO()); //TODO: replace m_VAO with VAO
	//glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}