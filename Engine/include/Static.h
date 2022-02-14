#pragma once

#define GLEW_BUILD
#include "GL/glew.h"
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
