#include <mythos\window.h>
#include <mythos\texture\texture.h>
#include <mythos\utility\stack.h>

#include <iostream>
#include <mythos\shaders\shader.h>


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

	__mythosStackInit();
	__mythosTextureInit();

	__MYTHOS_NOT_INITIALIZED = false;
}


MythosWindow::MythosWindow(int width, int height, const char* title) 
	: MythosGenericContainerWidget(vec2f(0.0f, 0.0f)) {

	mTitle = title;
	mXRes = 1.0;
	mYRes = 1.0;

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

void MythosWindow::setResolution(float xRes, float yRes) {
	
	mXRes = xRes;
	mYRes = yRes;
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

	MYTHOS_EVENT_RETURN res = MythosGenericContainerWidget::update(key, ptr);

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, mWidth, mHeight);

	mythosLoadIdentity();
	
	mythosPushMatrix();
	mythosScalef(2 * mXRes / mWidth, -2 * mYRes / mHeight, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO figure out if i need more here?

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	MythosGenericContainerWidget::render();

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