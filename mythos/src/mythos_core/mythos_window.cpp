#include "mythos_window.h"

bool _GLEW_NOT_INITIALIZED = true;

MythosWindow::MythosWindow(int width, int height, const char* title, MythosWindow* contextSharing) : mTitle(title) {

	mXRes = 1.0;
	mYRes = 1.0;

	mWindow = glfwCreateWindow(
		width, 
		height,
		mTitle, 
		nullptr, 
		(contextSharing ? contextSharing->mWindow : nullptr)
	);

	if (!mWindow)
		throw "GLFW window failed to initialize.";

	glfwMakeContextCurrent(mWindow);


	if (_GLEW_NOT_INITIALIZED) {

		// TODO: leave in next line or not?
		glewExperimental = true;

		if (glewInit() != GLEW_OK) {
			// mythos_exit();
			throw "GLEW failed to initialize.";
		}

		_GLEW_NOT_INITIALIZED = false;
	}


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	setDimensions(width, height);
}



MythosWindow::~MythosWindow() {

	__ChildVector children = mChildren;
	
	for (__ChildVector::iterator it = children.begin(); it != children.end(); ++it) 
		delete *it;
	
	if (mWindow)
		glfwDestroyWindow(mWindow);
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


void MythosWindow::addChild(MythosWindow* child) {
	
	mChildren.push_back(child);
}

void MythosWindow::removeChild(MythosWindow* child) {

	for (__ChildVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {
		
		if (*it == child) {
			mChildren.erase(it);
			break;
		}
	}
}


void MythosWindow::update(void) {

	// TODO update per frame

	for (__ChildVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->update();
}

void MythosWindow::render(void) {

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TODO figure out which i need

	// TODO the actual rendering

	glfwSwapBuffers(mWindow);

	for (__ChildVector::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->render();
}


MythosChildWindow::MythosChildWindow(MythosWindow* parent, int width, int height, const char* title) : MythosWindow(width, height, title) {

	mParent = parent;
	mParent->addChild(this);
}

MythosChildWindow::~MythosChildWindow() {
	
	mParent->removeChild(this);

	MythosWindow::~MythosWindow();
}