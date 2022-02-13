#pragma once

#include <iostream>

#define GLEW_BUILD
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#ifdef Engine_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
