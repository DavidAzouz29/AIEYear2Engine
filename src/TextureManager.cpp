/// <summary>
/// Notes:
/// m_textures.insert_or_assign(filename, pNewTexture); replaces
/// m_textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(filename, pNewTexture));
/// 
/// 
/// </summary>

#include "TextureManager.h"
#include "Texture.h"

#include <assert.h>
#include <string>

/// --------------------------------------------------------------------
/// <summary>
/// Adds a Texture to a map.
/// the './data/' is within LoadTexture V_below_V.
/// <para><param>P1: name we specify.</param></para>
/// <para><param>P2: the ID we generated from our 'TextureInit' func.</param></para>
/// <example> id = TextureInit("models/soulspear/soulspear_diffuse.tga"); </example>
/// </summary> //"models/soulspear/soulspear_diffuse.tga"
/// --------------------------------------------------------------------
/*bool TextureManager::Create()
{
	GLuint id = 0;
	// TODO: split into path and filename
	id = TextureInit("models/soulspear/soulspear_diffuse.tga");
	AddTexture("soulspear_d", std::make_shared<Texture>(id));

	id = TextureInit("models/soulspear/soulspear_normal.tga");
	AddTexture("soulspear_n", std::make_shared<Texture>(id));

	id = TextureInit("models/characters/Pyro/Pyro_D.tga");
	AddTexture("Pyro_D", std::make_shared<Texture>(id));

	id = TextureInit("models/characters/Pyro/Pyro_N.tga");
	AddTexture("Pyro_N", std::make_shared<Texture>(id));

	id = TextureInit("models/characters/Pyro/Pyro_S.tga");
	AddTexture("Pyro_S", std::make_shared<Texture>(id));

	//id = m_pTexture->TextureInit("./data/models/characters/Pyro/Pyro_S.tga");
	//AddTexture("Pyro_S", id);

	return true;
} */

/// --------------------------------------------------------------------
/// <summary>
/// Generates a Texture map
/// Note: Textures must be in the 'data' file.
/// <para><param>P1: File Path.</param></para>
/// </summary>
/// --------------------------------------------------------------------
std::shared_ptr<Texture> TextureManager::LoadTexture(const GLchar* szFileName)
{
	//---------------------------------
	// if you want to add to the filename
	//---------------------------------
	//std::string filename = "./data/";
	//filename += szFileName;
	//---------------------------------

	//strcat_s(filename, 100, "./Images/");
	//strcat_s(filename, 100, szFileName);

	auto it = m_textures.find(szFileName); //filename

	// if not at the end
	if (it != m_textures.end())
	{
		// set lock and store in a variable
		// Pointing to a 
		std::shared_ptr<Texture> texture = it->second.lock();
		// check if the texture may have been deleted - create again.
		if (texture == nullptr)
		{
			// point to a new instance
			texture = std::make_shared<Texture>(szFileName); //filename.c_str()
			it->second = texture;
		}
		// return what's valid
		return texture;
	}

	// if we have reached the end/ hasn't been loaded before
	std::shared_ptr<Texture> pNewTexture = std::make_shared<Texture>(szFileName); //filename.c_str()
	m_textures.emplace(szFileName, pNewTexture);
	return pNewTexture;
}

// Returns a flag (nullptr) value to determine whether a Texture has been loaded.
std::shared_ptr<Texture> TextureManager::GetTextureByName(const GLchar* a_name)
{
	// First find our texture in our map.
	// Use the result to check and add...
	auto texture = m_textures.find(a_name);

	// ...if the texture exists...
	if (texture != m_textures.end())
	{
		// return the texture...
		// if the texture is discarded: return a nullptr
		return texture->second.lock();
	}
	else
	{
		return nullptr; 
	}
}

/// --------------------------------------------------------------------
/// <summary> //TODO: needs to be added to
/// Checks if a texture exists in our (map).
/// if not: returns the end of the (map).
/// </summary>
/// --------------------------------------------------------------------
bool TextureManager::DoesTextureNameExist(const GLchar* a_name)
{
	return m_textures.find(a_name) != std::end(m_textures);
}