#include "mythos_window.h"


MythosWindowPtrVector __MYTHOS_ROOT_WINDOWS;

MYTHOS_API MythosWindow* mythosCreateRootWindow(int width, int height, const char* title) {

	MythosWindow* window = new MythosWindow(width, height, title);

	__MYTHOS_ROOT_WINDOWS.emplace_back(window);

	return window;
}

MYTHOS_API MythosWindow* mythosCreateChildWindow(MythosWindow* window, int width, int height, const char* title) {

	if (window == nullptr)
		throw "Window cannot be child of NULL.";
	
	return new MythosChildWindow(window, width, height, title);
}

MYTHOS_API void mythosDestroyWindow(MythosWindow* window) {

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



bool __MYTHOS_NOT_INITIALIZED = true;

/* Like mythosInit(), but more secret :P */
void mythosSecretInit() {

	// TODO: leave in next line or not?
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		// mythos_exit();
		throw "GLEW failed to initialize.";
	}

	__MYTHOS_NOT_INITIALIZED = false;
}


MythosWindow::MythosWindow(int width, int height, const char* title) : mTitle(title) {

	mXRes = 1.0;
	mYRes = 1.0;

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

		mythosSecretInit();
	}


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glLoadIdentity();
	glPushMatrix();

	glOrtho(0.0, width, 0.0, -height, 0.1, -1000.1);
	glScaled(mXRes, mYRes, 1.0);

	/*
	glOrtho(0.0, width, 0.0, height, 0.1, -1000.1);
	glTranslated(0.0, height, 0.0);
	glScaled(windowXRes, -windowYRes, 1.0);
	*/

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	mWidth = width;
	mHeight = height;
}

void MythosWindow::setTitle(const char* title) {
	
	if (!mWindow)
		throw "Tried to change title of non-existant window.";

	glfwSetWindowTitle(mWindow, title);

	mTitle = title;
}

void MythosWindow::setResolution(double xRes, double yRes) {
	
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glLoadIdentity();
	glPushMatrix();

	glOrtho(0.0, mWidth, 0.0, mHeight, 0.1, -1000.1);
	glTranslated(0.0, mHeight, 0.0);
	glScaled(xRes, -yRes, 1.0);

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


void MythosWindow::update(void) {

	MythosContainerWidget::update();

	for (MythosWindowPtrVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->update();
}

MYTHOS_EVENT_RETURN MythosWindow::update(MYTHOS_EVENT_KEY key, const MythosEvent& ptr) {

	MYTHOS_EVENT_RETURN res = MythosContainerWidget::update(key, ptr);

	MythosWindowPtrVector::iterator it = mChildren.begin();

	while (it != mChildren.end() && res == MYTHOS_CONTINUE) {

		res = (*it)->update(key, ptr);

		++it;
	}

	return res;
}

void MythosWindow::render(void) {

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TODO figure out which i need

	MythosContainerWidget::render();

	glfwSwapBuffers(mWindow);

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