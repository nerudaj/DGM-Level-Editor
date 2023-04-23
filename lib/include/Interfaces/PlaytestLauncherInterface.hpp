#pragma once

#include <string>
#include <filesystem>

#include <TGUI/TGUI.hpp>

class PlaytestLauncherInterface
{
public:
	virtual ~PlaytestLauncherInterface() = default;

public:
	virtual void buildContextMenu(tgui::MenuBar::Ptr menu) = 0;

	[[nodiscard]]
	virtual const std::filesystem::path& getBinaryPath() const noexcept = 0;

	[[nodiscard]]
	virtual const std::string& getLaunchOptions() const noexcept = 0;
};
