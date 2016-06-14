#include "Helpers.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool CheckCompileStatus(GLuint shaderId)
{
	GLint result = GL_FALSE;
	int logLength = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
		char* logBuffer = NULL;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		logBuffer = new char[logLength];
		glGetShaderInfoLog(shaderId, logLength, NULL, logBuffer);
		std::cerr << "Compile Error: " << logBuffer << std::endl;
		delete[] logBuffer;
		return false;
	}
	return true;
}

bool CheckLinkStatus(GLuint programId)
{
	GLint result = GL_FALSE;
	int logLength = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		char* logBuffer = NULL;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
		logBuffer = new char[logLength];
		glGetProgramInfoLog(programId, logLength, NULL, logBuffer);
		std::cerr << "Link Error: " << logBuffer << std::endl;
		delete[] logBuffer;
		return false;
	}
	return true;
}

std::string ReadFile(const char* fileName)
{
	std::ifstream ifs(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!ifs.is_open()) return "";

	std::ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<char> bytes((unsigned int)fileSize);
	ifs.read(&bytes[0], fileSize);

	return std::string(&bytes[0], (unsigned int)fileSize);
}

/// ----------------------------------------------------------
/// <summary> Load Shader 
/// <para>Vertex, Fragment, Geometry shader files.</para>
/// <param><para>P1: Used to create a shader of 'type'.</para></param>
/// <param><para>P2: File path.</para></param>
/// <param><para>P2: File path.</para></param>
/// </summary>
/// ----------------------------------------------------------
GLuint LoadShader(GLuint a_iType, const GLchar* ac_cPath)
{
	// "rb" - read binary
	FILE* file = fopen(ac_cPath, "rb"); //TODO: fopen_s? - not reading files
	if (file == nullptr)
	{
		return 0;
	}

	// read the shader source
	fseek(file, 0, SEEK_END);
	GLuint uiLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	GLchar* cSource = new GLchar[uiLength + 1];
	memset(cSource, 0, uiLength + 1);
	fread(cSource, sizeof(GLchar), uiLength, file);
	fclose(file);

	GLuint uiShader = glCreateShader(a_iType);
	glShaderSource(uiShader, 1, &cSource, 0);
	glCompileShader(uiShader);
	delete[] cSource;

	return uiShader;
}

bool LoadShaderType(char* filename,
	GLenum shader_type,
	unsigned int* output)
{
	//we want to be able to return if we succeded
	bool succeeded = true;

	//open the shader file
	FILE* shader_file;
	fopen_s(&shader_file, filename, "r");

	//did it open successfully 
	if (shader_file == 0)
	{
		succeeded = false;
	}
	else
	{
		//find out how long the file is
		fseek(shader_file, 0, SEEK_END);
		int shader_file_length = ftell(shader_file);
		fseek(shader_file, 0, SEEK_SET);
		//allocate enough space for the file
		char *shader_source = new char[shader_file_length];
		//read the file and update the length to be accurate
		shader_file_length = fread(shader_source, 1, shader_file_length, shader_file);

		//create the shader based on the type that got passed in
		unsigned int shader_handle = glCreateShader(shader_type);
		//compile the shader
		glShaderSource(shader_handle, 1, &shader_source, &shader_file_length);
		glCompileShader(shader_handle);

		//chech the shader for errors
		int success = GL_FALSE;
		glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			int log_length = 0;
			glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_length);
			char* log = new char[log_length];
			glGetShaderInfoLog(shader_handle, log_length, NULL, log);
			printf("%s\n", log);
			delete[] log;
			succeeded = false;
		}
		//only give the result to the caller if we succeeded
		if (succeeded)
		{
			*output = shader_handle;
		}

		//clean up the stuff we allocated
		delete[] shader_source;
		fclose(shader_file);
	}

	return succeeded;
}

bool LoadShader(
	char* vertex_filename,
	char* geometry_filename,
	char* fragment_filename,
	GLuint* result)
{
	bool succeeded = true;

	*result = glCreateProgram();

	unsigned int vertex_shader;

	if (LoadShaderType(vertex_filename, GL_VERTEX_SHADER, &vertex_shader))
	{
		glAttachShader(*result, vertex_shader);
		glDeleteShader(vertex_shader);
	}
	else
	{
		printf("FAILED TO LOAD VERTEX SHADER\n");
	}

	if (geometry_filename != nullptr)
	{
		unsigned int geometry_shader;
		if (LoadShaderType(geometry_filename, GL_GEOMETRY_SHADER, &geometry_shader))
		{
			glAttachShader(*result, geometry_shader);
			glDeleteShader(geometry_shader);
		}
		else
		{
			printf("FAILED TO LOAD GEOMETRY SHADER\n");
		}
	}
	if (fragment_filename != nullptr)
	{
		unsigned int fragment_shader;
		if (LoadShaderType(fragment_filename, GL_FRAGMENT_SHADER, &fragment_shader))
		{
			glAttachShader(*result, fragment_shader);
			glDeleteShader(fragment_shader);
		}
		else
		{
			printf("FAILED TO LOAD FRAGMENT SHADER\n");
		}
	}

	glLinkProgram(*result);

	GLint success;
	glGetProgramiv(*result, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint log_length;
		glGetProgramiv(*result, GL_INFO_LOG_LENGTH, &log_length);
		char* log = new char[log_length];
		glGetProgramInfoLog(*result, log_length, 0, log);

		printf("ERROR: STUFF DONE SCREWED UP IN UR SHADER BUDDY!\n\n");
		printf("%s", log);

		delete[] log;
		succeeded = false;
	}

	return succeeded;
}

void APIENTRY openglCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (type == GL_DEBUG_TYPE_OTHER)
	{
		return;
	}
	std::cout << "---------------------opengl-callback-start------------" << std::endl;
	//TODO: add/ remove breakpoint on line '101' /231
	std::cout << "message: " << message << std::endl;
	std::cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER";
		break;
	}
	std::cout << std::endl;

	std::cout << "id: " << id;
	std::cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "HIGH";
		break;
	}
	std::cout << std::endl;
	std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

void TurnOnOpenGLDebugLogging()
{
	if (glDebugMessageCallback == nullptr) return;
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	GLuint unusedIds = 0;
	glDebugMessageControl(
		GL_DONT_CARE, // source
		GL_DONT_CARE, // type
		GL_DONT_CARE, // severity
		0,
		&unusedIds,
		true);
}

void _check_gl_error(const char *file, int line) {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		cerr << "GL_" << error.c_str() << " - " << file << ":" << line << endl;
		err = glGetError();
	}
}