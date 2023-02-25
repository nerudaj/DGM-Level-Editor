#include "include/Utilities/FileApi.hpp"

class FileApiMock final : public FileApiInterface
{
public:
	std::string mockFileName = "";
	bool userCancelled = false;

	std::string resolveAppdata() const override
	{
		return "C:\\appdata";
	}

	std::optional<std::string> getSaveFileName(const char* filter) const override
	{
		if (userCancelled)
			return {};
		return mockFileName;
	}

	std::optional<std::string> getOpenFileName(const char* filter) const override
	{
		if (userCancelled)
			return {};
		return mockFileName;
	}
};