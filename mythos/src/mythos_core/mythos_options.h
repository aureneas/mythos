#ifndef MYTHOS_OPTIONS_H
#define MYTHOS_OPTIONS_H

#include <string>

struct MythosOptions {
	const char*		windowTitle;

	int				windowWidth;
	int				windowHeight;

	double			windowXRes;
	double			windowYRes;

	MythosOptions() : windowTitle("DEFAULT TITLE") {
		windowWidth = 320;
		windowHeight = 240;
		windowXRes = 1.0;
		windowYRes = 1.0;
	}
};

#endif