/// ***EDIT***
/// - Texture class added	 	- David Azouz 13/05/16
///
/// TODO: add texture?

#include "FBXModel.h"
#include "FBXFile.h"
#include "Camera\Camera.h"
#include "Render.h"
#include "Renderable.h"
#include "Texture.h"

#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/ext.hpp>

using glm::vec3;
using glm::mat4;

// TOOD: FBXModel - give a path or file

FBXModel::FBXModel(const GLchar* szFileName) : m_timer(0)
{
	//TODO: move to create
	m_pFbx = std::make_shared<FBXFile>();
	if (!m_pFbx->load(szFileName, FBXFile::UNITS_METER))
	{
		printf("Fail to load FBX.");
	}
	LoadFBXTextures(m_pFbx.get());
	// goes through all loaded textures and creates their GL versions
	m_pFbx->initialiseOpenGLTextures();
}

// -----------------------
// Clean up our FBX related resources
// -----------------------
FBXModel::~FBXModel()
{
	CleanupOpenGLBuffers(m_pFbx.get());
}

// So far only used for FBX Skeleton/ animation
bool FBXModel::Create()
{
	//m_pFbx->load("./data/models/stanford/Bunny.fbx");
	//m_pFbx->load("./data/models/soulspear/soulspear.fbx");
	//m_pFbx->load("./data/models/characters/Pyro/pyro.fbx", FBXFile::UNITS_METER);
	//FBXLoader(); // Needed if FBX without Animation
	FBXSkeletonLoader();

	CreateOpenGLBuffers(m_pFbx.get());

	return true;
}

// Used for FBX Skeleton and Animation
GLvoid FBXModel::Update()
{
	//if no skeleton - don't update
	if (m_pFbx->getSkeletonCount() == 0)
	{
		return;
	}
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

GLvoid FBXModel::Draw(const Camera& m_pCamState)
{
	//RenderFBX(m_pCamState);m_prog
	FBXAnimationDraw(m_pCamState);
}

GLvoid FBXModel::Destroy()
{

}

GLvoid FBXModel::RenderUI()
{

}

// Loads Textures already placed on a FBX //TODO: correct comment?
GLvoid FBXModel::LoadFBXTextures(FBXFile* fbx)
{
	//Loops through our FBO Textures and
	//add them to our sampler vector.
	for (GLuint i = 0; i < fbx->getTextureCount(); ++i)
	{
		m_pRenderable->samplers.push_back(TextureManager::GetSingleton().LoadTexture(fbx->getTextureByIndex(i)->path.c_str()));
	}
}

///-----------------------------------------------------------------------------------------------------------
/// <summary> 
/// <para>Loop through the meshes in the scene and add 3 uint variables to their m_userData.</para>
/// <para>Initialises these variables to contain a VAO, VBO, IBO, based off the FBXMeshdata.</para>
/// <para><param name="fbx" type ="FBXFile*"> P1: A fbx file.</param></para></summary>
///-----------------------------------------------------------------------------------------------------------
GLvoid FBXModel::CreateOpenGLBuffers(FBXFile* a_fbx)
{
	// create the GL VAO/VBO/IBO data for each mesh
	for (GLuint i = 0; i < a_fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = a_fbx->getMeshByIndex(i);
		//FBXTexture* texture = fbx->getTextureByName("./data/soulspear/soulspear.obj");// , &imageWidth, &imageHeight, &imageFormat, STBI_default);

		// storage for the opengl data in 3 unsigned int
		GLuint* glData = new GLuint[3];

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
			mesh->m_indices.size() * sizeof(GLuint),
			mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normals

		glEnableVertexAttribArray(2); //tangents
		glEnableVertexAttribArray(3); //textcoords Texture on FBX model via coordinates
		glEnableVertexAttribArray(4); //weights
		glEnableVertexAttribArray(5); //indices

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)FBXVertex::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),  (GLvoid*)FBXVertex::NormalOffset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),  (GLvoid*)FBXVertex::TangentOffset);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)FBXVertex::WeightsOffset);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)FBXVertex::IndicesOffset);


		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;
	}
}

///-----------------------------------------------------------------------------------------------------------
/// <summary>Loop through and destroy all the OpenGL buffers for the FBXMeshNodes.</summary>
///-----------------------------------------------------------------------------------------------------------
GLvoid FBXModel::CleanupOpenGLBuffers(FBXFile* fbx)
{
	// clean up the vertex data attached to each mesh
	for (GLuint i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		GLuint* glData = (GLuint*)mesh->m_userData;

		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);

		delete[] glData;
	}
}

GLvoid FBXModel::FBXLoader()
{
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const GLchar* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Normal; \
							layout(location=2) in vec2 TexCoord; \
							out vec4 vNormal; \
							out vec2 vTexCoord; \
							uniform mat4 ProjectionView; \
							uniform mat4 WorldTransform; \
							void main() { vNormal = Normal; vTexCoord = TexCoord; \
							gl_Position = ProjectionView * WorldTransform * Position; }";

	const GLchar* fsSource = "#version 410\n \
							in vec4 vNormal; \
							in vec2 vTexCoord; \
							out vec4 FragColor; \
							uniform sampler2D diffuse; \
							void main() { FragColor = texture(diffuse, vTexCoord); }";

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

///-----------------------------------------------------------------------------------------------------------
/// <summary> 
/// <para>Bind the shader and send across the virtual camera's projection and view matrices combined,</para>
/// <para>then we loop through the meshes and render them.</para>
/// <para><param name="cam" type ="Camera&"> P1: A virtual camera.</param></para></summary>
///-----------------------------------------------------------------------------------------------------------
GLvoid FBXModel::RenderFBX(const Camera& a_pCamState)
{
	glUseProgram(m_program_ID);

	// bind the camera
	GLuint loc = glGetUniformLocation(m_program_ID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &a_pCamState.getProjectionView()[0][0]); //m_projectionViewMatrix

	//GLuint id = m_pRenderable->GetTextureByName("soulspear_d").GetId(); //TODO: soulspear
	GLuint id = m_pRenderable->GetTextureByPath("./data/models/soulspear/soulspear_diffuse.tga")->GetId(); //TODO: soulspear
	
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
	for (GLuint i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		GLuint* glData = (GLuint*)mesh->m_userData;

		glBindVertexArray(glData[0]); //TODO: replace m_VAO with VAO
		//glBindVertexArray(Geom->GetVAO()); //TODO: replace m_VAO with VAO
		//unsigned int indexCount = (a_iRows - 1) * (a_iCols - 1) * 6; //TODO: m_iIndexCount = this formula
		glDrawElements(GL_TRIANGLES, (GLuint)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
}

/// -----------------------------
// Skeleton/ bones vertex shader
/// -----------------------------
GLvoid FBXModel::FBXSkeletonLoader()
{
	/// ----------------------------------------------------------
	/// Create shaders 
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const GLchar* vsSource = "#version 410\n \
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

	const GLchar* fsSource = "#version 410\n \
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
	GLint success = GL_FALSE;
	GLuint iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const GLchar**)&vsSource, 0);
	glCompileShader(iVertexShader);

	GLuint iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(iFragmentShader, 1, (const GLchar**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_program_FBXAnimation_ID = glCreateProgram();
	glAttachShader(m_program_FBXAnimation_ID, iVertexShader);
	glAttachShader(m_program_FBXAnimation_ID, iFragmentShader);
	glLinkProgram(m_program_FBXAnimation_ID);

	glGetProgramiv(m_program_FBXAnimation_ID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetProgramiv(m_program_FBXAnimation_ID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength];

		glGetProgramInfoLog(m_program_FBXAnimation_ID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(iVertexShader);
	glDeleteShader(iFragmentShader);
}

// Render
GLvoid FBXModel::FBXSkeletonRender()
{
	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //normals
	glEnableVertexAttribArray(2); //tangents
	glEnableVertexAttribArray(3); //textcoords
	glEnableVertexAttribArray(4); //weights
	glEnableVertexAttribArray(5); //indices

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)FBXVertex::PositionOffset);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),  (GLvoid*)FBXVertex::NormalOffset);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),  (GLvoid*)FBXVertex::TangentOffset);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)FBXVertex::TexCoord1Offset);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)FBXVertex::WeightsOffset);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (GLvoid*)FBXVertex::IndicesOffset);
}

GLvoid FBXModel::FBXAnimationDraw(const Camera& a_pCamState)
{
	glUseProgram(m_program_FBXAnimation_ID);

	GLuint loc = glGetUniformLocation(m_program_FBXAnimation_ID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(a_pCamState.getProjectionView()));

	/*int light_dir_uniform = glGetUniformLocation(m_programID, "LightDir");
	glUniform3f(light_dir_uniform, 0, 1, 0);*/

	vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_program_FBXAnimation_ID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	GLuint light_colour_uniform = glGetUniformLocation(m_program_FBXAnimation_ID, "LightColour");
	glUniform3f(light_colour_uniform, 1, 1, 1);

	mat4 camera_matrix = a_pCamState.getTransform();
	GLint camera_pos_uniform = glGetUniformLocation(m_program_FBXAnimation_ID, "CameraPos");
	glUniform3f(camera_pos_uniform, camera_matrix[3][0], camera_matrix[3][1], camera_matrix[3][2]);

	GLuint specular_uniform = glGetUniformLocation(m_program_FBXAnimation_ID, "SpecPow");
	glUniform1f(specular_uniform, 12);

	// Scale the FBX
	glm::mat4 m4WorldTransform(1);
	GLfloat fScale = 0.003f;
	m4WorldTransform = glm::scale(glm::vec3(fScale));
	GLuint worldTransform = glGetUniformLocation(m_program_FBXAnimation_ID, "WorldTransform");
	glUniformMatrix4fv(worldTransform, 1, GL_FALSE, glm::value_ptr(m4WorldTransform));

	// -----------------------------------------------------
	// Renders job
	// If we have any textures...
	if (m_pRenderable->samplers.size() != 0)
	{
		// Diffuse Texture in slot 0
		glActiveTexture(GL_TEXTURE0); // TODO: delete me: glBindTexture(GL_TEXTURE_2D, m_pRenderable->GetTextureByName("Pyro_D").GetId()); // m_texture);
		// Hacky condition for the Pyro/ characters
		// if we're not loading a character...
		if (m_pRenderable->samplers.size() <= 3)
		{
			//... take the first texture from our samplers
			glBindTexture(GL_TEXTURE_2D, m_pRenderable->samplers.begin()->tTexture->GetId()); // m_texture); 
		}
		else
		{
			//Pyro_D "./data/models/characters/Pyro/Pyro_D.tga"
			glBindTexture(GL_TEXTURE_2D, m_pRenderable->samplers[3].tTexture->GetId()); // m_texture); 
		}
		loc = glGetUniformLocation(m_program_FBXAnimation_ID, "diffuse");
		glUniform1i(loc, 0);

		// If we have a normal texture...
		if (m_pRenderable->samplers.size() >= 1)
		{
			// Normal Texture in slot 1
			glActiveTexture(GL_TEXTURE1); //TODO: delete me: glBindTexture(GL_TEXTURE_2D, m_pRenderable->GetTextureByName("Pyro_N").GetId()); // m_normal);
			//glBindTexture(GL_TEXTURE_2D, m_pRenderable->GetTextureByPath("./data/models/characters/Pyro/Pyro_N.tga")->GetId()); // m_normal);
			// Hacky condition for the Pyro/ characters
			if (m_pRenderable->samplers.size() <= 3)
			{
				glBindTexture(GL_TEXTURE_2D, m_pRenderable->samplers[2].tTexture->GetId()); // m_texture); 
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, m_pRenderable->samplers[4].tTexture->GetId()); // m_texture); 
			}
			loc = glGetUniformLocation(m_program_FBXAnimation_ID, "normal");
			glUniform1i(loc, 1);
		}
	}

	// -----------------------------------------------------
	// FBX Mesh Draw
	if (m_pFbx->getSkeletonCount() != 0)
	{
		FBXSkeleton* skeleton = m_pFbx->getSkeletonByIndex(0);
		skeleton->updateBones();

		GLint bones_location = glGetUniformLocation(m_program_FBXAnimation_ID, "bones");
		glUniformMatrix4fv(bones_location, skeleton->m_boneCount, GL_FALSE, (GLfloat*)skeleton->m_bones);
	}
	// bind our vertex array object and draw the mesh
	for (GLuint i = 0; i < m_pFbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_pFbx->getMeshByIndex(i);
		GLuint* glData = (GLuint*)mesh->m_userData;
		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES,
			(GLuint)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	// -----------------------------------------------------
}