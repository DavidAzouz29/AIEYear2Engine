#include "TextureManager.h"
#include "Texture.h"

#include <assert.h>
#include <string>

TextureManager* TextureManager::m_pSingleton = nullptr;

TextureManager::TextureManager()
{
	m_textures = std::map<std::string, std::weak_ptr<Texture>>();
}

TextureManager::~TextureManager()
{
	auto it = m_textures.begin();

	while (it != m_textures.end())
	{
		m_textures.erase(it);
		//delete it->second;
		//it++;
	}

	m_textures.clear();
	//delete m_textures; //TODO: clean up
}

/// --------------------------------------------------------------------
/// <summary>
/// Generates a Texture map
/// Note: Textures must be in the 'data' file.
/// <para><param>P1: File Path.</param></para>
/// </summary>
/// --------------------------------------------------------------------
std::shared_ptr<Texture> TextureManager::LoadTexture(GLchar* szFileName)
{
	std::string filename = "./data/";
	filename += szFileName;

	//strcat_s(filename, 100, "./Images/");
	//strcat_s(filename, 100, szFileName);

	auto it = m_textures.find(filename);
	
	// if not at the end
	if (it != m_textures.end)
	{
		// set lock and store in a variable 
		std::shared_ptr<Texture> texture = it->second.lock();
		// check if the texture may have been deleted - create again.
		if (texture == nullptr)
		{
			//TODO: is this right?
			std::shared_ptr<Texture> pNewTexture = std::make_shared<Texture>(filename.c_str());
			m_textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(filename, pNewTexture));
			return pNewTexture;
		}
	}

	// TODO: remove? if we are at the end
	if (it == m_textures.end())
	{
		std::shared_ptr<Texture> pNewTexture = std::make_shared<Texture>(filename.c_str());
		m_textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(filename, pNewTexture));
		return pNewTexture;
	}
	else
	{
		return it->second.lock();
	}
}

bool TextureManager::Create()
{
	GLuint id = 0;
	// TODO: split into path and filename
	id = TextureInit("models/soulspear/soulspear_diffuse.tga");
	AddTexture("soulspear_d");

	id = TextureInit("models/soulspear/soulspear_normal.tga");
	AddTexture("soulspear_n");

	id = TextureInit("models/characters/Pyro/Pyro_D.tga");
	AddTexture("Pyro_D");

	id = TextureInit("models/characters/Pyro/Pyro_N.tga");
	AddTexture("Pyro_N");

	id = TextureInit("models/characters/Pyro/Pyro_S.tga");
	AddTexture("Pyro_S");

	//id = m_pTexture->TextureInit("./data/models/characters/Pyro/Pyro_S.tga");
	//AddTexture("Pyro_S", id);

	return true;
}

//TODO: FIX Destroy?
/*GLvoid TextureManager::Destroy()
{
	if (m_textureID != (GLuint)-1)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = (GLuint)-1;
	}
} */

/// --------------------------------------------------------------------
/// <summary>
/// Adds a Texture to a map.
/// <para><param>P1: name we specify.</param></para>
/// <para><param>P2: the ID we generated from our 'TextureInit' func.</param></para>
/// <example> m_pTexture->AddTexture("soulspear_n", id); </example>
/// </summary>
/// --------------------------------------------------------------------
std::shared_ptr<Texture> TextureManager::AddTexture(const GLchar* name)
{
	// pairs a name we specify with an id generated from the 'TextureInit' func.
	// inserts that into a map.
	// old method: return m_textures.insert_or_assign(std::pair<const std::string, const GLuint>(name, id));
	return m_textures.insert_or_assign(std::pair<const std::string, std::weak_ptr<Texture>>(name, m_textures.at());
}

// TODO: Return a Texture (class)
std::shared_ptr<Texture> TextureManager::GetTextureByName(const GLchar* a_name)
{
	//TODO: ? std::vector<std::string>::iterator iter = m_textures.begin();

	// TODO: ? std::map<std::string, const GLuint>::iterator texture = m_textures.find(a_name);
	// First find our texture in our map.
	// Use the result to check and add...
	std::map<std::string, std::weak_ptr<Texture>>::iterator texture = m_textures.find(a_name);

	// ...if the texture exists...
	if (texture != m_textures.end())
	{
		return texture->second.lock(); //TODO: do I want this on the texture 
	}
	else
	{
		return Texture m_textures USHRT_MAX; // TODO: future proof?
	}
}

/// <summary>
/// Checks if a texture exists in our (map).
/// if not: returns the end of the (map).
/// </summary>
bool TextureManager::DoesTextureNameExist(const GLchar* a_name)
{
	return m_textures.find(a_name) != std::end(m_textures);
}