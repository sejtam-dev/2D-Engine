#pragma once

#include "GL/glew.h"

#define GLEW_BUILD
#undef GLEW_STATIC

#include <GLFW/glfw3.h>

#include <string>

#include <fstream>
#include <sstream>
#include <iostream>



#ifdef Engine_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#ifdef _DEBUG
#define GLCall( x ) \
     GLClearErrors(); \
     x; \
     if ( GLLogCall( #x, __FILE__, __LINE__) ) __debugbreak();
#define GLCallReturn( x ) [&]() { \
     GLClearErrors(); \
     auto retVal = x; \
     if ( GLLogCall( #x, __FILE__, __LINE__) ) __debugbreak(); \
     return retVal; \
   }()
#else
#define GLCallV( x ) x
#define GLCall( x ) x
#endif

static void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, const int line)
{
	while(const GLenum error = glGetError())
	{
		std::cerr << "[OpenGL Error] (" << error << ") => " << function << " | " << file << " | " << line << std::endl;
		return true;
	}

	return false;
}