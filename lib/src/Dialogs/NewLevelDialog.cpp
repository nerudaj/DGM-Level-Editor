#include "include/Dialogs/NewLevelDialog.hpp"
#include "include/FileApi.hpp"
#include "include/LogConsole.hpp"
#include "include/Globals.hpp"

constexpr const char* INPUT_LEVEL_WIDTH_ID = "InputLevelWidth";
constexpr const char* INPUT_LEVEL_HEIGHT_ID = "InputLevelHeight";
constexpr const char* INPUT_LEVEL_CONFIG_ID = "InputLevelConfig";

void NewLevelDialog::customOpenCode() {
	if (ini.hasSection("Editor") && ini["Editor"].hasKey("configPath")) {
		auto box = gui.get<tgui::EditBox>(INPUT_LEVEL_CONFIG_ID);
		box->setText(ini["Editor"]["configPath"].asString());
	}
}

unsigned NewLevelDialog::getLevelWidth() const {
	return std::stoul(getEditboxValue(INPUT_LEVEL_WIDTH_ID));
}

unsigned NewLevelDialog::getLevelHeight() const {
	return std::stoul(getEditboxValue(INPUT_LEVEL_HEIGHT_ID));
}

std::string NewLevelDialog::getConfigPath() const {
	return getEditboxValue(INPUT_LEVEL_CONFIG_ID);
}

NewLevelDialog::NewLevelDialog(
	tgui::Gui& gui, 
	cfg::Ini& ini) 
:
	DialogInterface(
		gui,
		"ModalNewLevel",
		"New Level",
		std::vector<OptionLine>({
			{ "Level width:", "66%", INPUT_LEVEL_WIDTH_ID, "20" },
			{ "Level height:", "66%", INPUT_LEVEL_HEIGHT_ID, "10" },
			{ "Config file:", "58%", INPUT_LEVEL_CONFIG_ID, "Pick a config file" }})),
	ini(ini)
{}
