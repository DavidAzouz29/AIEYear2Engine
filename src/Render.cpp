/// <summary>
/// File Name:		Render.cpp
/// Author: 		David Azouz
/// Date Created: 	8/02/16
/// Date Modified: 	8/02/16
///----------------------------------------------------------
/// Brief: A Render Class that enables 
/// viewed: 
/// GLfloat http://r3dux.org/2011/05/simple-opengl-keyboard-and-mouse-fps-controls/
/// http://gamedev.stackexchange.com/questions/13436/glm-euler-angles-to-quaternion
/// https://open.gl/textures
/// http://www.cplusplus.com/reference/iterator/begin/
///
/// ***EDIT***
/// - Camera	 	- David Azouz 2/02/16
/// - Camera zoom 	- David Azouz 5/02/16
/// - Moved texture related content to the "Texture" class 	- David Azouz 13/05/16
/// 
/// TODO:
/// DrawGeom(m_projectionViewMatrix or in this case m4ProjectionTrans
/// FBXTexture
///
/// would have textures and shaders
/// 
/// </summary>
/// ----------------------------------------------------------
#include "Render.h"
#include "Camera\Camera.h"
//#include "Mesh.h" //TODO:
#include "Renderable.h"
//#include "Texture.h"
//#include "gl_core_4_4.h"
#include "VertexData.h"

//#define STB_IMAGE_IMPLEMENTATION //TODO: delete me
//#include <stb_image.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm/vec2.hpp>
#include <GLFW\glfw3.h>

//#include <cstdio> //TODO: remove once Texture class is working

//TODO: delete me
/*Render::Render() :
	fHeightScale(2.0f),
	fTime(0.0f)
{
	//m_pMesh = std::make_shared<Mesh>();
} */

/*bool Render::Create()
{
	//m_pRender = std::make_shared<Render>(); //TODO: I need this somewhere
	//m_pTexture = std::make_shared<Texture>();
	
	/*m_pRender->generateGrid(uiGrid, uiGrid);
	//m_pRender->InitGeometry();
	// -----------------------

	//m_pRender->TextureLoader();

	//unsigned int id = m_pRender->TextureInit("./data/textures/crate.png");
	//m_pRender->AddTexture("crate", id);

	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, id); 

	//TextureLoader();

	//TODO: remove?
	GLuint id = m_pTexture->TextureInit("./data/models/soulspear/soulspear_diffuse.tga");
	m_pTexture->AddTexture("soulspear_d", id);

	id = m_pTexture->TextureInit("./data/models/soulspear/soulspear_normal.tga");
	m_pTexture->AddTexture("soulspear_n", id);

	id = m_pTexture->TextureInit("./data/models/characters/Pyro/Pyro_D.tga");
	m_pTexture->AddTexture("Pyro_D", id);

	id = m_pTexture->TextureInit("./data/models/characters/Pyro/Pyro_N.tga");
	m_pTexture->AddTexture("Pyro_N", id);

	id = m_pTexture->TextureInit("./data/models/characters/Pyro/Pyro_S.tga");
	m_pTexture->AddTexture("Pyro_S", id); 

	//m_pRender->RenderTexture(); //For the Render Target?

	//For render target? //TODO
	//RenderTexture();
	//=======================================================
	//m_pRender.get()->RenderTargetLoader();
	//RenderTargetLoader();
	//GetSharedPointer()->CreateRenderTargetQuad();
	//=======================================================
	return true;
} */

GLvoid Render::InitGeometry()
{
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	//uniform float time;
	//uniform float heightScale;

	const GLchar* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Colour; \
							out vec4 vColour; \
							uniform mat4 ProjectionView; \
							uniform float time; \
							uniform float heightScale; \
							void main() { vColour = Colour; vec4 P = Position; P.y += tan ( time + Position.x ) * heightScale; gl_Position = ProjectionView * P; }";

	const GLchar* fsSource = "#version 410\n \
							in vec4 vColour; \
							out vec4 FragColor; \
							void main() { FragColor = vColour; }";

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	GLint success = GL_FALSE;
	GLuint iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(iVertexShader, 1, (const GLchar**)&vsSource, 0);
	glCompileShader(iVertexShader);
	glShaderSource(iFragmentShader, 1, (const GLchar**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, iVertexShader);
	glAttachShader(m_programID, iFragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(iFragmentShader);
	glDeleteShader(iVertexShader);
}

GLvoid Render::DrawGeometry(const Camera& a_pCamState)
{
	fTime = static_cast<GLfloat>(glfwGetTime());
	glUseProgram(m_programID);
	GLuint projectionViewUniform = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, &a_pCamState.getProjectionView()[0][0]); //m_projectionViewMatrix

	GLuint uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	GLuint uiTime = glGetUniformLocation(m_programID, "time");

	glUniform1f(uiHeightScale, fHeightScale);
	glUniform1f(uiTime, fTime);

	glBindVertexArray(m_mesh.GetVAO()); //TODO: replace m_VAO with VAO
							  //unsigned int indexCount = (a_iRows - 1) * (a_iCols - 1) * 6; //TODO: m_iIndexCount = this formula
	glDrawElements(GL_TRIANGLES, m_mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
}

/// ----------------------------------------------------------
/// FBX Loader
/// ----------------------------------------------------------
/*void Render::FBXLoader()
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
/// ----------------------------------------------------------
} */

/// ----------------------------------------------------------
/// Texture
/// ----------------------------------------------------------
/*GLuint Render::TextureInit(const GLchar* name)
{
	GLint imageWidth = 0, imageHeight = 0, imageFormat = 0;

	/// ----------------------------------------------------------
	// This call will read in the default texel data for the crate.png.
	// In the case the image stores RGB values at 515x512 resolution.
	/// ----------------------------------------------------------
	// Load diffuse map
	GLubyte* data = stbi_load(name, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	/// ----------------------------------------------------------
	GLuint id;
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
	GLuint uiNormal;
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
	const GLchar* vsSource = "#version 410\n \
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
	const GLchar* fsSource = "#version 410\n \
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
	GLuint iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const GLchar**)&vsSource, 0);
	glCompileShader(iVertexShader);

	GLuint iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(iFragmentShader, 1, (const GLchar**)&fsSource, 0);
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

	glDeleteShader(iVertexShader);
	glDeleteShader(iFragmentShader);
}

struct Vertex
{
	GLfloat x, y, z, w;
	GLfloat nx, ny, nz, nw;
	GLfloat tx, ty, tz, tw;
	GLfloat s, t;
};

GLvoid Render::RenderTexture()
{
	/ *float vertexData[] = {
		-5,0,5,1,0,1,
		5,0,5,1,1,1,
		5,0,-5,1,1,0,
		-5,0,-5,1,0,0 }; */
/*
	// Recreates the textured quad, except that they now contain normal that point up (0,1,0)
	// and tangents to point in the direction of the texture's S axis (or U axis) which is position X (1,0,0).
	Vertex vertexData[] =
	{
		{ -5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
		{  5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
		{  5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
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
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((GLchar*)0) + 32);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//
	//Render::VAO = m_VAO
	//m_pMesh->SetVAO(m_VAO);
	//delete[] vertexData; //TODO: more clean up code?
}

//TODO: remove once Texture class is working
GLvoid Render::DrawTexture(Camera* cam)
{
	// use our texture program
	glUseProgram(m_programID);

	// bind the camera
	GLint loc = glGetUniformLocation(m_programID, "ProjectionView");
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
	glBindVertexArray(m_mesh.GetVAO()); //TODO: replace m_VAO with VAO
	//glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

GLvoid Render::DrawTextureP(Camera* cam)
{
	// use our texture program
	glUseProgram(m_programID);

	// bind the camera
	GLint loc = glGetUniformLocation(m_programID, "ProjectionView");
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
	glBindVertexArray(m_mesh.GetVAO()); //TODO: replace m_VAO with VAO
										  //glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

GLvoid Render::AddTexture(const GLchar* name, const GLuint id)
{
	m_textures.insert(std::pair<const std::string, const GLuint>(name, id));
}

GLuint Render::GetTextureByName(const GLchar* name)
{
	//std::vector<std::string>::iterator iter = m_textures.begin();
	return m_textures[name];
} */

Meshes* CreateMeshFromBuffers(SimpleVertex* vertex_data, unsigned int vertex_count,
	unsigned int *index_data, unsigned int index_count,
	Material material)
{
	if (!vertex_data || !index_data) return 0;

	//NOTE(aidan): all data for the scene amortized into a single buffer for easy deallocation
	//			This is easy to do as its all just simple POD types
	size_t total_bytes = sizeof(Meshes) +
		sizeof(SubMesh) +
		sizeof(SimpleVertex) * vertex_count +
		sizeof(unsigned int) * index_count;
	void* memory = calloc(total_bytes, 1);

	Meshes* result = (Meshes*)memory;
	SubMesh* sub_mesh = (SubMesh*)(result + 1);
	SimpleVertex* verts = (SimpleVertex*)(sub_mesh + 1);
	unsigned int* indices = (unsigned int*)(verts + vertex_count);

	memcpy(verts, vertex_data, sizeof(SimpleVertex) * vertex_count);
	memcpy(indices, index_data, sizeof(unsigned int) * index_count);

	result->sub_meshes = sub_mesh;
	result->sub_mesh_count = 1;
	result->vertex_count = vertex_count;
	result->vertex_data = verts;

	sub_mesh->index_count = index_count;
	sub_mesh->index_data = indices;
	sub_mesh->material = material;

	glGenBuffers(1, &result->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, result->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * result->vertex_count, result->vertex_data, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &sub_mesh->vao);
	glBindVertexArray(sub_mesh->vao);

	glGenBuffers(1, &sub_mesh->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub_mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sub_mesh->index_count, sub_mesh->index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(sizeof(float) * 6));

	glBindVertexArray(0);

	return result;
}

void FreeMesh(Meshes* mesh)
{
	*mesh = {};
	free(mesh);
}

void RebuildVertexBuffer(Meshes* mesh)
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->vertex_count * sizeof(SimpleVertex), mesh->vertex_data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Renderer::Renderer()
{
	LoadShader("./data/shaders/main_shader.vs", 0, "./data/shaders/main_shader.fs", &main_shader);
}

void Renderer::PushMesh(Meshes* mesh, mat4 transform)
{
	RenderItem item = {};
	item.mesh = mesh;
	item.transform = transform;

	render_queue.push_back(item);
}