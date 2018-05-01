#include <fstream>

#include "../../include/debug.h"

std::ofstream file;

void debug_init() {
	file.open("mythos/debug/log.txt", std::ios::out);
}

void debug_log(std::string text) {
	file << text << "\n";
}

void debug_close() {
	file.close();
}