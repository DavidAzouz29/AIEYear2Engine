#pragma once

#include "gl_core_4_4.h"

#include <glm/vec3.hpp>

struct GPUParticle
{
	//GPUParticle() : fLifetime(1), fLifespan(0) {}
	GPUParticle() : vLifetime(1), vLifespan(0) {}

	glm::vec3 v3Position;
	glm::vec3 v3Velocity;
	GLfloat vLifetime;
	GLfloat vLifespan;
};