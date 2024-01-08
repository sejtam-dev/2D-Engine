#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <cstring>
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>


#ifdef Engine_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#if _WIN32
#define __FILENAME__ ({const char * pStr = strrchr(__FILE__, '\\'); pStr ? pStr + 1 : __FILE__;})
#else
#define __FILENAME__ ({const char * pStr = strrchr(__FILE__, '/'); pStr ? pStr + 1 : __FILE__;})
#endif

#define LOG(message) \
    std::cout << "(" << __FILENAME__ << " | " << __LINE__ << "): " << message << std::endl

#define ERROR(message) \
    std::cerr << "(" << __FILENAME__ << " | " << __LINE__ << "): " << message << std::endl

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

static void GLClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char *function, const char *file, const int line) {
    while (const GLenum errorCode = glGetError()) {
        std::string errorStr;
        switch (errorCode) {
            case GL_INVALID_ENUM: errorStr = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE: errorStr = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION: errorStr = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW: errorStr = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW: errorStr = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY: errorStr = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: errorStr = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            default: errorStr = "Undefined";
                break;
        }

        std::cerr << "[OpenGL Error] (" << errorCode << " | " << errorStr << ") => " << function << " | " << file <<
                " | " << line << std::endl;

        return true;
    }

    return false;
}
