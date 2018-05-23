#include <iostream>
#include "main.h"

using namespace std;

MYTHOS_EVENT_RETURN printOnKeyPress(MythosWidget* widget, const MythosEvent& e) {

	cout << "Key pressed.\n";

	return MYTHOS_CONTINUE;
}

int main() {

	mythosInit();

	MythosWindow* rootWindow = mythosCreateRootWindow(320, 240, "Test Window");

	rootWindow->setEventFunc(MYTHOS_KEY_PRESS, printOnKeyPress);

	mythosRun(rootWindow);

	mythosDestroyWindow(rootWindow);

	mythosExit();

	return 0;
}