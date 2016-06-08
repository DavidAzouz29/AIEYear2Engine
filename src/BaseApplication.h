#pragma once
#include <gl_core_4_4.h> //#include "gl_core_4_4.h"

struct GLFWwindow;

class BaseApplication {
public:

	BaseApplication() = default;
	virtual ~BaseApplication() = default;

	void run();
	
	virtual bool startup() = 0;
	virtual GLvoid shutdown() = 0;

	virtual bool Update(float deltaTime) = 0;
	virtual GLvoid Draw() = 0;

protected:

	virtual bool createWindow(const char* title, int width, int height);
	virtual GLvoid destroyWindow();

	GLFWwindow*	m_pWindow;
};