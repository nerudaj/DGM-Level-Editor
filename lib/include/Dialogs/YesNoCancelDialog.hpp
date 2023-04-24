#pragma once

#include "include/Gui.hpp"
#include "include/Interfaces/DialogInterfaces.hpp"
#include "include/Utilities/GC.hpp"

class YesNoCancelDialog : public YesNoCancelDialogInterface
{
    GC<Gui> gui;
    const std::string DIALOG_ID = "YesNoCancelDialog";

public:
    virtual void open(
        const std::string title,
        const std::string& text,
        std::function<void(UserChoice)> completedCallback) override;

    YesNoCancelDialog(GC<Gui> gui) : gui(gui) {};
};
