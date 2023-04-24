#pragma once

#include "include/Interfaces/DialogInterfaces.hpp"
#include "include/Interfaces/PlaytestLauncherInterface.hpp"
#include "include/Interfaces/ProcessCreatorInterface.hpp"
#include "include/Interfaces/ShortcutEngineInterface.hpp"
#include "include/Utilities/GC.hpp"

class PlaytestLauncher final : public PlaytestLauncherInterface
{
public:
    PlaytestLauncher(
        GC<ShortcutEngineInterface> shortcutEngine,
        GC<ProcessCreatorInterface> processCreator,
        GC<PlaytestSettingsDialogInterface> dialogPlaytestSettings,
        std::function<std::string()> getCurrentLevelPathCallback,
        const std::filesystem::path& binaryPath,
        const std::string& launchOptions) noexcept;

public:
    void buildContextMenu(tgui::MenuBar::Ptr menu) override;

    [[nodiscard]] const std::filesystem::path&
    getBinaryPath() const noexcept override
    {
        return binaryPath;
    }

    [[nodiscard]] const std::string& getLaunchOptions() const noexcept override
    {
        return launchOptions;
    }

private:
    void handlePlaytestExecuted();

    void handleConfigureLaunchOptions();

private:
    GC<ShortcutEngineInterface> shortcutEngine;
    GC<ProcessCreatorInterface> processCreator;
    GC<PlaytestSettingsDialogInterface> dialogPlaytestSettings;
    std::function<std::string()> getCurrentLevelPathCallback;
    std::filesystem::path binaryPath;
    std::string launchOptions;
};
