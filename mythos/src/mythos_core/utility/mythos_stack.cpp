#include <stack>
#include "mythos_stack.h"

#include <iostream>


std::stack<MythosFloatHMatrix> __mythosFStack;


MythosFloatHMatrix __getIdentityf() {

	MythosFloatHMatrix iMat;

	for (int i = 3; i >= 0; --i)
		iMat(i, i) = 1.0f;

	return iMat;
}


void __mythosStackInit() {

	__mythosFStack.emplace();
	mythosLoadIdentity();
}

void MYTHOS_CORE_API mythosPushMatrix() {

	__mythosFStack.emplace(__mythosFStack.top());
}

void MYTHOS_CORE_API mythosPopMatrix() {

	if (__mythosFStack.size() > 1)
		__mythosFStack.pop();
	else
		throw MythosError("Invalid Stack Pop Operation", "Tried to pop last matrix in stack.");
}

void MYTHOS_CORE_API mythosLoadIdentity() {

	__mythosFStack.top() = __getIdentityf();
}

void MYTHOS_CORE_API mythosTranslatef(float dx, float dy, float dz) {

	MythosFloatHMatrix tMat = __getIdentityf();
	tMat(0, 3) = dx;
	tMat(1, 3) = dy;
	tMat(2, 3) = dz;

	__mythosFStack.top() = __mythosFStack.top() % tMat;
}

void MYTHOS_CORE_API mythosScalef(float sx, float sy, float sz) {

	MythosFloatHMatrix sMat;
	sMat(0, 0) = sx;
	sMat(1, 1) = sy;
	sMat(2, 2) = sz;
	sMat(3, 3) = 1.0f;

	__mythosFStack.top() = __mythosFStack.top() % sMat;
}

vec3f MYTHOS_CORE_API mythosProject(float x, float y) {

	return mythosProject(vec3f(x, y, 0));
}

vec3f MYTHOS_CORE_API mythosProject(float x, float y, float z) {

	return mythosProject(vec3f(x, y, z));
}

vec3f MYTHOS_CORE_API mythosProject(vec3f& pos) {

	Matrix<float, 4, 1> nPos;
	nPos(0, 0) = pos.x;
	nPos(1, 0) = pos.y;
	nPos(2, 0) = pos.z;
	nPos(3, 0) = 1;

	vec3f tPos;

	Matrix<float, 4, 1> tPosM = __mythosFStack.top() % nPos;
	tPos.x = tPosM(0, 0);
	tPos.y = tPosM(1, 0);
	tPos.z = tPosM(2, 0);

	return tPos;
}

void MYTHOS_CORE_API mythosSendMatrixToShader(GLuint loc) {

	glUniformMatrix4fv(loc, 1, GL_FALSE, &__mythosFStack.top().m[0]);
}

void __mythosPrintStack() {
	for (int i = 0; i < 4; ++i) {
		std::cout << "[ ";
		for (int j = 0; j < 4; ++j)
			std::cout << __mythosFStack.top()(i, j) << "\t";
		std::cout << "]\n";
	}
	std::cout << "\n";
}