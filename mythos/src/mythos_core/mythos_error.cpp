#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "mythos_error.h"


MythosError::MythosError(const char* message) : MythosError(std::string("Error"), std::string(message)) {}

MythosError::MythosError(const char* header, const char* message) : MythosError(std::string(header), std::string(message)) {}

MythosError::MythosError(std::string message) : MythosError(std::hash<std::string>()(message), std::string("Error"), message) {}

MythosError::MythosError(std::string header, std::string message) : MythosError(std::hash<std::string>()(header), header, message) {}

MythosError::MythosError(std::size_t errorHash, std::string header, std::string message) {

	mMessage = message;

	mHash = errorHash;
	mHeader = header + std::string(" ") + std::to_string(mHash);
}

void MythosError::display() {

	MessageBox(NULL, mMessage.c_str(), mHeader.c_str(), MB_OK);
}

MYTHOS_CORE_API void mythosDisplayError(const char* message) {

	MythosError(message).display();
}

MYTHOS_CORE_API void mythosDisplayError(const char* header, const char* message) {

	MythosError(header, message).display();
}

MYTHOS_CORE_API void mythosDisplayError(std::string message) {

	MythosError(message).display();
}

MYTHOS_CORE_API void mythosDisplayError(std::string header, std::string message) {

	MythosError(header, message).display();
}