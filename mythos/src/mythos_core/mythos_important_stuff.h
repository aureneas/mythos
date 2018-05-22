#pragma once

// Does some important things.

#ifdef MYTHOS_CORE_EXPORTS
#define MYTHOS_API __declspec(dllexport)
#else
#define MYTHOS_API __declspec(dllimport)
#endif

#include <GL/glew.h>
#include <GL/glfw3.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")