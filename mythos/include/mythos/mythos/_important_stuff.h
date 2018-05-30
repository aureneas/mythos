#pragma once

// Does some important things.

#ifdef MYTHOS_CORE_EXPORTS
#define MYTHOS_CORE_API __declspec(dllexport)
#else
#define MYTHOS_CORE_API __declspec(dllimport)
#endif

#include <GL/glew.h>
#include <GL/glfw3.h>

#ifdef GLEW_STATIC

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")

#else

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")

#endif

#define MYTHOS_STACK

#include "error.h"

#define MYTHOS_TRUE		1
#define MYTHOS_FALSE	0