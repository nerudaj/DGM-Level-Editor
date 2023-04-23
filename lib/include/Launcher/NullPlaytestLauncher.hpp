#pragma once

#include "include/Interfaces/PlaytestLauncherInterface.hpp"

class NullPlaytestLauncher final : public PlaytestLauncherInterface
{
public:
	void buildContextMenu(tgui::MenuBar::Ptr) override {}

	[[nodiscard]]
	const std::filesystem::path& getBinaryPath() const noexcept override
	{
		return path;
	}

	[[nodiscard]]
	const std::string& getLaunchOptions() const noexcept
	{
		return str;
	}

private:
	std::string str = "";
	std::filesystem::path path;
};
