#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

/*
	Represents particles properties.
	This is kept on the CPU side and is used to control
	the particles send to the GPU.
*/

// Convention
struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;

	float size; // In world units
	float lifeTime; // In seconds
	float lifeSpan;
};