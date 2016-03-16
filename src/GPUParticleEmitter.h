#pragma once

#include "GPUParticle.h"
#include "gl_core_4_4.h"

#include <glm/mat4x4.hpp>

class GPUParticleEmitter
{
public:
	GPUParticleEmitter();
	virtual ~GPUParticleEmitter();

	/// ----------------------------------------------------------
	/// <summary> Initialise GPU Particle System
	/// <para><param>P1: Maximum amount of particles wanted to be used.</param></para>
	/// <para><param>P2 + P3: Life span: how short to how long should the particles be alive.</param></para>
	/// <para><param>P4 + P5: How fast should the particles move.</param></para>
	/// <para><param>P6 + P7: How small or large should a particle be.</param></para>
	/// <para><param>P8 + P9: What color a particle should be when it's born/ dies.</param></para>
	/// </summary>
	/// ----------------------------------------------------------
	void Initalise(GLuint a_uiMaxParticles,
		GLfloat a_fLifespanMin, GLfloat a_fLifespanMax,
		GLfloat a_fVelocityMin, GLfloat a_fVelocityMax,
		GLfloat a_fStartSize, GLfloat a_fEndSize,
		const glm::vec4& a_v4StartColor,
		const glm::vec4& a_v4EndColor);

	void Draw(GLfloat a_ftime, const glm::mat4& a_m4CameraTransform,
		const glm::mat4& a_m4ProectionView);

	void RenderUI();
	GLuint LoadShader(GLuint a_iType, const GLchar* ac_cPath);

protected:
	///--------------------------------------------------
	/// <summary> Create </summary>
	///--------------------------------------------------
	void CreateBuffers();
	void CreateUpdateShader();
	void CreateDrawShader();
	///--------------------------------------------------

	GPUParticle* m_particles;

	GLuint m_uiMaxParticles;

	glm::vec3 m_v3Position;

	GLfloat m_fLifespanMin;
	GLfloat m_fLifespanMax;

	GLfloat m_fVelocityMin;
	GLfloat m_fVelocityMax;

	GLfloat m_fStartSize;
	GLfloat m_fEndSize;

	glm::vec4 m_v4StartColor;
	glm::vec4 m_v4EndColor;

	GLuint m_activeBuffer;
	GLuint m_vao[2];
	GLuint m_vbo[2];

	GLuint m_drawShader;
	GLuint m_updateShader;

	GLfloat m_fLastDrawTime;
};

