#pragma once

#include "matrix.h"

#define MYTHOS_MODELVIEW 0

void __mythosStackInit(void);

MYTHOS_CORE_API void			mythosPushMatrix(int = MYTHOS_MODELVIEW);
MYTHOS_CORE_API void			mythosPopMatrix(int = MYTHOS_MODELVIEW);

MYTHOS_CORE_API int				mythosCreateStack(int);
MYTHOS_CORE_API int				mythosCloneStack(int, int);
MYTHOS_CORE_API void			mythosDeleteStack(int);

MYTHOS_CORE_API const mat4f&	mythosGetStackMatrix(int = MYTHOS_MODELVIEW);

MYTHOS_CORE_API void			mythosLoadIdentity(int = MYTHOS_MODELVIEW);

MYTHOS_CORE_API void			mythosTranslatef(float, float, float, int = MYTHOS_MODELVIEW);
MYTHOS_CORE_API void			mythosTranslatef(const vec4f&, int = MYTHOS_MODELVIEW);
MYTHOS_CORE_API void			mythosScalef(float, float, float, int = MYTHOS_MODELVIEW);
MYTHOS_CORE_API void			mythosRotateXf(float, int = MYTHOS_MODELVIEW);
MYTHOS_CORE_API void			mythosRotateYf(float, int = MYTHOS_MODELVIEW);
MYTHOS_CORE_API void			mythosRotateZf(float, int = MYTHOS_MODELVIEW);

MYTHOS_CORE_API void			mythosTransformf(mat4f&, mat4f&, int = MYTHOS_MODELVIEW);
void							__mythosTransformf(mat4f&, int);
void							__mythosInverseTransformf(mat4f&, int);

MYTHOS_CORE_API vec4f			mythosProject(float, float, int = MYTHOS_MODELVIEW);
MYTHOS_CORE_API vec4f			mythosProject(float, float, float, int = MYTHOS_MODELVIEW);
MYTHOS_CORE_API vec4f			mythosProject(const vec4f&, int = MYTHOS_MODELVIEW);

MYTHOS_CORE_API vec4f			mythosUnproject(float, float, int = MYTHOS_MODELVIEW);
MYTHOS_CORE_API vec4f			mythosUnproject(const vec2f&, int = MYTHOS_MODELVIEW);

MYTHOS_CORE_API void			mythosSendMatrixToShader(GLuint, int = MYTHOS_MODELVIEW);

void							__mythosPrintStack(int = MYTHOS_MODELVIEW);