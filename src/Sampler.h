/// <summary>
/// File Name:		Sampler.h
/// Author: 		David Azouz
/// Date Created: 	24/05/16
/// Date Modified: 	24/05/16
///----------------------------------------------------------
/// Brief: A Sampler Class that modifies parameters of a texture.
/// Furthermore we can have more than one texture with different parameters
/// Things like GL_CLAMP_TO_EDGE happen here.
/// viewed: 
/// https://github.com/johnsietsma/RefEngine/blob/f46a001e7194b57ac7777af8b56a4809366025b4/Engine/src/graphics/Sampler.h
/// 
/// ***EDIT***
/// - Sampler class 	 	- David Azouz 24/05/16
/// 
/// TODO:
/// 
/// </summary>
/// ----------------------------------------------------------

#pragma once

#include "Texture.h"

#include "gl_core_4_4.h"

struct Sampler
{
	Sampler(Texture a_texture, GLuint a_textureID) :
		tTexture(a_texture),
		textureID(a_textureID)
	{}

	Texture tTexture;
	GLuint textureID; //programID
};

