/// Date Created: 21/03/2016
/// viewed: USHRT_MAX http://www.cplusplus.com/reference/climits/

#include "RenderTarget.h"
#include "Render.h"
#include "Mesh.h"
#include "Camera\Camera.h"

#include <glm/ext.hpp>

bool RenderTarget::Create()
{
	RenderTargetLoader();
	CreateRenderTargetQuad();

	return false;
}

GLvoid RenderTarget::Destroy()
{
	// TODO: delete
	//glDeleteVertexArrays(1, &m_VAO);
	//glDeleteBuffers(1, &m_VBO);
	//glDeleteBuffers(1, &m_IBO);

	GLuint texId = m_fboTexture.GetId();
	glDeleteTextures(1, &texId);
	glDeleteRenderbuffers(1, &m_fboDepth);

	m_fboID = USHRT_MAX; //(GLuint)-1;
	//m_fboTexture = USHRT_MAX; //(GLuint)-1; // TODO: Needed?
	//m_fboDepth = USHRT_MAX; //(GLuint)-1; // TODO: Needed?
}

GLvoid RenderTarget::RenderTargetLoader()
{
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const GLchar* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							out vec2 vTexCoord; \
							uniform mat4 ProjectionView; \
							void main() { \
							vTexCoord = TexCoord; \
							gl_Position = ProjectionView * Position; \
							}";

	// RGB x 2 - 1 to move it from RGB to XYZ, or 
	// XYZ x 0.5 + 0.5 to move it from XYZ to RGB.
	const GLchar* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							out vec4 FragColor; \
							uniform sampler2D diffuse; \
							void main() { \
							FragColor = texture(diffuse, vTexCoord);}";

	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	//if (!m_program.create(vsSource, fsSource)) return false; //TODO:
	GLint success = GL_FALSE;
	GLuint iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(iVertexShader, 1, (const GLchar**)&vsSource, 0);
	glCompileShader(iVertexShader);

	GLuint iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(iFragmentShader, 1, (const GLchar**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_fboID = glCreateProgram();
	glAttachShader(m_fboID, iVertexShader);
	glAttachShader(m_fboID, iFragmentShader);
	glLinkProgram(m_fboID);

	glGetProgramiv(m_fboID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetProgramiv(m_fboID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength];

		glGetProgramInfoLog(m_fboID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	} //*/

	glDeleteShader(iVertexShader);
	glDeleteShader(iFragmentShader);

	//m_pMesh.get()->CreateFrame();
	CreateFrame();
}

bool RenderTarget::CreateFrame()
{
	// setup and bind a frambuffer
	glGenFramebuffers(1, &m_fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

	// Construct the FBO texture.
	m_fboTexture = Texture(m_fboSize.x, m_fboSize.y, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// attach it to the framebuffer as the first colour attachment
	// the FBO MUST still be bound
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fboTexture.GetId(), 0);

	// Let the FBO know which attachment to render to
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	
	/// setup and bind a 24bit depth buffer as a render buffer
	glGenRenderbuffers(1, &m_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);

	// -----------------------------------------------------
	// TODO: render buffer / renderbuffer is unrelated to a texture?
	// -----------------------------------------------------
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_fboSize.x, m_fboSize.y); //512
	// while the FBO is still bound
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);
	// -----------------------------------------------------

	// while the FBO is still bound
	/*GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers); */

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error!\n");
	}

	// unbind the FBO so that we can render to the back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

// Render Geometry to the FBO.
GLvoid RenderTarget::CreateRenderTargetQuad()
{
	GLfloat vertexData[] = {
		-5,0, -5,1,0,0,
		5,0,-5,1,1,0,
		5,10,-5,1,1,1,
		-5,10,-5,1,0,1,
	};

	// Recreates the textured quad, except that they now contain normal that point up (0,1,0)
	// and tangents to point in the direction of the texture's S axis (or U axis) which is position X (1,0,0).
	/*	Vertex vertexData[] =
	{
	{ -5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
	{ 5, 0,  5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
	{ 5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
	{ -5, 0, -5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 }
	}; */
	GLuint indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	//TODO:
	m_mesh.Create(vertexData, 6 * 4, indexData, 6);

	/* Add the following line to generate a VertexArrayObject
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_mesh.GetIBO());
	// 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, ((GLchar*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); */
}

GLvoid RenderTarget::RenderRenderTargetQuad(const glm::mat4& a_projectionView)
{
	glUseProgram(m_fboID);

	//glm::mat4 projView = m_pCameraStateMachine->GetCurrentCamera()->getProjectionView();

	// Render Target
	// bind the camera
	GLint loc = glGetUniformLocation(m_fboID, "ProjectionView"); //m_program_ID
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(a_projectionView));

	// Set texture slots
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_pRenderApp->GetSharedPointer()->GetFboTexture());
	//glBindTexture(GL_TEXTURE_2D, GetFboTexture());
	glBindTexture(GL_TEXTURE_2D, GetTexture().GetId());

	// tell the shader where it is
	GLint diffLoc = glGetUniformLocation(m_fboID, "diffuse"); // m_program_ID, m_render.GetProgramID()
	glUniform1i(diffLoc, 0);
}

GLvoid RenderTarget::BindDraw()
{
	glUseProgram(m_fboID);

	glBindVertexArray(m_mesh.GetVAO()); // mesh Vertex Array Object
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
