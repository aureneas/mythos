#include <stack>
#include <unordered_map>
#include <mythos\utility\stack.h>

#include <iostream>


typedef std::stack<mat4f> __MythosStack;
typedef std::unordered_map<int, __MythosStack> __MythosStackMap;

__MythosStackMap __mythosStack;


mat4f __getIdentityf() {

	mat4f iMat;

	for (int i = 3; i >= 0; --i)
		iMat(i, i) = 1.0f;

	return iMat;
}


void __mythosStackInit() {

	mythosCreateStack(MYTHOS_MODELVIEW);
}

__MythosStack& __mythosGetStack(int index) {

	__MythosStackMap::iterator it = __mythosStack.find(index);

	if (it == __mythosStack.end())
		throw MythosError("Invalid Stack Index", "Passed index of stack that doesn't exist.");

	return it->second;
}

MYTHOS_CORE_API void mythosPushMatrix(int index) {

	__MythosStack& stack = __mythosGetStack(index);
	stack.emplace(stack.top());
}

MYTHOS_CORE_API void mythosPopMatrix(int index) {

	__MythosStack& stack = __mythosGetStack(index);

	if (stack.size() <= 1)
		throw MythosError("Stack Pop Error", "Cannot pop last matrix in stack.");

	stack.pop();
}

MYTHOS_CORE_API int mythosCreateStack(int index) {

	__MythosStackMap::iterator it = __mythosStack.find(index);

	if (it != __mythosStack.end())
		throw MythosError("Stack Occupied Index Error", "Attempted to create stack with an index already in use.");

	__MythosStack stack;
	stack.push(__getIdentityf());

	__mythosStack.emplace(index, stack);

	if (index >= 0)
		mythosCreateStack(-(index + 1));

	return index;
}

MYTHOS_CORE_API int mythosCloneStack(int indexCloneFrom, int indexCloneTo) {

	__MythosStackMap::iterator itCloneTo = __mythosStack.find(indexCloneTo);

	if (itCloneTo != __mythosStack.end())
		throw MythosError("Stack Occupied Index Error", "Attempted to create stack with an index already in use.");

	__MythosStack& stackCloneFrom = __mythosGetStack(indexCloneFrom);

	__mythosStack.emplace(indexCloneTo, stackCloneFrom);

	if (indexCloneTo >= 0)
		mythosCloneStack(-(indexCloneFrom + 1), -(indexCloneTo + 1));

	return indexCloneTo;
}

MYTHOS_CORE_API void mythosDeleteStack(int index) {

	__MythosStackMap::iterator it = __mythosStack.find(index);

	if (it == __mythosStack.end())
		throw MythosError("Invalid Stack Index", "Passed index of stack that doesn't exist.");

	__mythosStack.erase(it);

	if (index >= 0)
		mythosDeleteStack(-(index + 1));
}

MYTHOS_CORE_API const mat4f& mythosGetStackMatrix(int index) {

	__MythosStack& stack = __mythosGetStack(index);
	return stack.top();
}

MYTHOS_CORE_API void mythosLoadIdentity(int index) {

	__MythosStack& stack = __mythosGetStack(index);

	stack.top() = __getIdentityf();

	if (index >= 0)
		mythosLoadIdentity(-(index + 1));
}

MYTHOS_CORE_API void mythosTranslatef(float dx, float dy, float dz, int index) {

	mat4f tMat = __getIdentityf();
	tMat(0, 3) = dx;
	tMat(1, 3) = dy;
	tMat(2, 3) = dz;

	mat4f tMatInv = __getIdentityf();
	tMatInv(0, 3) = -dx;
	tMatInv(1, 3) = -dy;
	tMatInv(2, 3) = -dz;

	mythosTransformf(tMat, tMatInv, index);
}

MYTHOS_CORE_API void mythosTranslatef(const vec4f& disp, int index) {

	mat4f tMat = __getIdentityf();
	tMat(0, 3) = disp.x;
	tMat(1, 3) = disp.y;
	tMat(2, 3) = disp.z;

	mat4f tMatInv = __getIdentityf();
	tMatInv(0, 3) = -disp.x;
	tMatInv(1, 3) = -disp.y;
	tMatInv(2, 3) = -disp.z;

	mythosTransformf(tMat, tMatInv, index);
}

MYTHOS_CORE_API void mythosScalef(float sx, float sy, float sz, int index) {

	mat4f sMat;
	sMat(0, 0) = sx;
	sMat(1, 1) = sy;
	sMat(2, 2) = sz;
	sMat(3, 3) = 1.0f;

	mat4f sMatInv;
	sMat(0, 0) = 1.0f / sx;
	sMat(1, 1) = 1.0f / sy;
	sMat(2, 2) = 1.0f / sz;
	sMat(3, 3) = 1.0f;

	mythosTransformf(sMat, sMatInv, index);
}

MYTHOS_CORE_API void mythosRotateXf(float theta, int index) {

	float cosTheta = cosf(theta);
	float sinTheta = sinf(theta);

	mat4f rMat;
	rMat(0, 0) = 1.0f;
	rMat(1, 1) = cosTheta;
	rMat(1, 2) = -sinTheta;
	rMat(2, 1) = sinTheta;
	rMat(2, 2) = cosTheta;
	rMat(3, 3) = 1.0f;

	mat4f rMatInv;
	rMatInv(0, 0) = 1.0f;
	rMatInv(1, 1) = cosTheta;
	rMatInv(1, 2) = sinTheta;
	rMatInv(2, 1) = -sinTheta;
	rMatInv(2, 2) = cosTheta;
	rMatInv(3, 3) = 1.0f;

	mythosTransformf(rMat, rMatInv, index);
}

MYTHOS_CORE_API void mythosRotateYf(float theta, int index) {

	float cosTheta = cosf(theta);
	float sinTheta = sinf(theta);

	mat4f rMat;
	rMat(0, 0) = cosTheta;
	rMat(0, 2) = sinTheta;
	rMat(1, 1) = 1.0f;
	rMat(2, 0) = -sinTheta;
	rMat(2, 2) = cosTheta;
	rMat(3, 3) = 1.0f;

	mat4f rMatInv;
	rMatInv(0, 0) = cosTheta;
	rMatInv(0, 2) = sinTheta;
	rMatInv(1, 1) = 1.0f;
	rMatInv(2, 0) = -sinTheta;
	rMatInv(2, 2) = cosTheta;
	rMatInv(3, 3) = 1.0f;

	mythosTransformf(rMat, rMatInv, index);
}

MYTHOS_CORE_API void mythosRotateZf(float theta, int index) {

	float cosTheta = cosf(theta);
	float sinTheta = sinf(theta);

	mat4f rMat;
	rMat(0, 0) = cosTheta;
	rMat(0, 1) = -sinTheta;
	rMat(1, 0) = sinTheta;
	rMat(1, 1) = cosTheta;
	rMat(2, 2) = 1.0f;
	rMat(3, 3) = 1.0f;

	mat4f rMatInv;
	rMatInv(0, 0) = cosTheta;
	rMatInv(0, 1) = -sinTheta;
	rMatInv(1, 0) = sinTheta;
	rMatInv(1, 1) = cosTheta;
	rMatInv(2, 2) = 1.0f;
	rMatInv(3, 3) = 1.0f;

	mythosTransformf(rMat, rMatInv, index);
}

MYTHOS_CORE_API void mythosTransformf(mat4f& matrix, mat4f& matrixInverse, int index) {

	__mythosTransformf(matrix, index);
	__mythosInverseTransformf(matrixInverse, -(index + 1));
}

void __mythosTransformf(mat4f& matrix, int index) {

	__MythosStack& stack = __mythosGetStack(index);

	stack.top() = stack.top() % matrix;
}

void __mythosInverseTransformf(mat4f& matrix, int index) {

	__MythosStack& stack = __mythosGetStack(index);

	stack.top() = matrix % stack.top();
}


MYTHOS_CORE_API vec4f mythosProject(float x, float y, int index) {

	return mythosProject(vec4f(x, y), index);
}

MYTHOS_CORE_API vec4f mythosProject(float x, float y, float z, int index) {

	return mythosProject(vec4f(x, y, z), index);
}

MYTHOS_CORE_API vec4f mythosProject(const vec4f& pos, int index) {

	__MythosStack& stack = __mythosGetStack(index);

	return vec4f(stack.top() % pos);
}


mat4f __mythosGetInverse(int index) {

	__MythosStack& stack = __mythosGetStack(-(index + 1));

	return stack.top();
}

MYTHOS_CORE_API vec4f mythosUnproject(float x, float y, int index) {

	return mythosUnproject(vec2f(x, y), index);
}

MYTHOS_CORE_API vec4f mythosUnproject(const vec2f& pos, int index) {

	mat4f inv = __mythosGetInverse(index);

	return vec4f(inv % vec4f(pos));
}


MYTHOS_CORE_API void mythosSendMatrixToShader(GLuint loc, int index) {

	__MythosStack& stack = __mythosGetStack(index);

	glUniformMatrix4fv(loc, 1, GL_FALSE, &stack.top().m[0]);
}

void __mythosPrintStack(int index) {

	__MythosStack& stack = __mythosGetStack(index);
	
	for (int i = 0; i < 4; ++i) {
		std::cout << "[ ";
		for (int j = 0; j < 4; ++j)
			std::cout << stack.top()(i, j) << "\t";
		std::cout << "]\n";
	}
	std::cout << "\n";
}