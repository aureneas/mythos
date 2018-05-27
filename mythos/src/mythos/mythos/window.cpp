#include "mythos_window.h"
#include "texture/mythos_texture.h"
#include "utility/mythos_stack.h"

#include <iostream>
#include "shaders/mythos_shader.h"


MythosWindowPtrVector __MYTHOS_ROOT_WINDOWS;

MYTHOS_CORE_API MythosWindow* mythosCreateRootWindow(int width, int height, const char* title) {

	MythosWindow* window = new MythosWindow(width, height, title);

	__MYTHOS_ROOT_WINDOWS.emplace_back(window);

	return window;
}

MYTHOS_CORE_API MythosWindow* mythosCreateChildWindow(MythosWindow* window, int width, int height, const char* title) {

	if (window == nullptr)
		throw "Window cannot be child of NULL.";
	
	return new MythosChildWindow(window, width, height, title);
}

MYTHOS_CORE_API void mythosDestroyWindow(MythosWindow* window) {

	if (window->getParentWindow() == nullptr) {
		
		for (MythosWindowPtrVector::iterator it = __MYTHOS_ROOT_WINDOWS.begin(); it != __MYTHOS_ROOT_WINDOWS.end(); ++it) {

			if (it->get() == window) {
				__MYTHOS_ROOT_WINDOWS.erase(it);
				break;
			}
		}
	}
}

MythosWindow* mythosFindWindow(GLFWwindow* window) {

	for (MythosWindowPtrVector::iterator it = __MYTHOS_ROOT_WINDOWS.begin(); it != __MYTHOS_ROOT_WINDOWS.end(); ++it) {

		MythosWindow* res = (*it)->findWindow(window);
		if (res)
			return res;
	}

	return nullptr;
}



// TODO delete later
MythosShader* __mythosSolidShaderProg;
GLint __mythosSolidPos;
GLint __mythosSolidCol;

bool __MYTHOS_NOT_INITIALIZED = true;

/* Like mythosInit(), but more secret :P */
void __mythosSecretInit() {

	
	glewExperimental = true;

	if (glewInit() != GLEW_OK) 
		throw MythosError("GLEW failed to initialize.");

#ifdef MYTHOS_STACK
	__mythosStackInit();
#endif

	__mythosTextureInit();

	// TODO delete
	__mythosSolidShaderProg = mythosGetSolidShader();
	__mythosSolidPos = __mythosSolidShaderProg->getAttribLocation("position");
	__mythosSolidCol = __mythosSolidShaderProg->getUniformLocation("color");

	std::cout << "GL: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLEW: " << glewGetString(GLEW_VERSION) << "\n";
	std::cout << "GLFW: " << glfwGetVersionString() << "\n";

	__MYTHOS_NOT_INITIALIZED = false;
}


MythosWindow::MythosWindow(int width, int height, const char* title) 
	: MythosContainerWidget(vec2f(0.0f, 0.0f)) {

	mTitle = title;
	mXRes = 1.0;
	mYRes = 1.0;
	mIntRes = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	mWindow = glfwCreateWindow(
		width, 
		height,
		mTitle, 
		nullptr, 
		(__MYTHOS_ROOT_WINDOWS.size() > 0 ? __MYTHOS_ROOT_WINDOWS.front()->getWindow() : nullptr)
	);

	if (!mWindow)
		throw "GLFW window failed to initialize.";


	if (__MYTHOS_NOT_INITIALIZED) {

		glfwMakeContextCurrent(mWindow);

		__mythosSecretInit();
	}

	setDimensions(width, height);

	glfwSetKeyCallback(mWindow, mythosKeyCallback);
	glfwSetCursorPosCallback(mWindow, mythosMouseMoveCallback);
	glfwSetMouseButtonCallback(mWindow, mythosMouseButtonCallback);
	glfwSetCursorEnterCallback(mWindow, mythosMouseEnterCallback);
}

MythosWindow::~MythosWindow() {

	if (mWindow)
		glfwDestroyWindow(mWindow);
}

MythosWindow* MythosWindow::findWindow(GLFWwindow* window) {

	if (window == mWindow)
		return this;

	for (MythosWindowPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {

		MythosWindow* res = (*it)->findWindow(window);
		if (res)
			return res;
	}

	return nullptr;
}

void MythosWindow::setDimensions(int width, int height) {

	glfwSetWindowSize(mWindow, width, height);

	mWidth = width;
	mHeight = height;
}

void MythosWindow::setTitle(const char* title) {
	
	if (!mWindow)
		throw "Tried to change title of non-existant window.";

	glfwSetWindowTitle(mWindow, title);

	mTitle = title;
}

void MythosWindow::setResolution(int xRes, int yRes) {

	mXRes = (double)xRes;
	mYRes = (double)yRes;
	mIntRes = true;
}

void MythosWindow::setResolution(double xRes, double yRes) {
	
	mXRes = xRes;
	mYRes = yRes;
	mIntRes = false;
}


void MythosWindow::addChildWindow(MythosWindow* child) {
	
	mChildren.emplace_back(child);
}

void MythosWindow::removeChildWindow(MythosWindow* child) {

	for (MythosWindowPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {
		
		if (it->get() == child) {
			mChildren.erase(it);
			break;
		}
	}
}


MYTHOS_EVENT_RETURN MythosWindow::update(MYTHOS_EVENT_KEY key, const MythosEvent& ptr) {

	MYTHOS_EVENT_RETURN res = MythosContainerWidget::update(key, ptr);

	if (key == MYTHOS_TIMER) {

		MythosWindowPtrVector::iterator it = mChildren.begin();

		while (it != mChildren.end() && res == MYTHOS_CONTINUE) {

			res = (*it)->update(key, ptr);

			++it;
		}
	}

	return res;
}

void MythosWindow::render(void) {

	glfwMakeContextCurrent(mWindow);

#ifndef MYTHOS_STACK
	static const double zNear = MYTHOS_NEAR + 0.1;
	static const double zFar = -(MYTHOS_FAR + 0.1);
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, mWidth, mHeight);

#ifdef MYTHOS_STACK

	mythosLoadIdentity();
	
	mythosPushMatrix();
	mythosTranslatef(-1.0f, 1.0f, MYTHOS_FAR);
	mythosScalef(2 * mXRes / mWidth, -2 * mYRes / mHeight, 1.0f);

#else

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glPushMatrix();
	glOrtho(0.0, mWidth, 0.0, mHeight, zNear, zFar);
	glTranslated(0.0, mHeight, 0.0);
	glScaled(mXRes, -mYRes, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

#endif

//	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO figure out if i need more here?

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	MythosContainerWidget::render();

	glfwSwapBuffers(mWindow);

#ifdef MYTHOS_STACK

	mythosPopMatrix();

#else

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

#endif

	for (MythosWindowPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->render();
}


MythosChildWindow::MythosChildWindow(MythosWindow* parent, int width, int height, const char* title) : MythosWindow(width, height, title) {

	mParentWindow = parent;
	mParentWindow->addChildWindow(this);
}

MythosChildWindow::~MythosChildWindow() {
	
	mParentWindow->removeChildWindow(this);

	MythosWindow::~MythosWindow();
}