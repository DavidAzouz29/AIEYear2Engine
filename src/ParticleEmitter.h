#pragma once
#include "Entity\Entity.h"
#include "Render.h"
#include "Program.h"
#include "Mesh.h"
#include <glm/vec4.hpp>

struct Particle;

struct ParticleEmitterConfig 
{
	ParticleEmitterConfig() : particleCount(-1) {}
	
	GLuint particleCount; // How many I have
	
	GLfloat emitRate;

	GLfloat lifespanMin;
	GLfloat lifespanMax;

	GLfloat velocityMin;
	GLfloat velocityMax;

	GLfloat startSize;
	GLfloat endSize;

	glm::vec4 startColor;
	glm::vec4 endColor;

	glm::vec3 v3ParticlePosition;
};

/**

*/

class ParticleEmitter : public Entity
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	bool isValid() { return m_config.particleCount != -1; }
	bool Create();
	bool ParticleLoader(ParticleEmitterConfig config);
	GLvoid Update(GLfloat a_deltaTime, const glm::mat4 a_m4camMatrix);
	GLvoid Draw(const glm::mat4& projView);
	GLvoid Destroy();

	/// ----------------------------------------------------------
	/// Safety for dangling pointer
	/// ----------------------------------------------------------
	ParticleEmitter(const ParticleEmitter& rhs) = delete;
	GLvoid operator =(const ParticleEmitter& rhs) = delete;

private:
	GLvoid Emit();
	GLvoid BillboardParticle(unsigned int vertexIndex, const glm::mat4& billboardMat, const Particle* particle);
	
	glm::vec3 m_v3particlePosition;

	Particle* m_pPartiles; //could be a smart pointer
	Vertex_PositionColor* m_pVertices; // GPU sides, verts that will render

	GLuint m_uiFirstDeadIndex; // First dead thing
	//GLuint m_uiMaxParticles; // Maximum amount of particles we have

	ParticleEmitterConfig m_config;
	GLfloat m_fEmitTimer;

	//Render m_render; 
	//Geometry m_geometry; //
	Mesh m_mesh; // need the VAO, VBO, IBO.
	Program m_program;
};

