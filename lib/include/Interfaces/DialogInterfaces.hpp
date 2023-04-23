#pragma once

#include "include/Dialogs/DialogBase.hpp"
#include <filesystem>
#include <functional>
#include <string>

enum class UserChoice
{
    Confirmed = 0,
    Denied,
    Cancelled
};

class YesNoCancelDialogInterface
{
public:
    virtual void open(
        const std::string title,
        const std::string& text,
        std::function<void(UserChoice)> completedCallback) = 0;

    virtual ~YesNoCancelDialogInterface() = default;
};

class ErrorInfoDialogInterface
{
public:
    virtual ~ErrorInfoDialogInterface() = default;

public:
    virtual void open(const std::string& text) = 0;
};

class PlaytestSettingsDialogInterface : public DialogInterface
{
public:
    PlaytestSettingsDialogInterface(
        tgui::Gui& gui,
        tgui::Theme& theme,
        const std::string& id,
        const std::string& title,
        const std::vector<OptionLine>& options)
        : DialogInterface(gui, theme, id, title, options)
    {
    }

    virtual ~PlaytestSettingsDialogInterface() = default;

public:
    [[nodiscard]] virtual std::filesystem::path getBinaryPath() const = 0;

    [[nodiscard]] virtual std::string getLaunchOptions() const = 0;
};
