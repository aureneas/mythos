#pragma once

#include <string>
#include <list>
#include "mythos_widget.h"


class MythosWindow;

typedef std::shared_ptr<MythosWindow>	MythosWindowPtr;
typedef std::vector<MythosWindowPtr>	MythosWindowPtrVector;

class MYTHOS_API MythosWindow : public MythosContainerWidget {
	private:

		MythosWindowPtrVector	mChildren;

		GLFWwindow*				mWindow;

		const char*				mTitle;

		int						mWidth;
		int						mHeight;

		double					mXRes;
		double					mYRes;

	public:

		MythosWindow(int, int, const char*);

		virtual ~MythosWindow(void);

		GLFWwindow* getWindow(void) { return mWindow; }

		MythosWindow* findWindow(GLFWwindow*);

		void setDimensions(int, int);

		int getWidth(void) { return mWidth; }

		int getHeight(void) { return mHeight; }

		void setTitle(const char*);

		const char* getTitle(void) { return mTitle; }

		void setResolution(double, double);

		double getXResolution(void) { return mXRes; }

		double getYResolution(void) { return mYRes; }

		virtual MythosWindow* getParentWindow(void) { return nullptr; }

		void addChildWindow(MythosWindow*);

		void removeChildWindow(MythosWindow*);

		void update(void);

		MYTHOS_EVENT_RETURN update(MYTHOS_EVENT_KEY, const MythosEvent&);

		void render(void);
};

class MythosChildWindow : public MythosWindow {
	private:

		MythosWindow*			mParentWindow;

	public:

		MythosChildWindow(MythosWindow*, int, int, const char*);

		~MythosChildWindow(void);

		MythosWindow* getParentWindow() { return mParentWindow; }
};


MYTHOS_API MythosWindow* mythosCreateRootWindow(int, int, const char*);

MYTHOS_API MythosWindow* mythosCreateChildWindow(MythosWindow*, int, int, const char*);

MYTHOS_API void mythosDestroyWindow(MythosWindow*); 

MythosWindow* mythosFindWindow(GLFWwindow*);