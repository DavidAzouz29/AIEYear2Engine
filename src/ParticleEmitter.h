#pragma once
#include "Render.h"
#include "Program.h"
#include "Mesh.h"
#include <glm/vec4.hpp>

struct Particle;

struct ParticleEmitterConfig 
{
	ParticleEmitterConfig() : particleCount(-1) {}
	
	GLuint particleCount; // How many I have
	
	//float emitTimer;
	float emitRate;

	float lifespanMin;
	float lifespanMax;

	float velocityMin;
	float velocityMax;

	float startSize;
	float endSize;

	glm::vec4 startColor;
	glm::vec4 endColor;

	glm::vec3 v3ParticlePosition;
	//glm::vec3 m_positionB;
};

/**

*/

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	bool isValid() { return m_config.particleCount != -1; }
	int create(ParticleEmitterConfig config);
	void destroy();

	void update(float a_deltaTime, const glm::mat4 a_m4camMatrix);
	void draw(const glm::mat4& projView);

	/// ----------------------------------------------------------
	/// Safety for dangling pointer
	/// ----------------------------------------------------------
	ParticleEmitter(const ParticleEmitter& rhs) = delete;
	void operator =(const ParticleEmitter& rhs) = delete;

private:
	void emit();
	void billboardParticle(unsigned int vertexIndex, const glm::mat4& billboardMat, const Particle* particle);
	
	glm::vec3 m_v3particlePosition;
	//glm::vec3 m_positionB;

	Particle* m_pPartiles; //could be a smart pointer
	Vertex_PositionColor* m_pVertices; // GPU sides, verts that will render

	GLuint m_uiFirstDeadIndex; // First dead thing
	//GLuint m_uiMaxParticles; // Maximum amount of particles we have

	ParticleEmitterConfig m_config;
	GLfloat m_emitTimer;

	//Render m_render; 
	//Geometry m_geometry; //
	Mesh m_mesh; // need the VAO, VBO, IBO.
	Program m_program;
};

