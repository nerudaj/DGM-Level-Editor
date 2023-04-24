#pragma once

#include "include/Gui.hpp"
#include "include/Interfaces/DialogInterfaces.hpp"
#include "include/Utilities/GC.hpp"

class ErrorInfoDialog final : public ErrorInfoDialogInterface
{
public:
    ErrorInfoDialog(GC<Gui> gui) noexcept : gui(gui) {}

public:
    virtual void open(const std::string& text) override;

private:
    GC<Gui> gui;
    constexpr static inline const char* DIALOG_ID = "ErrorInfoDialog";
};
