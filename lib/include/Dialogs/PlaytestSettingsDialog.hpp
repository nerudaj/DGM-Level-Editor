#pragma once

#include "include/Interfaces/DialogInterfaces.hpp"
#include "include/Interfaces/FileApiInterface.hpp"
#include "include/Utilities/GC.hpp"

class PlaytestSettingsDialog final : public PlaytestSettingsDialogInterface
{
public:
    PlaytestSettingsDialog(
        tgui::Gui& gui,
        tgui::Theme& theme,
        GC<FileApiInterface> fileApi,
        const std::filesystem::path& binaryPath,
        const std::string& launchOptions);

public:
    [[nodiscard]] std::filesystem::path getBinaryPath() const override;

    [[nodiscard]] std::string getLaunchOptions() const override;

private:
    virtual void customOpenCode() override {}
};
