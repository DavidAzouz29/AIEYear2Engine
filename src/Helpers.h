#pragma once

#include <gl_core_4_4.h>
#include <fstream>
#include <iostream>


// Check whether a shader compile succeeded.
// Logs to stderr and returns false if ti has failed.
bool CheckCompileStatus(GLuint shaderId);

// Check whether a program link succeeded.
// Logs to stderr and returns false if ti has failed.
bool CheckLinkStatus(GLuint programId);

// Read an entire file from disk.
// Returns an empty string on failure.
std::string ReadFile(const char* fileName);

// Register the OpenGL debugging callback.
void TurnOnOpenGLDebugLogging();

void _check_gl_error(const char *file, int line);
#define check_gl_error() _check_gl_error(__FILE__,__LINE__);