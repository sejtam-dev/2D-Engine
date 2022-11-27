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

#if _WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\ ') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/ ') + 1 : __FILE__)
#endif

#define LOG(message) \
    std::cout << "(" << __FILENAME__ << " | " << __LINE__ << "): " << message << std::endl 

#define ERROR(message) \
    std::cerr << "(" << __FILENAME__ << " | " << __LINE__ << "): " << message << std::endl 

#ifdef _DEBUG
#define GLCall( x ) \
     GLClearErrors(); \
     x; \
     if ( GLLogCall( #x) ) __debugbreak();
#define GLCallReturn( x ) [&]() { \
     GLClearErrors(); \
     auto retVal = x; \
     if ( GLLogCall( #x) ) __debugbreak(); \
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

static bool GLLogCall(const char* function)
{
	while(const GLenum errorCode = glGetError())
	{
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }

		ERROR("[OpenGL Error] (" << error << ") => " << function);

		return true;
	}

	return false;
}