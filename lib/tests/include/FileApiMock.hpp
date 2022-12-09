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

	virtual std::optional<std::string> getSaveFileName(const char* filter) override
	{
		if (userCancelled)
			return {};
		return mockFileName;
	}

	virtual std::optional<std::string> getOpenFileName(const char* filter) override
	{
		if (userCancelled)
			return {};
		return mockFileName;
	}
};