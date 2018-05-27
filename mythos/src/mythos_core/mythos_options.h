#ifndef MYTHOS_OPTIONS_H
#define MYTHOS_OPTIONS_H

#include <string>

struct MythosOptions {
	std::string		windowTitle;

	int				windowWidth;
	int				windowHeight;

	double			windowXRes;
	double			windowYRes;

	MythosOptions(std::string title) {
		windowTitle = title;
		windowWidth = 320;
		windowHeight = 240;
		windowXRes = 1.0;
		windowYRes = 1.0;
	}
};

#endif