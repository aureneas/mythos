#pragma once

#include "matrix.h"

typedef Matrix<float, 4, 4> MythosFloatHMatrix;

void __mythosStackInit(void);

void MYTHOS_CORE_API mythosPushMatrix(void);
void MYTHOS_CORE_API mythosPopMatrix(void);

void MYTHOS_CORE_API mythosLoadIdentity(void);

void MYTHOS_CORE_API mythosTranslatef(float, float, float);
void MYTHOS_CORE_API mythosScalef(float, float, float);

vec3f MYTHOS_CORE_API mythosProject(float, float);
vec3f MYTHOS_CORE_API mythosProject(float, float, float);
vec3f MYTHOS_CORE_API mythosProject(vec3f&);

void MYTHOS_CORE_API mythosSendMatrixToShader(GLuint);

void __mythosPrintStack(void);