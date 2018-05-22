#pragma once

#include "mythos_important_stuff.h"
#include "mythos_window.h"

MYTHOS_API void mythosInit(void);

MYTHOS_API void mythosRun(MythosWindow*);

MYTHOS_API void mythosExit(void);

MYTHOS_API MythosWindow* mythosCreateRootWindow(int, int, const char*);

MYTHOS_API MythosWindow* mythosCreateChildWindow(MythosWindow*, int, int, const char*);

MYTHOS_API void mythosDestroyWindow(MythosWindow*);