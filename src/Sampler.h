/// <summary>
/// File Name:		Sampler.h
/// Author: 		David Azouz
/// Date Created: 	24/05/16
/// Date Modified: 	24/05/16
///----------------------------------------------------------
/// Brief: A Sampler Class that modifies parameters of a texture.
/// Parameters include Wrapping and Filtering.
/// Furthermore we can have more than one filter for two or more instances of the same texture.
/// Things like GL_CLAMP_TO_EDGE happen here.
/// viewed: 
/// https://github.com/johnsietsma/RefEngine/blob/f46a001e7194b57ac7777af8b56a4809366025b4/Engine/src/graphics/Sampler.h
/// 
/// ***EDIT***
/// - Sampler class 	 		- David Azouz 24/05/16
/// - Texture adjustments made 	- David Azouz 9/06/16
/// 
/// TODO:
/// 
/// </summary>
/// ----------------------------------------------------------

#pragma once

#include "Texture.h"

#include "gl_core_4_4.h"

#include <memory>

struct Sampler
{
	Sampler(const std::shared_ptr<Texture>& a_texture);
	~Sampler();

	std::shared_ptr<Texture> tTexture; //

private:
	//GLuint m_samplerID;
};

