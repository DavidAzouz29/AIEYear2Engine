/// ----------------------------------------------------------
/// <summary>
/// File Name:		Texture.cpp
/// Author: 		David Azouz
/// Date Created: 	13/05/16
/// Date Modified: 	13/05/16
///----------------------------------------------------------
/// Brief: A Texture Class that enables more than one object to share the same texture.
/// Notes:
/// RGB x 2 - 1 to move it from RGB to XYZ, or 
/// XYZ x 0.5 + 0.5 to move it from XYZ to RGB.
/// viewed: http://lodev.org/lodepng/
/// https://github.com/richard-stern/SpaceCowboy/blob/6dd6520b484d4a31a814bc203ef3ab67d420cd70/AIE_GL/source/Texture.cpp
///
/// ***EDIT***
/// - Texture class created	 	- David Azouz 13/05/16
/// -  	- David Azouz 5/02/16
/// -  	- David Azouz 6/02/16
/// 
/// TODO: texture class that FBX class has a copy of
/// 
/// </summary>
/// ----------------------------------------------------------S

#include "Texture.h"
#include "Camera\Camera.h"
#include "Helpers.h"
#include "Mesh.h"

//#include <glm\glm.hpp>
//#include <glm\ext.hpp>
#include "lodepng.h"
#include <GLFW\glfw3.h>
#include <stb_image.h>

#include <cstdio>

const Texture Texture::Invalid = Texture();

/*Texture::Texture()
{
	Create();
	//TODO: remove Create() and add these?
	//CreateBuffers();
	//CreateTextureShader();
} */

Texture::Texture(const GLchar *filename)
{
	m_textureID = LoadTexture(filename, &m_width, &m_height);
}

Texture::Texture(GLuint a_width, GLuint a_height, GLuint *a_pPixels)
{
	m_width = a_width;
	m_height = a_height;

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, a_pPixels);


	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	// delete the shaders
	glDeleteProgram(m_textureID);
}

bool Texture::Create()
{
	CreateBuffers();
	CreateTextureShader();

	//TODO: from Render: steps:
	/*GLvoid CreateBuffers(); //RenderTexture();
	TextureInit
	AddTexture
	GLvoid CreateBuffers(); //RenderTexture(); 	*/

	return false;
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
GLuint Texture::TextureInit(const GLchar* a_path)
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
GLvoid Texture::GenTexture(const GLchar* a_path, GLuint a_TextureType)
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

// Load PNG images only
// Returns an OpenGL texture ID
// out_width and out_height are pointers, the function will return the textures width and height
// through these paramaters if not NULL
GLuint Texture::LoadTexture(const GLchar *filename, GLuint *out_width, GLuint *out_height)
{
	GLuint width = 0;
	GLuint height = 0;
	std::vector< GLubyte > pixels; //unsigned char
	std::vector< GLubyte > flipped;

	// populates "pixels" vector with pixel data formated as RGBA
	// width and height are passed in as reference. If the function succeeds, 
	// with and height should be populated with the loaded textures widht and height in pixels
	lodepng::decode(pixels, width, height, filename);

	if (out_width != NULL)	*out_width = width;
	if (out_height != NULL)	*out_height = height;

	// pixels are upside down in lodepng...
	// this will invert the raw pixel data again
	GLuint row_bytes = width * 4;
	for (GLuint i = 0; i < height; i++)
	{
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		//memcpy(*(&pixels[0])+(row_bytes * (height-1-i)), row_pointers[i], row_bytes);
		for (GLuint j = 0; j < row_bytes; j++)
		{
			flipped.push_back(pixels[((height - i - 1) * row_bytes) + j]);
		}
	}

	// TODO:
	//-------------------------------------------------------------------------
	// Procedrally genorate an "ERROR" texture
	// and load it into memory for any textures that could not be found
	//-------------------------------------------------------------------------

	// create an openGL texture
	//-------------------------------------------------------------------------

	//unsigned int textureID = 0;
	glGenTextures(1, &m_textureID); // genorate the texture and store the unique id in textureID

	glBindTexture(GL_TEXTURE_2D, m_textureID); // make the genorated texture the current texture

	// send the pixel data to the current texture
	// NOTE: we need to tell openGL what the current format of the pixels is in RAM,
	//       we then need to tell it what format we want the pixels to be formatted in within video memory (openGL will do the conversion)
	//		 we also need to tell it the size of each color channel, which is GL_UNSIGNED_BYTE (one byte each for Red, Green, Blue and Alpha)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &flipped[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	//-------------------------------------------------------------------------

	// return the genorated texture ID
	return m_textureID;
}

// unloads the texture from graphics memory
// expects a valid openGL texture ID, as returned from the LoadTexture function
GLvoid Texture::DeleteTexture(GLuint a_textureID)
{
	// yep, this is a lit easier than the Load texture function
	glDeleteTextures(1, &a_textureID);
}

struct Vertex
{
	GLfloat x, y, z, w;
	GLfloat nx, ny, nz, nw;
	GLfloat tx, ty, tz, tw;
	GLfloat s, t;
};

GLvoid Texture::CreateBuffers()
{
	/*float vertexData[] = {
	-5,0,5,1,0,1,
	5,0,5,1,1,1,
	5,0,-5,1,1,0,
	-5,0,-5,1,0,0 }; */

	// Recreates the textured quad, except that they now contain normal that point up (0,1,0)
	// and tangents to point in the direction of the texture's S axis (or U axis) which is position X (1,0,0).
	Vertex vertexData[] =
	{
		{ -5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
		{ 5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
		{ 5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ -5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 }
	};
	GLuint indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Add the following line to generate a VertexArrayObject
	glGenVertexArrays(1, &m_mesh.GetVAO());
	glBindVertexArray(m_mesh.GetVAO());

	// ----------------------------------------------------------
	// Generate our GL Buffers
	// Let's move these so that they are all generated together
	// ----------------------------------------------------------
	glGenBuffers(1, &m_mesh.GetVBO());
	//... Code Segment here to bind and fill VBO + IBO
	//
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.GetVBO());
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_mesh.GetIBO());
	// 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((GLchar*)0) + 48);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((GLchar*)0) + 16);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((GLchar*)0) + 32);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//
	//Render::VAO = m_VAO
	//m_pMesh->SetVAO(m_VAO);
	//delete[] vertexData; //TODO: more clean up code?
}

GLvoid Texture::CreateTextureShader()
{
	GLint success = GL_FALSE;

	/// ----------------------------------------------------------
	/// Create shaders via loading files
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into shorts for processign by OpenGL.
	/// ----------------------------------------------------------
	GLushort vsSource = LoadShader(GL_VERTEX_SHADER,	"./data/shaders/texture.vert");
	GLushort fsSource = LoadShader(GL_FRAGMENT_SHADER,	"./data/shaders/texture.frag");

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	m_textureID = glCreateProgram();
	glAttachShader(m_textureID, vsSource);
	glAttachShader(m_textureID, fsSource);
	glLinkProgram(m_textureID);

	//TODO: clean up - look at Render.cpp "TextureLoader" and GPuPa..Em...cpp for tips
	/*int success = GL_FALSE;
	//GLuint iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const GLchar**)&vsSource, 0);
	glCompileShader(iVertexShader);

	GLuint iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(iFragmentShader, 1, (const GLchar**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, iVertexShader);
	glAttachShader(m_programID, iFragmentShader);
	glLinkProgram(m_programID); */

	/// ----------------------------------------------------------
	/// Error checking
	/// ----------------------------------------------------------
	glGetProgramiv(m_textureID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_textureID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_textureID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

//	glDeleteShader(iVertexShader);
//	glDeleteShader(iFragmentShader);
	glDeleteShader(vsSource);
	glDeleteShader(fsSource);
}

GLvoid Texture::Draw(const Camera& m_pCamState)
{
	//TODO: HELP - is this where this should be done?
	DrawTexture(m_pCamState, m_textures["soulspear_d"], m_textures["soulspear_n"]);
	DrawTexture(m_pCamState, m_textures["Pyro_D"], m_textures["Pyro_N"]);
}

/// --------------------------------------------------------------------
/// <summary>
/// Draws Textures 
/// <para><param>P1: Cam</param></para>
/// <para><param>P2: Texture 1 - generally the diffuse.</param></para>
/// <para><param>P3: Texture 2 - generally the normal.</param></para>
/// <para><param>P3: Texture 3 - generally the specular.</param></para>
/// <example> m_textures["soulspear_d"] </example>
/// </summary>
/// --------------------------------------------------------------------
/*GLvoid Texture::DrawTexture(const Camera& m_pCamState, GLuint a_uiTexture1, GLuint a_uiTexture2)
{
	// use our texture program
	glUseProgram(m_textureID);

	// bind the camera
	GLint loc = glGetUniformLocation(m_textureID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_pCamState.getProjectionView()[0][0]));

	/* unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	// unsigned int uiTime = glGetUniformLocation(m_programID, "time");
	// 
	// glUniform1f(uiHeightScale, fHeightScale);
	// glUniform1f(uiTime, fTime);

	//glBindTexture(GL_TEXTURE_2D, m_textures["crate"]); * /

	// Set texture slots
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, a_uiTexture1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, a_uiTexture2);
	//glBindTexture(GL_TEXTURE_2D, m_normalmap["soulspear_n"]);

	// tell the shader where it is
	loc = glGetUniformLocation(m_textureID, "diffuse");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(m_textureID, "normal");
	glUniform1i(loc, 1);

	// bind the light
	glm::vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_textureID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	// draw
	glBindVertexArray(m_mesh.GetVAO()); //TODO: replace m_VAO with VAO
	//glBindVertexArray(m_pMesh->GetVAO()); //TODO: replace m_VAO with VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
} 

GLvoid Texture::DrawTexture(const Camera& m_pCamState, Texture a_uiTexture1, Texture a_uiTexture2)
{

} */
