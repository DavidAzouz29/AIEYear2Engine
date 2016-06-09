#include "Sampler.h"

Sampler::Sampler(const std::shared_ptr<Texture>& a_texture) : 
	tTexture(a_texture) 
{
	glGenSamplers(1, &m_samplerID);
}

Sampler::~Sampler()
{
	glDeleteSamplers(1, &m_samplerID);
}

/*
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
*/