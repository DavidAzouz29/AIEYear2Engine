#include "Grid.h"
#include "Renderable.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm\ext.hpp> //perlin
#include <glm/gtc/type_ptr.hpp> //value_ptr

using glm::vec3;

Grid::~Grid()
{
}

bool Grid::Create()
{
	CreateDrawShader();

	GenerateGrid(iGrid, iGrid);
	//TODO: perlin creation
	return true;
}

GLvoid Grid::Update()
{

}

GLvoid Grid::Draw(const Camera& a_camState)
{
	DrawGeometry(a_camState.getProjectionView());
}

/*GLvoid Grid::Destroy()
{

} */

/// -----------------------------
// Renders UI elements via ImGui
/// -----------------------------
GLvoid Grid::RenderUI()
{
	if (ImGui::CollapsingHeader("Perlin Generation"))
	{
		// Locations in Grid format
		/*if (ImGui::TreeNode("Locations"))
		{
			const int MAX_LOCATIONS = 4;
			//static bool selected[MAX_LOCATIONS] = { true, false, false, false };
			for (GLint i = 0; i < MAX_LOCATIONS; i++)
			{
				/ImGui::PushID(i); // TODO: better way to do this?
				std::string sLocationName = "Location ";
				sLocationName = sLocationName + std::to_string(i + 1);
				m_cLocationName[i] = sLocationName.c_str();
				// If button 'A' is pressed...
				if (ImGui::Button(m_cLocationName[i], ImVec2(80, 40)))
				{
					//int x = i % MAX_COLS, y = i / MAX_COLS;
					// ... go to location 'A'
					m_v4EndLocation = m_v4LocationArray[i];
					/*if (selected[i] == 0)
					{
					m_transform[3] = v4Location1;
					}
					else if (selected[i] == 1)
					{
					m_transform[3] = v4Location2;
					}
					else if (selected[i] == 2)
					{
					m_transform[3] = v4Location2;
					}
					else if (selected[i] == 3)
					{
					m_transform[3] = v4Location3;
					} //* /
				}
				if ((i % MAX_COLS) < 3)
				{
					ImGui::SameLine();
				} 
			ImGui::PopID();

			ImGui::TreePop();
		}*/

		/*if (ImGui::Button("Pause"))
		{
			//m_transform[3] = m_transform[3];
		}*/

		ImGui::DragFloat("Height Scale", &fHeightScale, 0.1f, 0.01f, (GLfloat)INT_MAX);
		ImGui::DragInt("Octaves", &uiOctaves, 0.1f, uiOctaves, USHRT_MAX);
		ImGui::DragFloat("Amplitude", &fAmplitude, 0.1f, 0.01f, (GLfloat)INT_MAX);
		ImGui::DragFloat("Persistence", &fPersistence, 0.1f, 0.01f, (GLfloat)INT_MAX);
		/*ImGui::DragFloat("Timer", &m_fTimer, 0.1f, 0.01f, MAX_LOCATIONS * m_fLengthTime + m_fLengthTime);
		ImGui::DragFloat("End Time", &m_fEndTime, 0.1f, 0.01f, (GLfloat)INT_MAX);
		ImGui::DragFloat("Length of time", &m_fLengthTime, 0.1f, 0.01f, MAX_LOCATIONS * m_fLengthTime);
		ImGui::DragFloat("Lerped Interpolant", &m_fTravelLerp, 0.1f, 0.01f, 1.0f);
		ImGui::Separator();
		ImGui::DragFloat4("Camera Location", m_transform[3].data, 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX);
		ImGui::DragFloat4("Location 1", glm::value_ptr(v4Location1), 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX);
		ImGui::DragFloat4("Location 2", glm::value_ptr(v4Location2), 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX);
		ImGui::DragFloat4("Location 3", v4Location3.data, 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX);
		ImGui::DragFloat4("Location 4", v4Location4.data, 1.1f, -(GLfloat)INT_MAX, (GLfloat)INT_MAX); */
	}
}

GLvoid Grid::GenerateGrid(const GLuint a_iRows, const GLuint a_iCols)
{
	Vertex_PositionColor* aoVertices = new Vertex_PositionColor[a_iRows * a_iCols];
	for (GLushort r = 0; r < a_iRows; ++r)
	{
		for (GLushort c = 0; c < a_iCols; ++c)
		{
			aoVertices[r * a_iCols + c].position = glm::vec4((GLfloat)c, 0.0f, (GLfloat)r, 1.0f);

			//create some arbitary colour based off something
			//that might not be related to tiling a texture
			glm::vec3 colour = glm::vec3(sinf((c / (GLfloat)(a_iCols - 1.0f)) * (r / (GLfloat)(a_iRows - 1.0f))));
			aoVertices[r * a_iCols + c].color = glm::vec4(colour, 1.0f);
		}
	}

	/// ----------------------------------------------------------
	/// we'll do more here soon!
	/// ----------------------------------------------------------
	/// defining index count based off quad count (2 triangles per quad)
	/// ----------------------------------------------------------
	GLuint* auiIndices = new GLuint[(a_iRows - 1) * (a_iCols - 1) * 6];

	GLuint index = 0;
	for (GLuint r = 0; r < (a_iRows - 1); ++r)
	{
		for (GLuint c = 0; c < (a_iCols - 1); ++c)
		{
			// triangle 1
			auiIndices[index++] = r * a_iCols + c;
			auiIndices[index++] = (r + 1) * a_iCols + c;
			auiIndices[index++] = (r + 1) * a_iCols + (c + 1);

			// triangle 2
			auiIndices[index++] = r * a_iCols + c;
			auiIndices[index++] = (r + 1) * a_iCols + (c + 1);
			auiIndices[index++] = r * a_iCols + (c + 1);
		}
	}

	m_pRenderable->mesh.SetIndexCount((a_iRows - 1) * (a_iCols - 1) * 6);

	m_pRenderable->mesh.Create(aoVertices, a_iRows * a_iCols, auiIndices, m_pRenderable->mesh.GetIndexCount());

	/// ----------------------------------------------------------
#pragma region Procedural
	/// ----------------------------------------------------------
	/// Grid Size
	GLint iGrid = 64;
	GLfloat *perlin_data = new GLfloat[a_iRows * a_iCols];
	GLfloat fScale = (1.0f / iGrid) * 3;

	for (GLint x = 0; x < iGrid; ++x)
	{
		for (GLint y = 0; y < iGrid; ++y)
		{
			perlin_data[y * iGrid + x] = 0;// glm::perlin(vec2(x, y) * fScale) * 0.5f + 0.5f;

			for (int o = 0; o < uiOctaves; ++o)
			{
				GLfloat fFreq = powf(2, (GLfloat)o);
				GLfloat fPerlin_sample = glm::perlin(glm::vec2((GLfloat)x, (GLfloat)y) * fScale * fFreq) * 0.5f + 0.5f;
				perlin_data[y * iGrid + x] += fPerlin_sample * fAmplitude; //TODO: iGrid.x?
				fAmplitude *= fPersistence;
			} //*/
		}
	}

	//GLuint m_perlinTextureID;
	/*glGenTextures(1, &m_perlinTextureID);
	glBindTexture(GL_TEXTURE_2D, m_perlinTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, iGrid, iGrid, 0, GL_RED, GL_FLOAT, perlin_data); */

	m_perlinTexture = Texture(iGrid, iGrid, GL_R32F, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#pragma endregion

#pragma region Big Old
#pragma region Old
	/// ----------------------------------------------------------
	/// create and bind buffers to a vertex array object
	/// Generates a VBO
	/// ----------------------------------------------------------
	/* TODO: Delete?  glGenBuffers(1, &m_VBO); */
	/*glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (a_iRows * a_iCols) * sizeof(Vertex_PositionColor), aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), (void*)(sizeof(glm::vec4)));*/

	/* TODO: Delete?  glBindBuffer(GL_ARRAY_BUFFER, 0); */

	/// ----------------------------------------------------------
	/// OpenGL Index Buffer
	/// ----------------------------------------------------------
	/* TODO: Delete?  glGenBuffers(1, &m_IBO); */

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (a_iRows - 1) * (a_iCols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	*//* TODO: Delete?  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); */
#pragma endregion

	/// ----------------------------------------------------------
	/// Thing
	/// ----------------------------------------------------------
	/*	// Generate our GL Buffers
	// Let's move these so that they are all generated together
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Add the following line to generate a VertexArrayObject
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	//... Code Segment here to bind and fill VBO + IBO
	//
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (a_iRows * a_iCols) * sizeof(Vertex_PositionColor), aoVertices, GL_STATIC_DRAW);

	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (a_iRows - 1) * (a_iCols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_PositionColor), (void*)(sizeof(glm::vec4)));

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//
	//Render::VAO = m_VAO */
#pragma endregion

	delete[] aoVertices;
}

GLvoid Grid::CreateDrawShader()
{
	/// ----------------------------------------------------------
	/// Create shaders
	/// ----------------------------------------------------------
	/// Storing writing out our shader code into char arrays for processign by OpenGL.
	/// ----------------------------------------------------------
	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							layout(location=2) in vec4 Normal; \
							uniform mat4 ProjectionView; \
							uniform sampler2D perlin_texture; \
							out vec4 vNormal; \
							out vec2 outCoord; \
							void main() { \
							outCoord = TexCoord; \
							vNormal = Normal; \
							vec4 pos = Position; \
							pos.y += texture(perlin_texture, outCoord).r * 5; \
							gl_Position = ProjectionView * pos; }";

	//vec4 P = Position; P.y += tan ( time + Position.x ) * heightScale; \

	const char* fsSource = "#version 410\n \
							in vec2 outCoord; \
							in vec4 vNormal; \
							out vec4 out_color; \
							uniform vec3 LightDir; \
							uniform vec3 LightColour; \
							uniform sampler2D perlin_texture; \
							uniform sampler2D dirt_texture; \
							uniform sampler2D grass_texture; \
							uniform sampler2D snow_texture; \
							void main() { \
							float d = max(0, \
							dot( normalize(vNormal).xyz, LightDir ) ); \
							float height = texture(perlin_texture, outCoord).r; \
							vec4 dirtColor = texture(dirt_texture, outCoord); \
							vec4 grassColor = texture(grass_texture, outCoord); \
							vec4 snowColor = texture(snow_texture, outCoord); \
							float blendValue = smoothstep(0.2, 0.5, height); \
							vec4 color = mix(dirtColor, grassColor, blendValue); \
							blendValue = smoothstep(0.7, 0.8, height); \
							out_color = d * mix(color, snowColor, blendValue); \
							out_color.a = 1; }";

	//TODO: LightColour; \
	//void main() {	out_color = texture(perlin_texture, outCoord).rrrr; \
	//void main() { out_color = vec4(outCoord.xy, 0, 1); \

	/*out_color = d * texture(perlin_texture, outCoord).rrrr; \
	out_color = d * mix(color, snowColor, blendValue); \
	*/
	/// ----------------------------------------------------------
	/// Compile shaders
	/// ----------------------------------------------------------
	int success = GL_FALSE;
	unsigned int iVertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int iFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(iVertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(iVertexShader);
	glShaderSource(iFragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(iFragmentShader);

	m_perlinTextureID = glCreateProgram();
	glAttachShader(m_perlinTextureID, iVertexShader);
	glAttachShader(m_perlinTextureID, iFragmentShader);
	glLinkProgram(m_perlinTextureID);

	glGetProgramiv(m_perlinTextureID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_perlinTextureID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_perlinTextureID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(iFragmentShader);
	glDeleteShader(iVertexShader);
}

GLvoid Grid::DrawGeometry(const glm::mat4& a_projectionView)
{
	fTime = static_cast<float>(glfwGetTime());
	glUseProgram(m_perlinTextureID);
	GLuint projectionViewUniform = glGetUniformLocation(m_perlinTextureID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, &a_projectionView[0][0]); //m_projectionViewMatrix

	/*unsigned int uiHeightScale = glGetUniformLocation(m_programID, "heightScale");
	unsigned int uiTime = glGetUniformLocation(m_programID, "time");

	glUniform1f(uiHeightScale, fHeightScale);
	glUniform1f(uiTime, fTime); //*/

	// Set prelin_texture sampler2D sampler to the tex unit we're going to use, init?
	// tell the shader where it is
	// TODO: Create only once
	glActiveTexture(GL_TEXTURE0);
	GLint perlinTextureLocation = glGetUniformLocation(m_perlinTextureID, "perlin_texture");
	glBindTexture(GL_TEXTURE_2D, m_perlinTextureID);

	glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, m_textures["dirt"]);
	glBindTexture(GL_TEXTURE_2D, TextureManager::GetSingleton().LoadTexture("./data/textures/dirt.png")->GetId()); //TODO: clean this up?

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureManager::GetSingleton().LoadTexture("./data/textures/grass.png")->GetId());

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureManager::GetSingleton().LoadTexture("./data/textures/snow.png")->GetId());

	// tell the shader where it is
	glUniform1i(perlinTextureLocation, 0);
	GLint loc = glGetUniformLocation(m_perlinTextureID, "dirt_texture");
	glUniform1i(loc, 1);
	loc = glGetUniformLocation(m_perlinTextureID, "grass_texture");
	glUniform1i(loc, 2);
	loc = glGetUniformLocation(m_perlinTextureID, "snow_texture");
	glUniform1i(loc, 3);

	// bind the position
	vec3 v3Pos(0, 1, 0);
	//vec3 v3Pos(cos(glfwGetTime()), 1, sin(glfwGetTime()));
	loc = glGetUniformLocation(m_perlinTextureID, "Position");
	glUniform3f(loc, v3Pos.x, v3Pos.y, v3Pos.z);

	// bind the Light Dir
	vec3 light(0, 1, 0);
	//vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	loc = glGetUniformLocation(m_perlinTextureID, "LightDir");
	glUniform3f(loc, light.x, light.y, light.z);

	// bind the Light Colour
	//vec3 v3Colour(1, sin(glfwGetTime()), 255);
	//vec3 v3Colour(1, 50, 255); //TODO:
	vec3 v3Colour(0, 0, 1);
	loc = glGetUniformLocation(m_perlinTextureID, "LightColour");
	glUniform3f(loc, v3Colour.x, v3Colour.y, v3Colour.z);
	// TODO: v3Colour *= d 

	loc = glGetUniformLocation(m_perlinTextureID, "CameraPos");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &a_projectionView[0][0]);

	/*/ Specular Power
	// Lower numbers increase the size of the specular shine.
	float fSpecPow = 128.0f;
	loc = glGetUniformLocation(m_perlinTextureID, "SpecPow");
	glUniform1f(loc, fSpecPow); */

	glBindVertexArray(m_pRenderable->mesh.GetVAO()); //TODO: replace m_VAO with VAO
	//unsigned int indexCount = (a_iRows - 1) * (a_iCols - 1) * 6; //TODO: iIndexCount = this formula
	glDrawElements(GL_TRIANGLES, m_pRenderable->mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
}
