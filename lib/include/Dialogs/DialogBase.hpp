#pragma once

#include "include/Gui.hpp"
#include "include/Utilities/GC.hpp"
#include <functional>
#include <variant>
#include <vector>

struct OptionInput
{
    std::string label;
    std::string id;
    std::string value;
};

struct OptionDeferredInput
{
    std::string label;
    std::string id;
    std::function<std::string()> value;
};

struct OptionInputWithButton
{
    OptionInput base;
    std::function<void(void)> buttonCallback = [] {};
};

struct OptionDeferredInputWithButton
{
    OptionDeferredInput base;
    std::function<void(void)> buttonCallback = [] {};
};

struct OptionText
{
    std::string text;
    unsigned rowsToAllocate;
};

using OptionLine = std::variant<
    OptionInput,
    OptionDeferredInput,
    OptionInputWithButton,
    OptionDeferredInputWithButton,
    OptionText>;

class DialogInterface
{
protected:
    GC<Gui> gui;
    const std::string DIALOG_ID;
    const std::string DIALOG_TITLE;
    const std::vector<OptionLine> OPTIONS;

protected:
    virtual void customOpenCode() = 0;

    [[nodiscard]] std::string getEditboxValue(const std::string& boxId) const
    {
        return gui->get<tgui::EditBox>(boxId)->getText().toAnsiString();
    }

public:
    void open(std::function<void()> confirmCallback);

    void close()
    {
        gui->closeModal(DIALOG_ID);
    }

    [[nodiscard]] bool isOpen() const
    {
        return gui->get<tgui::ChildWindow>(DIALOG_ID) != nullptr;
    }

    DialogInterface(
        GC<Gui> gui,
        const std::string& dialogId,
        const std::string& dialogTitle,
        const std::vector<OptionLine>& options);
    DialogInterface(const DialogInterface&) = delete;
    DialogInterface(DialogInterface&&) = delete;
    virtual ~DialogInterface() = default;
};
