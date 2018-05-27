#include <iostream>
#include "main.h"

using namespace std;

MYTHOS_EVENT_RETURN printOnKeyPress(MythosWidget* widget, const MythosEvent& e) {

	cout << "Key pressed.\n";

	return MYTHOS_CONTINUE;
}

int main() {

	//return mythosTutorial();

	///*
	mythosInit();

	MythosWindow* rootWindow = mythosCreateRootWindow(600, 400, "Test Window");

	MythosImage imgData = mythosLoadImage("../../../src/test/img/test1.png");
	rootWindow->addChildWidget(new MythosImageWidget(&imgData, vec2f(50.0f, 50.0f)));

	rootWindow->setEventFunc(MYTHOS_KEY_PRESS, printOnKeyPress);

	mythosRun(rootWindow);

	mythosDestroyWindow(rootWindow);

	mythosExit();
	//*/
	return 0;
}