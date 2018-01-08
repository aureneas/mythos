#include "include/mythos/mythos.h"
#include "include/debug/debug.h"

int main() {
	engine::init_allegro();
	engine::init("config.ini");

	engine::set_state(new engine::DebugState());

	engine::run();
	engine::destroy_allegro();
}
