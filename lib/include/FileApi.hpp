#pragma once

#include <string>
#include <optional>

class FileApiInterface {
public:
	virtual std::string resolveAppdata() = 0;

	virtual std::optional<std::string> getSaveFileName(const char* filter) = 0;

	virtual std::optional<std::string> getOpenFileName(const char* filter) = 0;

	FileApiInterface() = default;
	FileApiInterface(const FileApiInterface&) = delete;
	virtual ~FileApiInterface() = default;
};

class FileApi final : public FileApiInterface {
public:
	virtual std::string resolveAppdata() override;

	virtual std::optional<std::string> getSaveFileName(const char* filter) override;

	virtual std::optional<std::string> getOpenFileName(const char* filter) override;
};
