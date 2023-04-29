#pragma once

#include "include/Gui.hpp"
#include "include/Interfaces/DialogInterfaces.hpp"
#include "include/Interfaces/FileApiInterface.hpp"
#include "include/Launcher/PlaytestLauncherOptions.hpp"
#include "include/Utilities/GC.hpp"

class PlaytestSettingsDialog final : public PlaytestSettingsDialogInterface
{
public:
    PlaytestSettingsDialog(
        GC<PlaytestLauncherOptions> options,
        GC<Gui> gui,
        GC<FileApiInterface> fileApi);

public:
    [[nodiscard]] std::filesystem::path getBinaryPath() const override;

    [[nodiscard]] std::string getLaunchParameters() const override;

    [[nodiscard]] std::filesystem::path getWorkingDirPath() const override;

private:
    virtual void customOpenCode() override {}
};
