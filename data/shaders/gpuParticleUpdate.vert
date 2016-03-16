#version 410

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Velocity;
layout(location=2) in float Lifetime;
layout(location=3) in float Lifespan;

out vec3 v3Position;
out vec3 velocity;
out float vLifetime;
out	float vLifespan;

uniform float time;
uniform float deltaTime;
uniform float lifeMin; 
uniform float lifeMax;
uniform vec3 emitterPosition;

const float INVERSE_MAX_UINT = 1.0f / 4294967295.0f;

float rand(uint seed, float range)
{
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float(range * i) * INVERSE_MAX_UINT;
}

void main()
{
	v3Position = Position + Velocity * deltaTime;
	velocity = Velocity;
	vLifetime = Lifetime + deltaTime;
	vLifespan = Lifespan;

	// emit a new particle as soon as it dies
	if(vLifetime > vLifespan)
	{
		uint seed = uint(time * 1000.0) + uint(gl_VertexID);
		velocity.x = rand(seed++, 2) - 1;
		velocity.y = rand(seed++, 2) - 1;
		velocity.z = rand(seed++, 2) - 1;
		velocity = normalize(velocity);
		v3Position = emitterPosition;
		vLifetime = 0;
		vLifespan = rand(seed++, lifeMax - lifeMin) + lifeMin;
	}
}