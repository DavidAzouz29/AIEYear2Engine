/// <summary>
/// File Name:		GPUParticleEmitter.cpp
/// Author: 		David Azouz
/// Date Created: 	11/03/16
/// Date Modified: 	11/03/16
///----------------------------------------------------------
/// Brief: A GPU Particle Emitter Class that emits particles on the GPU.
/// viewed: 
/// Robert Bridson - co-author of "Fluid Simulation for Computer Graphics"
/// https://github.com/johnsietsma/RefEngine/blob/master/Engine/src/Engine/Helpers.cpp
/// http://en.cppreference.com/w/cpp/types/extent
///
/// ***EDIT***
/// - Partcles on the GPU	 	- David Azouz 11/03/16
///
/// Notes:
/// in shader code '#version 410' = line 2, count onwards
/// 
/// TODO:
/// - Add gravity to the emitter that gets applied to the particle's movement during update.
/// - Allow for different emit types rather than just point.
/// - Add textures to particles.
/// - Move LoadShader to seperate file
/// </summary>
/// ----------------------------------------------------------

#include "GPUParticleEmitter.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <glm/ext.hpp>
#include <cstdio>

GPUParticleEmitter::GPUParticleEmitter()
	: m_particles(nullptr), m_uiMaxParticles(0),
	m_v3Position(0, 0, 0),
	m_drawShader(0),
	m_updateShader(0),
	m_fLastDrawTime(0)
{
	m_vao[0] = 0;
	m_vao[1] = 0;
	m_vbo[0] = 0;
	m_vbo[1] = 0;
}


GPUParticleEmitter::~GPUParticleEmitter()
{
	delete[] m_particles;

	glDeleteVertexArrays(2, m_vao);
	glDeleteBuffers(2, m_vbo);

	// delete the shaders
	glDeleteProgram(m_drawShader);
	glDeleteProgram(m_updateShader);
}

bool GPUParticleEmitter::Create()
{
	m_pGPUEmitter = std::make_shared<GPUParticleEmitter>();
	m_pGPUEmitter.get()->Initalise(MAX_PARTICLES, 3.0f, 5.0f, 5, 20, 1, 0.1f,
		m_v4StartColor, m_v4EndColor);
	return false;
}

/// ----------------------------------------------------------
/// <summary> Initialise GPU Particle System (won't appear)
/// <para><param>P1: Maximum amount of particles wanted to be used.</param></para>
/// <para><param>P2 + P3: Life span: how short to how long should the particles be alive.</param></para>
/// <para><param>P4 + P5: How fast should the particles move.</param></para>
/// <para><param>P6 + P7: How small or large should a particle be.</param></para>
/// <para><param>P8 + P9: What color a particle should be when it's born/ dies.</param></para>
/// </summary>
/// ----------------------------------------------------------
void GPUParticleEmitter::Initalise(GLuint a_uiMaxParticles,
	GLfloat a_fLifespanMin, GLfloat a_fLifespanMax,
	GLfloat a_fVelocityMin, GLfloat a_fVelocityMax,
	GLfloat a_fStartSize, GLfloat a_fEndSize,
	const glm::vec4& a_v4StartColor,
	const glm::vec4& a_v4EndColor)
{
	// store all variables passed in
	m_v4StartColor = a_v4StartColor;
	m_v4EndColor = a_v4EndColor;
	m_fStartSize = a_fStartSize;
	m_fEndSize = a_fEndSize;
	m_fVelocityMin = a_fVelocityMin;
	m_fVelocityMax = a_fVelocityMax;
	m_fLifespanMin = a_fLifespanMin;
	m_fLifespanMax = a_fLifespanMax;
	m_uiMaxParticles = a_uiMaxParticles;

	// create particle array
	m_particles = new GPUParticle[a_uiMaxParticles];

	// set our starting ping-pong buffer
	m_activeBuffer = 0;

	CreateBuffers();

	CreateUpdateShader();

	CreateDrawShader();
}

void GPUParticleEmitter::CreateBuffers()
{
	// create OpenGL buffers
	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);

	// setup the first buffer
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_uiMaxParticles * sizeof(GPUParticle), m_particles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((GLchar*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((GLchar*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((GLchar*)0) + 28);
	
	// setup the second buffer
	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_uiMaxParticles * sizeof(GPUParticle), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((GLchar*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((GLchar*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((GLchar*)0) + 28);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/// ----------------------------------------------------------
/// <summary>
/// Creates our shader that 'updates' our particles.
/// <para>Uses the Transform Feedback.</para>
/// <para>Flawlessly.</para>
/// </summary>
/// ----------------------------------------------------------
void GPUParticleEmitter::CreateUpdateShader()
{
	int success = GL_FALSE;
	// create a shader
	GLuint vs = LoadShader(GL_VERTEX_SHADER, "./data/shaders/gpuParticleUpdate.vert");

	m_updateShader = glCreateProgram();
	glAttachShader(m_updateShader, vs);

	// specify the data that we will stream back
	const GLchar* c_cVaryings[] = { "v3Position", "velocity",
									"vLifetime", "vLifespan" };
	GLuint uiSize = sizeof(c_cVaryings) / sizeof(const GLchar*);
	glTransformFeedbackVaryings(m_updateShader, uiSize, c_cVaryings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(m_updateShader);

	/// ----------------------------------------------------------
	/// Error checking
	/// ----------------------------------------------------------
	glGetProgramiv(m_updateShader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_updateShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_updateShader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	/// ----------------------------------------------------------
	// remove unneeded handles
	glDeleteShader(vs);

	// bind the shader so that we can set some
	// uniforms that don't change per-frame
	glUseProgram(m_updateShader);

	// bind lifetime minimum and maximum
	int location = glGetUniformLocation(m_updateShader, "lifeMin");
	glUniform1f(location, m_fLifespanMin);
	location = glGetUniformLocation(m_updateShader, "lifeMax");
	glUniform1f(location, m_fLifespanMax);
}

/// ----------------------------------------------------------
/// <summary>
/// Takes the buffer of particles and passes them 
/// <para>through the Vertex Shader to| the Geometry Shader.</para>
/// <para>Geometry Shader recieves points -> outputs two tris for every point.</para>
/// </summary>
/// ----------------------------------------------------------
void GPUParticleEmitter::CreateDrawShader()
{
	int success = GL_FALSE;
	
	GLuint vs = LoadShader(GL_VERTEX_SHADER,	"./data/shaders/gpuParticle.vert");
	GLuint gs = LoadShader(GL_GEOMETRY_SHADER,	"./data/shaders/gpuParticle.geom");
	GLuint fs = LoadShader(GL_FRAGMENT_SHADER,	"./data/shaders/gpuParticle.frag");

	m_drawShader = glCreateProgram();
	glAttachShader(m_drawShader, vs);
	glAttachShader(m_drawShader, fs);
	glAttachShader(m_drawShader, gs);
	glLinkProgram(m_drawShader);

	/// ----------------------------------------------------------
	/// Error checking
	/// ----------------------------------------------------------
	glGetProgramiv(m_drawShader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_drawShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_drawShader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	/// ----------------------------------------------------------

	// remove unneeded handles
	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);

	// bind the shader so that we can set
	// some uniforms that don't change per-frame
	glUseProgram(m_drawShader);

	// bind size information for interpolation that won't change
	GLint location = glGetUniformLocation(m_drawShader, "fSizeStart");
	glUniform1f(location, m_fStartSize);
	location = glGetUniformLocation(m_drawShader, "fSizeEnd");
	glUniform1f(location, m_fEndSize);

	// bind colour information for interpolation that won't change
	location = glGetUniformLocation(m_drawShader, "v4ColorStart");
	glUniform4fv(location, 1, &m_v4StartColor[0]);
	location = glGetUniformLocation(m_drawShader, "v4ColorEnd");
	glUniform4fv(location, 1, &m_v4EndColor[0]);

}

/// ----------------------------------------------------------
/// <summary> Draw
/// <para>Ping-Pongs between buffers to draw.</para>
/// <param><para>P1: Time.</para></param>
/// <param><para>P2: Camera Transform used for location.</para></param>
/// <param><para>P3: Proj View.</para></param>
/// </summary>
/// ----------------------------------------------------------
void GPUParticleEmitter::Draw(GLfloat a_ftime, 
	const glm::mat4& a_m4CameraTransform,
	const glm::mat4& a_m4ProectionView)
{
	// update the particles using transform feedback
	glUseProgram(m_updateShader);

	// bind time information
	int location = glGetUniformLocation(m_updateShader, "time");
	glUniform1f(location, a_ftime);

	GLfloat deltaTime = a_ftime - m_fLastDrawTime;
	m_fLastDrawTime = a_ftime;

	location = glGetUniformLocation(m_updateShader, "deltaTime");
	glUniform1f(location, deltaTime);

	// bind emitter's position
	location = glGetUniformLocation(m_updateShader, "emitterPosition");
	glUniform3fv(location, 1, &m_v3Position[0]);

	// disable rasterisation
	// this is done by enableing the OpenGL state "GL_RASTERIZER_DISCARD"
	glEnable(GL_RASTERIZER_DISCARD);

	// bind the buffer we will update
	glBindVertexArray(m_vao[m_activeBuffer]);

	// work out the "other" buffer
	GLuint otherBuffer = (m_activeBuffer + 1) % 2;

	// bind the buffer we will update into as points
	// and begin transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_uiMaxParticles);

	// ^^ "other" buffer should now contain our updated particles.

	// disable transform feedback and enable rasterization again
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	// draw the particles using the Geometry Shader to billboard them
	glUseProgram(m_drawShader);

	location = glGetUniformLocation(m_drawShader, "m4ProjectionView");
	glUniformMatrix4fv(location, 1, false, &a_m4ProectionView[0][0]);
	location = glGetUniformLocation(m_drawShader, "m4CameraTransform");
	glUniformMatrix4fv(location, 1, false, &a_m4CameraTransform[0][0]);

	// bind colour information for interpolation that won't change
	location = glGetUniformLocation(m_drawShader, "v4ColorStart");
	glUniform4fv(location, 1, &m_v4StartColor[0]);
	location = glGetUniformLocation(m_drawShader, "v4ColorEnd");
	glUniform4fv(location, 1, &m_v4EndColor[0]);

	// draw particles in the "other" buffer
	glBindVertexArray(m_vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_uiMaxParticles);

	// swap for next frame
	m_activeBuffer = otherBuffer;
}

/// -----------------------------
// Renders UI elements via ImGui
/// -----------------------------
void GPUParticleEmitter::RenderUI()
{
	if (ImGui::CollapsingHeader("GPU Particles"))
	{
		// Locations in Grid format
		if (ImGui::TreeNode("Patricle Color"))
		{
			ImGui::ColorEdit4("Particle Start Color", glm::value_ptr(m_v4StartColor));
			ImGui::ColorEdit4("Particle End Color", glm::value_ptr(m_v4EndColor));

			ImGui::TreePop();
		}
	}
}

/// ----------------------------------------------------------
/// <summary> Load Shader 
/// TODO: move to seperate header/ cpp to load any "file": 
/// <para>Vertex, Fragment, Geometry shader files.</para>
/// <param><para>P1: Used to create a shader of 'type'.</para></param>
/// <param><para>P2: File path.</para></param>
/// </summary>
/// ----------------------------------------------------------
GLuint GPUParticleEmitter::LoadShader(GLuint a_iType, const GLchar* ac_cPath)
{
	// "rb" - read binary
	FILE* file = fopen(ac_cPath, "rb"); //TODO: fopen_s? - not reading files
	if (file == nullptr)
	{
		return 0;
	}

	// read the shader source
	fseek(file, 0, SEEK_END);
	GLuint uiLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	GLchar* cSource = new GLchar[uiLength + 1];
	memset(cSource, 0, uiLength + 1);
	fread(cSource, sizeof(GLchar), uiLength, file);
	fclose(file);

	GLuint uiShader = glCreateShader(a_iType);
	glShaderSource(uiShader, 1, &cSource, 0);
	glCompileShader(uiShader);
	delete[] cSource;

	return uiShader;
}