#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include <functional>

class GenericSettingsDialog {
protected:
	tgui::Gui& gui;

	struct OptionLine {
		std::string label;
		std::string width;
		std::string id;
		std::string value;
	};

	virtual std::string getDialogId() const = 0;
	virtual std::vector<OptionLine> getLines() const = 0;
	virtual void customOpenCode() = 0;
	virtual std::string getDialogTitle() const = 0;

public:
	void open(std::function<void()> confirmCallback);

	void close() {
		auto modal = gui.get<tgui::ChildWindow>(getDialogId());
		gui.remove(modal);
	}

	bool isOpen() const {
		return gui.get<tgui::ChildWindow>(getDialogId()) != nullptr;
	}

	GenericSettingsDialog(tgui::Gui& gui) : gui(gui) {}
};

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
class NewLevelDialog : public GenericSettingsDialog {
private:
	const std::string INPUT_LEVEL_WIDTH_ID = "InputLevelWidth";
	const std::string INPUT_LEVEL_HEIGHT_ID = "InputLevelHeight";
	const std::string INPUT_LEVEL_CONFIG_ID = "InputLevelConfig";

	cfg::Ini& ini;

	virtual std::string getDialogId() const {
		return "ModalNewLevel";
	}
	virtual std::string getDialogTitle() const override {
		return "New Level";
	}

	virtual std::vector<OptionLine> getLines() const;
	virtual void customOpenCode();

public:
	unsigned getLevelWidth() const {
		return std::stoul(gui.get<tgui::EditBox>(INPUT_LEVEL_WIDTH_ID)->getText().toAnsiString());
	}
	unsigned getLevelHeight() const {
		return std::stoul(gui.get<tgui::EditBox>(INPUT_LEVEL_HEIGHT_ID)->getText().toAnsiString());
	}
	std::string getConfigPath() const {
		return gui.get<tgui::EditBox>(INPUT_LEVEL_CONFIG_ID)->getText().toAnsiString();
	}

	NewLevelDialog(tgui::Gui& gui, cfg::Ini &ini) : GenericSettingsDialog(gui), ini(ini) {}
};

class ResizeDialog : public GenericSettingsDialog {
protected:
	const std::string INPUT_LEVEL_WIDTH_ID = "InputLevelWidth";
	const std::string INPUT_LEVEL_HEIGHT_ID = "InputLevelHeight";

	virtual std::string getDialogId() const;
	virtual std::string getDialogTitle() const override {
		return "Resize Level";
	}
	virtual std::vector<OptionLine> getLines() const;
	virtual void customOpenCode();

public:
	unsigned getLevelWidth() const {
		return std::stoul(gui.get<tgui::EditBox>(INPUT_LEVEL_WIDTH_ID)->getText().toAnsiString());
	}
	unsigned getLevelHeight() const {
		return std::stoul(gui.get<tgui::EditBox>(INPUT_LEVEL_HEIGHT_ID)->getText().toAnsiString());
	}

	ResizeDialog(tgui::Gui& gui) : GenericSettingsDialog(gui) {}
};
