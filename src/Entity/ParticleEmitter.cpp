#include "ParticleEmitter.h"
#include "Particle.h"
#include "Renderable.h"
//TODO: ResourceCreator.h

//#include <gl_core_4_4.h>
#include <assert.h>
#include <memory>

ParticleEmitter::ParticleEmitter(ParticleEmitterConfig a_config) :
	m_pPartiles(nullptr),
	m_pVertices(nullptr),
	m_uiFirstDeadIndex(-1),
	m_v3particlePosition(0)
{
	m_config = a_config;
}

/*ParticleEmitter::~ParticleEmitter()
{
}*/

bool ParticleEmitter::Create()
{
	///----------------------------------------------------------
	//m_pParticleEmitterA = std::make_shared<ParticleEmitter>();
	//m_pParticleEmitterB = std::make_shared<ParticleEmitter>();
	///----------------------------------------------------------
	GLuint uiAmount = 20; //TODO: 3000
	//ParticleEmitterConfig configA;
	m_config.particleCount = uiAmount; //1000 
	m_config.emitRate = (float)m_config.particleCount / 2; //500
	m_config.startColor = glm::vec4(1.56f, 0, 1.25f, 0.8f); //glm::vec4(1, 0, 0, 1); <-RED
	m_config.endColor = glm::vec4(0, 0.07f, 0.3f, 1);  //vec4(0, 0, 1, 0.8f); //vec4(0, 0, 1, 0.8f);//vec4(1, 1, 0, 1);
	m_config.lifespanMin = 0.1f;
	m_config.lifespanMax = 5; //5
	m_config.startSize = 0.4f; // 1
	m_config.endSize = 0.1f;
	m_config.velocityMin = 0.1f;
	m_config.velocityMax = 1.0f;
	m_config.v3ParticlePosition = glm::vec3(-3, 5, 0);

	if (!ParticleLoader(m_config)) return true;

	/*ParticleEmitterConfig configB;
	configB.particleCount = uiAmount; //5000
	configB.emitRate = (float)configB.particleCount / 2; //500
	configB.startColor = glm::vec4(1, 0, 1, 0.8f); //glm::vec4(1, 0, 0, 1); <-RED
	configB.endColor = glm::vec4(0.3, 0, 0.07f, 1);  //vec4(0, 0, 1, 0.8f); //vec4(0, 0, 1, 0.8f);//vec4(1, 1, 0, 1);
	configB.lifespanMin = 0.9f;
	configB.lifespanMax = 3; //5
	configB.startSize = 0.4f; // 1
	configB.endSize = 0.2f;
	configB.velocityMin = 0.1f;
	configB.velocityMax = 2.0f;
	configB.v3ParticlePosition = glm::vec3(3, 5, 0);

	if (ParticleLoader(configB)) return -5; //*/
	return false;
}
bool ParticleEmitter::ParticleLoader(ParticleEmitterConfig a_config)
{
	assert(!isValid());

	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const GLchar* vsSource = "#version 410\n \
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
	const GLchar* fsSource = "#version 410\n \
							in vec4 vColor; \
							void main() { \
							gl_FragColor = vColor; }";


	//TODO:
	//m_program = ResourceCreator::CreateProgram("./data/shaders/color.vert", "./data/shaders/vertexColor.frag");
	if (!m_program.create(vsSource, fsSource)) return false;

	m_config = a_config;
	m_fEmitTimer = 0;

	m_pPartiles = new Particle[m_config.particleCount];
	m_uiFirstDeadIndex = 0;

	GLuint vertCount = m_config.particleCount * 8; //4
	m_pVertices = new Vertex_PositionColor[vertCount];

	GLuint indexCount = m_config.particleCount * 6;
	GLuint* pIndexData = new GLuint[indexCount];

	m_v3particlePosition = m_config.v3ParticlePosition;

	for (GLuint i = 0; i < m_config.particleCount; i++)
	{
		GLuint indexStart = i * 6;
		GLuint vertStart = i * 4;
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
	m_pRenderable->mesh.Create(m_pVertices, vertCount, pIndexData, indexCount);

	delete pIndexData;

	return true;
}
GLvoid ParticleEmitter::Destroy()
{
	assert(!isValid());

	delete[] m_pPartiles;
	delete[] m_pVertices;

	m_pRenderable->mesh.Destroy();
}

GLvoid ParticleEmitter::Emit()
{
	assert(isValid());

	// Check that there is room for more particles.
	if (m_uiFirstDeadIndex >= m_config.particleCount) return;

	Particle& particle = m_pPartiles[m_uiFirstDeadIndex++];

	particle.position = m_v3particlePosition;

	particle.lifeTime = 0;
	particle.lifeSpan = (rand() / (GLfloat)RAND_MAX) * 
		(m_config.lifespanMax - m_config.lifespanMin) 
		+ m_config.lifespanMin; 

	particle.color = m_config.startColor;
	particle.size = m_config.startSize;

	/// <summary> 
	/// Rand velocity between 0 and 1,
	/// </summary>
	GLfloat fSpeed = (rand() / (GLfloat)RAND_MAX *
		(m_config.velocityMax - m_config.velocityMin) 
		+ m_config.velocityMin);

	particle.velocity.x = (rand() / (GLfloat)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (GLfloat)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (GLfloat)RAND_MAX) * 2 - 1;

	// TODO: Needed?
	particle.velocity = glm::normalize(particle.velocity) * fSpeed;

}

GLvoid ParticleEmitter::Update(GLfloat a_deltaTime, const glm::mat4 a_m4camMatrix)
{
	m_fEmitTimer += a_deltaTime;

	GLfloat invEmitRate = 1.f / m_config.emitRate;
	while (m_fEmitTimer > invEmitRate)
	{
		Emit();
		m_fEmitTimer -= invEmitRate;
	}

	GLint quadIndex = 0;
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
		
		GLfloat timeNorm = particle->lifeTime / particle->lifeSpan;
		particle->size = glm::mix(m_config.startSize, m_config.endSize, timeNorm);
		particle->color = glm::mix(m_config.startColor, m_config.endColor, timeNorm);

		GLfloat halfSize = particle->size * 0.5f;

		GLuint vertexIndex = quadIndex * 4;
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

		BillboardParticle(vertexIndex + 0, billboard, particle);
		BillboardParticle(vertexIndex + 1, billboard, particle);
		BillboardParticle(vertexIndex + 2, billboard, particle);
		BillboardParticle(vertexIndex + 3, billboard, particle);

		quadIndex++;
	}

}

GLvoid ParticleEmitter::BillboardParticle(GLuint vertexIndex, const glm::mat4& billboardMat, const Particle* particle)
{
	m_pVertices[vertexIndex].position =	billboardMat *
		m_pVertices[vertexIndex].position +
		glm::vec4(particle->position, 0);
}

GLvoid ParticleEmitter::Draw(const Camera& m_pCamState)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_pRenderable->mesh.GetVBO());
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_uiFirstDeadIndex * 4 * sizeof(Vertex_PositionColor), m_pVertices);

	glUseProgram(m_program.getId());
	m_program.setUniform("ProjectionView", m_pCamState.getProjectionView());
	glBindVertexArray(m_pRenderable->mesh.GetVAO());
	glDrawElements(GL_TRIANGLES, m_uiFirstDeadIndex * 6, GL_UNSIGNED_INT, 0);
}