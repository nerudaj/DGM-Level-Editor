#include "include/FileApi.hpp"

class FileApiMock final : public FileApiInterface
{
public:
	std::string mockFileName = "";
	bool userCancelled = false;

	virtual std::string resolveAppdata() override
	{
		return "C:\\appdata";
	}

	virtual std::string getSaveFileName(const char* filter) override
	{
		if (userCancelled)
			throw std::runtime_error("File select dialog failed");
		return mockFileName;
	}

	virtual std::string getOpenFileName(const char* filter) override
	{
		if (userCancelled)
			throw std::runtime_error("File select dialog failed");
		return mockFileName;
	}
};