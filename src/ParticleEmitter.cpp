#include "gl_core_4_4.h"
#include "ParticleEmitter.h"
#include "Particle.h"
//TODO: ResourceCreator.h

#include <assert.h>

ParticleEmitter::ParticleEmitter() :
	m_pPartiles(nullptr),
	m_pVertices(nullptr),
	m_uiFirstDeadIndex(-1),
	m_v3particlePosition(0)
{

}

ParticleEmitter::~ParticleEmitter()
{
}

int ParticleEmitter::create(ParticleEmitterConfig config)
{
	assert(!isValid());

	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const char* vsSource = "#version 410\n \
							in vec4 Position; \
							in vec4 Color; \
							out vec4 vColor; \
							uniform mat4 ProjectionView; \
							void main() { \
							vColor = Color; \
							gl_Position = ProjectionView * Position; \
							}";

	// RGB x 2 - 1 to move it from RGB to XYZ, or 
	// XYZ x 0.5 + 0.5 to move it from XYZ to RGB.
	const char* fsSource = "#version 410\n \
							in vec4 vColor; \
							void main() { \
							gl_FragColor = vColor; }";


	//TODO:
	//m_program = ResourceCreator::CreateProgram("./data/shaders/color.vert", "./data/shaders/vertexColor.frag");
	if (!m_program.create(vsSource, fsSource)) return false;

	m_config = config;
	m_emitTimer = 0;

	m_pPartiles = new Particle[m_config.particleCount];
	m_uiFirstDeadIndex = 0;

	unsigned int vertCount = m_config.particleCount * 8; //4
	m_pVertices = new Vertex_PositionColor[vertCount];

	unsigned int indexCount = m_config.particleCount * 6;
	unsigned int* pIndexData = new unsigned int[indexCount];

	m_v3particlePosition = m_config.v3ParticlePosition;

	for (unsigned int i = 0; i < m_config.particleCount; i++)
	{
		unsigned int indexStart = i * 6;
		unsigned int vertStart = i * 4;
		// -----------------------
		// Two triangles
		// -----------------------
		pIndexData[indexStart + 0] = vertStart + 0;
		pIndexData[indexStart + 1] = vertStart + 1;
		pIndexData[indexStart + 2] = vertStart + 2;

		pIndexData[indexStart + 3] = vertStart + 0;
		pIndexData[indexStart + 4] = vertStart + 2;
		pIndexData[indexStart + 5] = vertStart + 3;

	}

	// Creates storage (OpenGL)
	m_mesh.create(m_pVertices, vertCount, pIndexData, indexCount);

	delete pIndexData;

	return true;
}
void ParticleEmitter::destroy()
{
	assert(isValid());

	delete[] m_pPartiles; 
	delete[] m_pVertices; 

	m_mesh.destroy();
}

void ParticleEmitter::emit()
{
	assert(isValid());

	// Check that there is room for more particles.
	if (m_uiFirstDeadIndex >= m_config.particleCount) return;

	Particle& particle = m_pPartiles[m_uiFirstDeadIndex++];

	particle.position = m_v3particlePosition;

	particle.lifeTime = 0;
	particle.lifeSpan = (rand() / (float)RAND_MAX) * 
		(m_config.lifespanMax - m_config.lifespanMin) 
		+ m_config.lifespanMin; 

	particle.color = m_config.startColor;
	particle.size = m_config.startSize;

	/// <summary> 
	/// Rand velocity between 0 and 1,
	/// </summary>
	float fSpeed = (rand() / (float)RAND_MAX *
		(m_config.velocityMax - m_config.velocityMin) 
		+ m_config.velocityMin);

	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;

	// TODO: Needed?
	particle.velocity = glm::normalize(particle.velocity) * fSpeed;

}

void ParticleEmitter::update(float a_deltaTime, const glm::mat4 a_m4camMatrix)
{
	m_emitTimer += a_deltaTime;

	float invEmitRate = 1.f / m_config.emitRate;
	while (m_emitTimer > invEmitRate)
	{
		emit();
		m_emitTimer -= invEmitRate;
	}

	int quadIndex = 0;
	for (GLuint i = 0; i < m_uiFirstDeadIndex; i++)
	{
		Particle* particle = &m_pPartiles[i];

		particle->lifeTime += a_deltaTime;

		if (particle->lifeTime > particle->lifeSpan)
		{
			*particle = m_pPartiles[m_uiFirstDeadIndex - 1];
			m_uiFirstDeadIndex--;
			continue;
		}

		particle->position += particle->velocity * a_deltaTime;
		
		float timeNorm = particle->lifeTime / particle->lifeSpan;
		particle->size = glm::mix(m_config.startSize, m_config.endSize, timeNorm);
		particle->color = glm::mix(m_config.startColor, m_config.endColor, timeNorm);

		float halfSize = particle->size * 0.5f;

		unsigned int vertexIndex = quadIndex * 4;
		m_pVertices[vertexIndex + 0].position = glm::vec4( halfSize,  halfSize, 0, 1);
		m_pVertices[vertexIndex + 1].position = glm::vec4(-halfSize,  halfSize, 0, 1);
		m_pVertices[vertexIndex + 2].position = glm::vec4(-halfSize, -halfSize, 0, 1);
		m_pVertices[vertexIndex + 3].position = glm::vec4( halfSize, -halfSize, 0, 1);
		
		m_pVertices[vertexIndex + 0].color = particle->color;
		m_pVertices[vertexIndex + 1].color = particle->color;
		m_pVertices[vertexIndex + 2].color = particle->color;
		m_pVertices[vertexIndex + 3].color = particle->color;

		// create billboard
		glm::vec3 zAxis = glm::normalize(glm::vec3(a_m4camMatrix[3]) - particle->position);
		glm::vec3 xAxis = glm::cross(glm::vec3(a_m4camMatrix[1]), zAxis);
		glm::vec3 yAxis = glm::cross(zAxis, xAxis);

		glm::mat4 billboard(
			glm::vec4(xAxis, 0),
			glm::vec4(yAxis, 0),
			glm::vec4(zAxis, 0),
			glm::vec4(0, 0, 0, 1)
			);

		billboardParticle(vertexIndex + 0, billboard, particle);
		billboardParticle(vertexIndex + 1, billboard, particle);
		billboardParticle(vertexIndex + 2, billboard, particle);
		billboardParticle(vertexIndex + 3, billboard, particle);

		quadIndex++;
	}

}

void ParticleEmitter::billboardParticle(unsigned int vertexIndex, const glm::mat4& billboardMat, const Particle* particle)
{
	m_pVertices[vertexIndex].position =	billboardMat *
		m_pVertices[vertexIndex].position +
		glm::vec4(particle->position, 0);
}

void ParticleEmitter::draw(const glm::mat4& projView)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.GetVBO());
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_uiFirstDeadIndex * 4 * sizeof(Vertex_PositionColor), m_pVertices);

	glUseProgram(m_program.getId());
	m_program.setUniform("ProjectionView", projView);
	glBindVertexArray(m_mesh.GetVAO());
	glDrawElements(GL_TRIANGLES, m_uiFirstDeadIndex * 6, GL_UNSIGNED_INT, 0);
}