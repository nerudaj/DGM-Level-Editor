#include "include/Dialogs/PlaytestSettingsDialog.hpp"

constexpr const char* INPUT_BINARY_PATH = "InputBinaryPath";
constexpr const char* INPUT_LAUNCH_OPTIONS = "InputLaunchOptions";

PlaytestSettingsDialog::PlaytestSettingsDialog(
    GC<Gui> gui,
    GC<FileApiInterface> fileApi,
    const std::filesystem::path& binaryPath,
    const std::string& launchOptions)
    : PlaytestSettingsDialogInterface(
        gui,
        "PlaytestSettingsDialog",
        "Playtest Settings",
        std::vector<OptionLine> {
            OptionInputWithButton {
                "Binary to launch:",
                INPUT_BINARY_PATH,
                binaryPath.string(),
                [&, fileApi]
                {
                    auto box = gui->gui.get<tgui::EditBox>(INPUT_BINARY_PATH);
                    auto fileName = fileApi->getOpenFileName(EXES_FILTER);
                    box->setText(fileName.value_or(""));
                } },
            OptionText { "Use variable $(LevelPath) to pass currently "
                         "edited level in command-line parameters",
                         2 },
            OptionInput {
                "Cmd parameters:", INPUT_LAUNCH_OPTIONS, launchOptions } })
{
}

std::filesystem::path PlaytestSettingsDialog::getBinaryPath() const
{
    return getEditboxValue(INPUT_BINARY_PATH);
}

std::string PlaytestSettingsDialog::getLaunchOptions() const
{
    return getEditboxValue(INPUT_LAUNCH_OPTIONS);
}
