#include "include/FileApi.hpp"

class FileApiMock final : public FileApiInterface {
public:
	std::string mockFileName = "";

	virtual std::string resolveAppdata() override {
		return "C:\\appdata";
	}

	virtual std::string getSaveFileName(const char* filter) override {
		return mockFileName;
	}

	virtual std::string getOpenFileName(const char* filter) override {
		return mockFileName;
	}
};