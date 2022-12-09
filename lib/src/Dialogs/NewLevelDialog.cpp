#include "include/Dialogs/NewLevelDialog.hpp"
#include "include/FileApi.hpp"
#include "include/LogConsole.hpp"
#include "include/Globals.hpp"

constexpr const char* INPUT_LEVEL_WIDTH_ID = "InputLevelWidth";
constexpr const char* INPUT_LEVEL_HEIGHT_ID = "InputLevelHeight";
constexpr const char* INPUT_LEVEL_CONFIG_ID = "InputLevelConfig";

void NewLevelDialog::customOpenCode()
{
	if (ini.hasSection("Editor") && ini["Editor"].hasKey("configPath"))
	{
		auto box = gui.get<tgui::EditBox>(INPUT_LEVEL_CONFIG_ID);
		box->setText(ini["Editor"]["configPath"].asString());
	}
}

unsigned NewLevelDialog::getLevelWidth() const
{
	return std::stoul(getEditboxValue(INPUT_LEVEL_WIDTH_ID));
}

unsigned NewLevelDialog::getLevelHeight() const
{
	return std::stoul(getEditboxValue(INPUT_LEVEL_HEIGHT_ID));
}

std::string NewLevelDialog::getConfigPath() const
{
	return getEditboxValue(INPUT_LEVEL_CONFIG_ID);
}

NewLevelDialog::NewLevelDialog(
	tgui::Gui& gui,
	tgui::Theme& theme,
	cfg::Ini& ini)
	:
	DialogInterface(
		gui,
		theme,
		"ModalNewLevel",
		"New Level",
		std::vector<OptionLine>({
			OptionLine{ "Level width:", INPUT_LEVEL_WIDTH_ID, "20" },
			OptionLine{ "Level height:", INPUT_LEVEL_HEIGHT_ID, "10" },
			OptionLine{ "Config file:", INPUT_LEVEL_CONFIG_ID, "Pick a config file", true, [&gui] ()
				{
					auto fileApi = std::make_unique<FileApi>();
					auto box = gui.get<tgui::EditBox>("InputLevelConfig");
					auto fileName = fileApi->getOpenFileName(
						"JSON Files\0*.json\0Any File\0*.*\0");
					box->setText(fileName.value_or(""));
 } } })),
	ini(ini)
{}
