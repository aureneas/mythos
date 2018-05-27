#pragma once

#include <string>
#include <list>
#include "mythos_widget.h"


#define MYTHOS_NEAR		0.0
#define MYTHOS_FAR		1000.0


class MythosWindow;

typedef std::shared_ptr<MythosWindow>	MythosWindowPtr;
typedef std::vector<MythosWindowPtr>	MythosWindowPtrVector;

class MYTHOS_CORE_API MythosWindow : public MythosContainerWidget {
	private:

		MythosWindowPtrVector	mChildren;

		GLFWwindow*				mWindow;

		const char*				mTitle;

		int						mWidth;
		int						mHeight;

		double					mXRes;
		double					mYRes;
		bool					mIntRes;

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

		void setResolution(int, int);

		void setResolution(double, double);

		double getXResolution(void) { return mXRes; }

		double getYResolution(void) { return mYRes; }

		virtual MythosWindow* getParentWindow(void) { return nullptr; }

		void addChildWindow(MythosWindow*);

		void removeChildWindow(MythosWindow*);

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


MYTHOS_CORE_API MythosWindow* mythosCreateRootWindow(int, int, const char*);

MYTHOS_CORE_API MythosWindow* mythosCreateChildWindow(MythosWindow*, int, int, const char*);

MYTHOS_CORE_API void mythosDestroyWindow(MythosWindow*); 

MythosWindow* mythosFindWindow(GLFWwindow*);


void mythosTimerCallback(MythosWindow*);