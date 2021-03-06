#pragma once
#include "Entity\Entity.h"
#include "Render.h"
#include "Mesh.h"
#include "Program.h"
#include <glm/vec4.hpp>

struct Particle;
//class Mesh;

struct ParticleEmitterConfig 
{
	ParticleEmitterConfig() : particleCount(USHRT_MAX) {}
	
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
	ParticleEmitter(ParticleEmitterConfig a_config);
	~ParticleEmitter() = default;

	bool isValid() { return m_config.particleCount == (GLushort)USHRT_MAX; } //TODO: breaking -1?
	bool Create() override;
	bool ParticleLoader(ParticleEmitterConfig a_config);
	GLvoid Update(GLfloat a_deltaTime, const glm::mat4 a_m4camMatrix); // override; TODO: 
	//GLvoid Draw(const glm::mat4& projView) override;
	//GLvoid Draw(const Camera* m_pCamState) override;
	GLvoid Draw(const Camera& m_pCamState) override;
	GLvoid Destroy() override;
	GLvoid RenderUI() override;

	/// ----------------------------------------------------------
	/// Safety for dangling pointer
	/// ----------------------------------------------------------
	ParticleEmitter(const ParticleEmitter& rhs) = delete;
	GLvoid operator =(const ParticleEmitter& rhs) = delete;

private:
	GLvoid Emit();
	GLvoid BillboardParticle(GLuint vertexIndex, const glm::mat4& billboardMat, const Particle* particle);
	
	glm::vec3 m_v3particlePosition;
	// TODO: glm::mat4 m_m4WorldTransform;

	Particle* m_pPartiles; //could be a smart pointer
	Vertex_PositionColor* m_pVertices; // GPU sides, verts that will render

	GLuint m_uiFirstDeadIndex; // First dead thing
	//GLuint m_uiMaxParticles; // Maximum amount of particles we have

	ParticleEmitterConfig m_config;
	GLfloat m_fEmitTimer;

	// Colours used primarily for particles and 
	// modification during runtime via ImGui.
	glm::vec4 m_v4StartColor;
	glm::vec4 m_v4EndColor;
	
	Program m_program;
};

