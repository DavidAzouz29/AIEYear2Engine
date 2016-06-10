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
/// lodepng: Add this back in one day?
/// </summary>
/// ----------------------------------------------------------

#include "Texture.h"

//#include "lodepng.h"
#include <GLFW\glfw3.h>
#include <stb_image.h>

#include <assert.h>
#include <cstdio>

const Texture Texture::Invalid = Texture();

Texture::Texture(const GLchar *a_filename) :
	sPath(a_filename)
{
	// Load diffuse map
	GLint imageWidth = 0, imageHeight = 0, imageFormat = 0;

	/// ----------------------------------------------------------
	// This call will read in the default texel data for the crate.png.
	// In the case the image stores RGB values at 515x512 resolution.
	/// ----------------------------------------------------------
	GLubyte* data = stbi_load(a_filename, &imageWidth, &imageHeight, &imageFormat, STBI_default);
	/// ----------------------------------------------------------
	assert(data != nullptr);

	/// --------------------
	// Image Format
	/// --------------------
	GLenum E_FORMAT;
	switch (imageFormat)
	{
	case 1:
	{
		E_FORMAT = GL_RED;
		break;
	}
	case 2:
	{
		E_FORMAT = GL_RG;
		break;
	}
	case 3:
	{
		E_FORMAT = GL_RGB;
		break;
	}
	case 4:
	{
		E_FORMAT = GL_RGBA;
		break;
	}
	default:
	{
		assert(false);
	}
	}
	/// --------------------

	// Generate an OpenGL texture handle.
	glGenTextures(1, &m_textureID);
	// Bind the texture to the correct slot for the dimension, in this case 2-D.
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// Specify the data for the texture, including the format, resolution and variable type.
	// Out data is an unsigned char, therefor it should be GL_UNSIGNED_BYTE.
	glTexImage2D(GL_TEXTURE_2D, 0, E_FORMAT, imageWidth, imageHeight, 0, E_FORMAT, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

//TODO: if lodepng
/// --------------------------------------------------------------------
// Load PNG images only
// Returns an OpenGL texture ID
// out_width and out_height are pointers, the function will return the textures width and height
// through these paramaters if not NULL
/// --------------------------------------------------------------------
/*Texture::Texture(const GLchar *filename, GLuint *out_width, GLuint *out_height)
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
} */

/// --------------------------------------------------------------------
/// <summary>
/// Note: Can be used for procedural textures.
/// <para><param>P1: width.</param></para>
/// <para><param>P2: height.</param></para>
/// <para><param>P3: format: GL_R32F.</param></para>
/// <para><param>P4: components: GL_RGB.</param></para>
/// <para><param>P5: type: e.g. GL_UNSIGNED_BYTE.</param></para>
/// <para><param>P6: pixel data.</param></para>
/// <example> Texture </example>
/// </summary>
/// --------------------------------------------------------------------
Texture::Texture(GLuint a_width, GLuint a_height, GLenum a_format, GLenum a_components, GLenum a_type, const GLvoid* a_pPixels)
{
	m_width = a_width;
	m_height = a_height;

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, a_format, m_width, m_height, 0, a_components, a_type, a_pPixels);
}

// --------------------------------------------------------------------
// unloads the texture from graphics memory
// expects a valid openGL texture ID, as returned from the LoadTexture function
// --------------------------------------------------------------------
Texture::~Texture()
{
	if (m_textureID != Invalid.m_textureID)
	{
		// delete the Textures/ shaders
		glDeleteTextures(1, &m_textureID);
	}
}

// Move assignment operator
// Texture passed in is going to be destroyed
Texture& Texture::operator=(Texture&& a_copy)
{
	//if we're not self assigning
	if (this != &a_copy)
	{
		// If not invalid...
		if (m_textureID != Invalid.m_textureID)
		{
			glDeleteTextures(1, &m_textureID);
		}
		//TODO: move
		m_textureID = a_copy.m_textureID;
		sPath = a_copy.sPath;
		// invalidating the old data.
		a_copy.m_textureID = Invalid.m_textureID;
		a_copy.sPath = Invalid.sPath;
	}

	return *this;
}

//TODO: Texture Shader
/*
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							out vec2 vTexCoord; \
							uniform mat4 ProjectionView; \
							void main() { vTexCoord = TexCoord; \
							gl_Position = ProjectionView * Position; }";

	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							out vec4 FragColor; \
							uniform sampler2D diffuse; \
							void main() { FragColor = texture(diffuse,vTexCoord); }";

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	//int success = GL_FALSE;
	unsigned int iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(iVertexShader);

	unsigned int iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(iFragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, iVertexShader);
	glAttachShader(m_programID, iFragmentShader);
	glLinkProgram(m_programID);

	/*glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
	int infoLogLength = 0;
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	char* infoLog = new char[infoLogLength];

	glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
	printf("Error: Failed to link shader program!\n");
	printf("%s\n", infoLog);
	delete[] infoLog;
	} //* /

	glDeleteShader(iVertexShader);
	glDeleteShader(iFragmentShader);
*/

//TODO: Render Texture
/*
	float vertexData[] = {
		-5,0,5,1,0,1,
		5,0,5,1,1,1,
		5,0,-5,1,1,0,
		-5,0,-5,1,0,0 };

	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Add the following line to generate a VertexArrayObject
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// ----------------------------------------------------------
	// Generate our GL Buffers
	// Let's move these so that they are all generated together
	// ----------------------------------------------------------
	glGenBuffers(1, &m_VBO);
	//... Code Segment here to bind and fill VBO + IBO
	//
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IBO);
	// 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0)+16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//
	//Render::VAO = m_VAO
	//Gem->SetVAO(m_VAO);
	//delete[] vertexData; //TODO: more clean up code?
*/