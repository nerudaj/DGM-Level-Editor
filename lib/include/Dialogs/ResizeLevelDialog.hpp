#pragma once

#include "DialogBase.hpp"
#include "include/Gui.hpp"
#include "include/Utilities/GC.hpp"

class ResizeDialog final : public DialogInterface
{
private:
    virtual void customOpenCode() override {}

public:
    [[nodiscard]] unsigned getLevelWidth() const;

    [[nodiscard]] unsigned getLevelHeight() const;

    [[nodiscard]] bool isTranslationDisabled() const;

    ResizeDialog(GC<Gui> gui);
    ResizeDialog(const ResizeDialog&) = delete;
    ResizeDialog(ResizeDialog&&) = delete;
};
