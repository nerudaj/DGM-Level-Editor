#include "include/Dialogs/UpdateConfigPathDialog.hpp"

constexpr const char* INPUT_UPDATED_CONFIG_ID = "InputUpdatedConfig";

std::string UpdateConfigPathDialog::getConfigPath() const
{
    return getEditboxValue(INPUT_UPDATED_CONFIG_ID);
}

void UpdateConfigPathDialog::customOpenCode() {}

UpdateConfigPathDialog::UpdateConfigPathDialog(
    GC<Gui> gui, GC<FileApiInterface> fileApi)
    : DialogInterface(
        gui,
        "ModalUpdateConfigPath",
        "Update path to config",
        std::vector<OptionLine> { OptionInputWithButton {
            "Config file:",
            INPUT_UPDATED_CONFIG_ID,
            "Pick a config file",
            [&, fileApi]
            {
                auto box = gui->get<tgui::EditBox>(INPUT_UPDATED_CONFIG_ID);
                auto filename = fileApi->getOpenFileName(JSON_FILTER);
                box->setText(filename.value_or(""));
            } } })
{
}
