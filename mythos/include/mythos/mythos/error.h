#pragma once

#include <string>
#include "_important_stuff.h"


class MYTHOS_CORE_API MythosError {

private:

	std::size_t mHash;

	std::string mHeader;
	std::string mMessage;

public:

	MythosError(const char*);
	MythosError(const char*, const char*);
	MythosError(std::string);
	MythosError(std::string, std::string);
	MythosError(std::size_t, std::string, std::string);

	std::string* getHeader() { return &mHeader; }

	std::string* getMessage() { return &mMessage; }

	void display();
};

MYTHOS_CORE_API void mythosDisplayError(const char*);

MYTHOS_CORE_API void mythosDisplayError(const char*, const char*);

MYTHOS_CORE_API void mythosDisplayError(std::string);

MYTHOS_CORE_API void mythosDisplayError(std::string, std::string);