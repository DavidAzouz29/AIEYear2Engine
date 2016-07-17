#include "BaseApplication.h" //#include "gl_core_4_4.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <thread>

// Initialises OpenGL, GLFW, and ImGui
bool BaseApplication::createWindow(const char* title, int width, int height) {

	if (glfwInit() == GL_FALSE)
		return false;

	m_pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (m_pWindow == nullptr) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_pWindow);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow*, int w, int h){ glViewport(0, 0, w, h); });

	/// ----------------------------------------------------------
#pragma region ImGui
	/// ----------------------------------------------------------
	/// <summary>
	/// Then initialise ImGui
	/// <param>A pointer to the GLFWwindow that was 
	/// returned when you created your window,</param>
	/// <param> and the second is a boolean that specifies whether 
	/// or not to override the GLFW input callbacks.</param>
	/// <param> If you set this to false, you will need to 
	/// manually update ImGui when I key is pressed. 
	/// For now, we’ll set it to true.</param>
	/// </summary>
	/// ----------------------------------------------------------
	ImGui_ImplGlfwGL3_Init(m_pWindow, true);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = (float)width; // m_uiScreenWidth;
	io.DisplaySize.y = (float)height; // m_uiScreenHeight;

#pragma endregion

	// -----------------------
#pragma region OpenGL Version
	// -----------------------
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	// Casting/ turning our version of OpenGL into a float to check our version with < or > signs
	std::string sVersion = std::to_string(major) + "." + std::to_string(minor);
	int iMajor = atoi(&sVersion[0]);
	float fMinor = static_cast<float>(atof(&sVersion[2]));
	fMinor *= 0.100f; // turn 'x' into 0.x
	fOpenGLVersion = static_cast<float>(iMajor) + fMinor;
	std::cout << "GL: " << major << "." << minor << std::endl;
	std::cout << "String: " << sVersion << " Float: " << fOpenGLVersion << std::endl;
#pragma endregion

	// -----------------------
	///<summary>
	/// Threading
	/// may return 0 when not able to detect 
	/// TODO: Threads seek work from a queue if their spin count is greater than 'x'
	/// Research Windows IOCP
	///</summary>
	// -----------------------
	unsigned int concurentThreadsSupported = std::thread::hardware_concurrency();
	std::cout << concurentThreadsSupported << " concurrent threads are supported.\n";
	// We minus one as we are still running our main thread
	threadsAvailable = concurentThreadsSupported - 1;

	glClearColor(0.25f, 0.25f, 0.25f, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	return true;
}

GLvoid BaseApplication::destroyWindow() {

	ImGui_ImplGlfwGL3_Shutdown();
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

GLvoid BaseApplication::run() {

	double prevTime = glfwGetTime();
	double currTime = 0;

	while (currTime = glfwGetTime(),
		Update((float)(currTime - prevTime))) {

		glfwPollEvents();
		Draw();
		glfwSwapBuffers(m_pWindow);

		prevTime = currTime;
	}
}