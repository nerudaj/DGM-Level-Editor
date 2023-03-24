#pragma once

#include "include/Interfaces/PlaytestLauncherInterface.hpp"
#include "include/Interfaces/ShortcutEngineInterface.hpp"
#include "include/Utilities/GC.hpp"

class PlaytestLauncher final : public PlaytestLauncherInterface
{
public:
	PlaytestLauncher(
		tgui::Gui& gui,
		GC<ShortcutEngineInterface> shortcutEngine,
		const std::filesystem::path& binaryPath,
		const std::string& launchOptions) noexcept;

public:
	void buildContextMenu(tgui::MenuBar::Ptr menu) override;

	[[nodiscard]]
	const std::filesystem::path& getBinaryPath() const noexcept override
	{
		return binaryPath;
	}

	[[nodiscard]]
	const std::string& getLaunchOptions() const noexcept override
	{
		return launchOptions;
	}

private:
	void handlePlaytestExecuted();

	void handleConfigureLaunchOptions();

private:
	tgui::Gui& gui;
	GC<ShortcutEngineInterface> shortcutEngine;
	std::filesystem::path binaryPath;
	std::string launchOptions;
};
