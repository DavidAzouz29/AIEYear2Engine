/// ----------------------------------------------------------
/// <summary>
/// File Name:		TextureManager.h
/// Author: 		David Azouz
/// Date Created: 	8/06/16
/// Date Modified: 	8/06/16
///----------------------------------------------------------
/// Brief: A Texture Manager that handles textures.
/// viewed: http://en.cppreference.com/w/cpp/memory/weak_ptr
/// https://github.com/richard-stern/SpaceCowboy/search?utf8=%E2%9C%93&q=singleton
/// https://github.com/richard-stern/SpaceCowboy/blob/6dd6520b484d4a31a814bc203ef3ab67d420cd70/Project1/TextureManager.h
/// http://www.cplusplus.com/reference/map/map/erase/
/// http://docs.w3cub.com/cpp/container/map/emplace/
/// Singletons using smart pointers http://www.cplusplus.com/forum/general/37113/
/// Meyer Singleton
/// ***EDIT***
/// - Texture class created	 		- David Azouz 8/06/16
/// - Singleton  					- David Azouz 8/06/16
/// - texture map fixed/ clean up	- David Azouz 9/06/16
/// 
/// TODO: texture class that FBX class has a copy of
/// 
/// </summary>
/// ----------------------------------------------------------

#pragma once
#include "gl_core_4_4.h"

#include <string>
#include <unordered_map>
#include <memory>

class Texture;

class TextureManager
{
public:
	//-----------------------------------
	// Singleton: Meyer 
	//-----------------------------------
	// Made once. 
	// Thread safe
	//-----------------------------------
	static TextureManager& GetSingleton() { static TextureManager a_TextMan; return a_TextMan; }

	bool Create();
	std::shared_ptr<Texture> LoadTexture(const GLchar* szFileName);
	std::shared_ptr<Texture> LoadTexture(const GLchar* a_name, GLenum a_format, GLuint a_width, GLuint a_height, GLenum a_components, GLenum a_type, const GLvoid* a_pPixels);

	std::shared_ptr<Texture> GetTextureByName(const GLchar* a_name);
	bool DoesTextureNameExist(const GLchar* a_name);

private:
	// A way to hold all our textures that have been loaded.
	// Textures are able to be reused for more that one object
	std::unordered_map<std::string, std::weak_ptr<Texture>> m_textures;
};