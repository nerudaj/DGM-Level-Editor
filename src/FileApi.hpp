#pragma once

#include <string>

class FileApi {
public:
	static std::string getSaveFileName();

	static std::string getOpenFileName();
};