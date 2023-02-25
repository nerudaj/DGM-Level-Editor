#pragma once

#include <string>
#include <optional>

#include "include/Interfaces/FileApiInterface.hpp"

class FileApi final : public FileApiInterface
{
public:
	std::string resolveAppdata() const override;

	std::optional<std::string> getSaveFileName(const char* filter) const override;

	std::optional<std::string> getOpenFileName(const char* filter) const override;
};
