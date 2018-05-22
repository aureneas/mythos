#include "main.h"

int main() {
	mythosInit();

	MythosWindow* rootWindow = mythosCreateRootWindow(320, 240, "Test Window");

	mythosRun(rootWindow);

	mythosDestroyWindow(rootWindow);

	mythosExit();

	return 0;
}