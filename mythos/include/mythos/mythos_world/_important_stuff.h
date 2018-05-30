#pragma once

// Does some important things.

#ifdef MYTHOS_WORLD_EXPORTS
#define MYTHOS_WORLD_API __declspec(dllexport)
#else
#define MYTHOS_WORLD_API __declspec(dllimport)
#endif

#include <mythos.h>

#ifdef GLEW_STATIC

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")

#else

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")

#endif

#pragma comment(lib, "mythos.lib")