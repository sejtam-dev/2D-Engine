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

#ifdef DEBUG
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
#define GLCallReturn( x ) [&]() {\
         return x; \
    }()
#endif

static void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, const int line)
{
	while(const GLenum error = glGetError())
	{
        std::string errorStr;
        switch (error)
        {
            case GL_INVALID_ENUM:                  errorStr = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 errorStr = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             errorStr = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                errorStr = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               errorStr = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 errorStr = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: errorStr = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                               errorStr = "Undefined"; break;
        }

		std::cerr << "[OpenGL Error] (" << error << " | " << errorStr << ") => " << function << " | " << file << " | " << line << std::endl;
		return true;
	}

	return false;
}