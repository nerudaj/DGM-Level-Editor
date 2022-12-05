#include "include/Dialogs/ResizeLevelDialog.hpp"

constexpr const char* INPUT_LEVEL_WIDTH_ID = "InputLevelWidth";
constexpr const char* INPUT_LEVEL_HEIGHT_ID = "InputLevelHeight";

unsigned ResizeDialog::getLevelWidth() const
{
	return std::stoul(getEditboxValue(INPUT_LEVEL_WIDTH_ID));
}

unsigned ResizeDialog::getLevelHeight() const
{
	return std::stoul(getEditboxValue(INPUT_LEVEL_HEIGHT_ID));
}

ResizeDialog::ResizeDialog(tgui::Gui& gui, tgui::Theme& theme)
	:
	DialogInterface(
		gui,
		theme,
		"ResizeLevelDialog",
		"Resize Level",
		std::vector<OptionLine>({
			{ "Level width:", INPUT_LEVEL_WIDTH_ID, "20" },
			{ "Level height:", INPUT_LEVEL_HEIGHT_ID, "10" } }))
{}
