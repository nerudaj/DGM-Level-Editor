#pragma once

#include "include/Dialogs/DialogBuilderHelper.hpp"
#include "include/Dialogs/FormValidatorToken.hpp"
#include "include/Gui.hpp"
#include "include/Interfaces/ToolInterface.hpp"
#include "include/Interfaces/ToolPropertyInterface.hpp"
#include "include/Utilities/Box.hpp"
#include <functional>

class EditPropertyDialog final
{
public:
    [[nodiscard]] EditPropertyDialog(GC<Gui> gui) noexcept : gui(gui) {}

public:
    void open(Box<ToolPropertyInterface> property, ToolInterface& targetTool);

    void close();

public:
    static inline constexpr const char* DIALOG_ID = "EditPropertyDialog";

private:
    void buildBottomButtonBar(
        tgui::ChildWindow::Ptr& modal, std::function<void(void)> submit);

    tgui::Button::Ptr createButton(
        const std::string& label,
        const tgui::Layout2d& size,
        const tgui::Layout2d& position,
        std::function<void(void)> callback);

private:
    GC<Gui> gui;
    FormValidatorToken formValidatorToken;
    Box<ToolPropertyInterface> currentProperty = Box<NullToolProperty2>();
};
