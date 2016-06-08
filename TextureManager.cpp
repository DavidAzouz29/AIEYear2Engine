#include "TextureManager.h"
#include "Texture.h"

#include <assert.h>
#include <stb_image.h>
#include <string>

TextureManager* TextureManager::m_pSingleton = nullptr;

TextureManager::TextureManager()
{
	m_pTextureList = new std::map<std::string, std::weak_ptr<Texture>>();
}

TextureManager::~TextureManager()
{
	auto it = m_pTextureList->begin();

	while (it != m_pTextureList->end())
	{
		delete it->second;
		it++;
	}

	m_pTextureList->clear();
	delete m_pTextureList;
}

/// --------------------------------------------------------------------
/// <summary>
/// Generates a Texture map
/// Note: Textures must be in the 'data' file.
/// <para><param>P1: File Path.</param></para>
/// </summary>
/// --------------------------------------------------------------------
std::weak_ptr<Texture*> TextureManager::LoadTexture(GLchar* szFileName)
{
	std::string filename = "./data/";
	filename += szFileName;

	//strcat_s(filename, 100, "./Images/");
	//strcat_s(filename, 100, szFileName);

	auto it = m_pTextureList->find(filename);

	if (it == m_pTextureList->end())
	{
		std::weak_ptr<Texture*> pNewTexture = new Texture(filename.c_str());
		m_pTextureList->insert(std::pair<std::string, std::weak_ptr<Texture*>>(filename, pNewTexture));
		return pNewTexture;
	}
	else
	{
		return it->second;
	}
}

bool TextureManager::Create()
{
	GLuint id = 0;
	id = TextureInit("models/soulspear/soulspear_diffuse.tga");
	AddTexture("soulspear_d", id);

	id = TextureInit("models/soulspear/soulspear_normal.tga");
	AddTexture("soulspear_n", id);

	id = TextureInit("models/characters/Pyro/Pyro_D.tga");
	AddTexture("Pyro_D", id);

	id = TextureInit("models/characters/Pyro/Pyro_N.tga");
	AddTexture("Pyro_N", id);

	id = TextureInit("models/characters/Pyro/Pyro_S.tga");
	AddTexture("Pyro_S", id);

	//id = m_pTexture->TextureInit("./data/models/characters/Pyro/Pyro_S.tga");

	return true;
}

//TODO: FIX
GLvoid TextureManager::Destroy()
{
	if (m_textureID != (GLuint)-1)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = (GLuint)-1;
	}
}

/// ----------------------------------------------------------
/// Texture
/// ----------------------------------------------------------
/// Texture Initialize
/// Param:
///			a_path: path of a texture
///	Usage: id = m_pTexture->TextureInit("./data/models/soulspear/soulspear_normal.tga");
/// View AddTexture
/// ----------------------------------------------------------
GLuint TextureManager::TextureInit(const GLchar* a_path)
{
	//GLint imageWidth = 0, imageHeight = 0, imageFormat = 0;

	/// ----------------------------------------------------------
	// This call will read in the default texel data for the crate.png.
	// In the case the image stores RGB values at 515x512 resolution.
	/// ----------------------------------------------------------
	// Load diffuse map
	//GLubyte* data = stbi_load(name, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	/// ----------------------------------------------------------
	//assert(m_textureID == (GLuint)-1);

	// Load diffuse map
	GenTexture(a_path, m_textureID);
	//TODO: normal isn't being used for anything
	/*GLuint uiNormal;
	// load normal map
	GenTexture(a_path, uiNormal); */
	
	// TODO: V GenTexture - is this fine
	/*// Generate an OpenGL texture handle.
	glGenTextures(1, &m_textureID);
	// Bind the texture to the correct slot for the dimension, in this case 2-D.
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// Specify the data for the texture, including the format, resolution and variable type.
	// Out data is an unsigned char, therefor it should be GL_UNSIGNED_BYTE.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data); //TODO: delete texture.

	// load normal map
	data = stbi_load(name, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	/// ----------------------------------------------------------
	GLuint uiNormal;
	// Generate an OpenGL texture handle.
	glGenTextures(1, &uiNormal);
	// Bind the texture to the correct slot for the dimension, in this case 2-D.
	glBindTexture(GL_TEXTURE_2D, uiNormal);
	// Specify the data for the texture, including the format, resolution and variable type.
	// Out data is an unsigned char, therefor it should be GL_UNSIGNED_BYTE.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data); //TODO: delete texture. */

	return m_textureID;
}

/// --------------------------------------------------------------------
/// <summary>
/// Generates a Texture map
/// <para><param>P1: File Path.</param></para>
/// <para><param>P2: a_TextureType: is it a texture, normal map, specular etc?</param></para>
/// </summary>
/// --------------------------------------------------------------------
GLvoid TextureManager::GenTexture(const GLchar* a_path, GLuint a_TextureType)
{
	GLint imageWidth = 0, imageHeight = 0, imageFormat = 0;

	/// ----------------------------------------------------------
	// This call will read in the default texel data for the crate.png.
	// In the case the image stores RGB values at 515x512 resolution.
	/// ----------------------------------------------------------
	GLubyte* data = stbi_load(a_path, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	/// ----------------------------------------------------------
	assert(a_TextureType == (GLuint)-1);

	// Generate an OpenGL texture handle.
	glGenTextures(1, &a_TextureType);
	// Bind the texture to the correct slot for the dimension, in this case 2-D.
	glBindTexture(GL_TEXTURE_2D, a_TextureType);
	// Specify the data for the texture, including the format, resolution and variable type.
	// Out data is an unsigned char, therefor it should be GL_UNSIGNED_BYTE.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data); //TODO: delete texture.
}

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
	return m_textures.insert_or_assign(std::pair<const std::string, const GLuint>(name, id));
}

// TODO: Return a Texture (class)
std::shared_ptr<Texture> TextureManager::GetTextureByName(const GLchar* a_name)
{
	//TODO: ? std::vector<std::string>::iterator iter = m_textures.begin();

	std::map<std::string, const GLuint>::iterator texture = m_textures.find(a_name);

	// if the texture exists...
	if (texture != m_textures.end())
	{
		return texture->second;
	}
	else
	{
		return USHRT_MAX; // TODO: future proof?
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