#pragma once

#include <TGUI/TGUI.hpp>
#include <functional>

/**
 *  \brief Modal window for creating new level
 *
 *  User should be able to define initial level width, height
 *  and he is also required to provide path to config file.
 *
 *  Programmer first has to open the modal window. If the selection is
 *  made and modal is confirmed, associated callback is fired. Then the
 *  getter functions can be used. Modal is closed automatically.
 */
class NewLevelDialog {
private:
	const std::string INPUT_LEVEL_WIDTH_ID = "InputLevelWidth";
	const std::string INPUT_LEVEL_HEIGHT_ID = "InputLevelHeight";
	const std::string INPUT_LEVEL_CONFIG_ID = "InputLevelConfig";

	tgui::Gui& gui;

public:
	void open(std::function<void()> confirmCallback);

	void close();

	unsigned getLevelWidth() const {
		return std::stoul(gui.get<tgui::EditBox>(INPUT_LEVEL_WIDTH_ID)->getText().toAnsiString());
	}
	unsigned getLevelHeight() const {
		return std::stoul(gui.get<tgui::EditBox>(INPUT_LEVEL_HEIGHT_ID)->getText().toAnsiString());
	}
	std::string getConfigPath() const {
		return gui.get<tgui::EditBox>(INPUT_LEVEL_CONFIG_ID)->getText().toAnsiString();
	}

	NewLevelDialog(tgui::Gui& gui) : gui(gui) {}
};