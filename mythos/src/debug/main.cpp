#include "../mythos_core/mythos_core.h"

int main() {
	MythosOptions* opt = new MythosOptions();

	mythos_core::init(opt);

	while (true) {}

	mythos_core::shutdown();
}