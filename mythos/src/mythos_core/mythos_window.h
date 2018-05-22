#pragma once

#include <string>
#include <vector>
#include "mythos_important_stuff.h"


class MYTHOS_API MythosWindow {
	private:

		typedef std::vector<MythosWindow*> __ChildVector;

		__ChildVector	mChildren;

		GLFWwindow*		mWindow;

		const char*		mTitle;

		int				mWidth;
		int				mHeight;

		double			mXRes;
		double			mYRes;

	public:

		MythosWindow(int, int, const char*, MythosWindow* = nullptr);

		virtual ~MythosWindow(void);

		GLFWwindow* getWindow(void) { return mWindow; }

		void setDimensions(int, int);

		int getWidth(void) { return mWidth; }

		int getHeight(void) { return mHeight; }

		void setTitle(const char*);

		const char* getTitle(void) { return mTitle; }

		void setResolution(double, double);

		double getXResolution(void) { return mXRes; }

		double getYResolution(void) { return mYRes; }

		virtual MythosWindow* getParent(void) { return nullptr; }

		void addChild(MythosWindow*);

		void removeChild(MythosWindow*);

		void update(void);

		void render(void);
};

class MythosChildWindow : public MythosWindow {
	private:

		MythosWindow*		mParent;

	public:

		MythosChildWindow(MythosWindow*, int, int, const char*);

		~MythosChildWindow(void);

		MythosWindow* getParent() { return mParent; }
};