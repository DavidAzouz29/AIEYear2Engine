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
/// ***EDIT***
/// - Texture class created	 	- David Azouz 8/06/16
/// - Singleton  	- David Azouz 8/06/16
/// -  	- David Azouz /06/16
/// 
/// TODO: texture class that FBX class has a copy of
/// 
/// </summary>
/// ----------------------------------------------------------

#pragma once
#include "gl_core_4_4.h"

#include <string>
#include <map>
#include <memory>

class Texture;

class TextureManager
{
public:
	static TextureManager *GetSingleton() { return m_pSingleton; }
	static void CreateSingleton() { m_pSingleton = new TextureManager(); }
	static void DestroySingleton() { delete m_pSingleton; }

	std::shared_ptr<Texture> LoadTexture(char* szFileName);

	bool Create();

	//GLvoid Destroy();

	std::shared_ptr<Texture> AddTexture(const GLchar* name);
	std::shared_ptr<Texture> GetTextureByName(const GLchar* a_name);
	bool DoesTextureNameExist(const GLchar* a_name);

	GLuint TextureInit(const GLchar* a_path);
	GLvoid GenTexture(const GLchar* a_path, GLuint a_TextureType);

private:
	TextureManager();
	~TextureManager();
	static TextureManager* m_pSingleton;

	// A way to track 
	std::map<std::string, std::weak_ptr<Texture>> m_textures;
	//std::map<std::string, const GLuint> m_textures;
	//std::map<std::string, std::weak_ptr<Texture>>* m_pTextureList;
};

