#pragma once

#include <string>

class FileApi {
public:
	static std::string resolveAppdata();

	static std::string getSaveFileName(const char *filter);

	static std::string getOpenFileName(const char* filter);
};