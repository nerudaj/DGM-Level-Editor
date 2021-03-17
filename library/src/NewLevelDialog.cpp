#include "NewLevelDialog.hpp"
#include "FileApi.hpp"
#include "LogConsole.hpp"

void GenericSettingsDialog::open(std::function<void()> confirmCallback) {
	if (isOpen()) return;

	auto modal = tgui::ChildWindow::create("New level");
	modal->setSize("30%", "50%");
	modal->setPosition("35%", "25%");
	modal->connect("EscapeKeyPressed", [this] () { close(); });
	modal->connect("Closed", [this] () { close(); });
	gui.add(modal, getDialogId());

	const auto ROW_HEIGHT = "6%";
	const auto ROW_Y_OFFSET = 2;

	const auto lines = getLines();

	for (unsigned i = 0; i < lines.size(); i++) {
		auto label = tgui::Label::create(lines[i].label);
		label->setSize("26%", ROW_HEIGHT);
		label->setPosition("2%", std::to_string(i * 10 + ROW_Y_OFFSET) + "%");
		label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		modal->add(label);
	}

	for (unsigned i = 0; i < lines.size(); i++) {
		auto box = tgui::EditBox::create();
		box->setSize(lines[i].width, ROW_HEIGHT);
		box->setPosition("32%", std::to_string(i * 10 + ROW_Y_OFFSET) + "%");
		box->setText(lines[i].value);
		modal->add(box, lines[i].id);
	}

	// ## Buttons
	// Selection of config file
	auto btn = tgui::Button::create("...");
	btn->setSize("8%", "6%");
	btn->setPosition("90%", "22%");
	btn->connect("clicked", [this] () {
		try {
			auto str = FileApi::getOpenFileName("JSON Files\0*.json\0Any File\0*.*\0");
			auto box = gui.get<tgui::EditBox>("InputLevelConfig");
			box->setText(str);
		}
		catch (...) {}
		});
	modal->add(btn);

	btn = tgui::Button::create("Ok");
	btn->setSize("20%", "8%");
	btn->setPosition("56%", "90%");
	btn->connect("clicked", [this, confirmCallback] () {
		confirmCallback();
		close();
		});
	modal->add(btn);

	btn = tgui::Button::create("Cancel");
	btn->setSize("20%", "8%");
	btn->setPosition("78%", "90%");
	btn->connect("clicked", [this] () {
		close();
		});
	modal->add(btn);

	customOpenCode();
}

std::vector<GenericSettingsDialog::OptionLine> NewLevelDialog::getLines() const {
	return {
		{ "Level width:", "66%", INPUT_LEVEL_WIDTH_ID, "20" },
		{ "Level height:", "66%", INPUT_LEVEL_HEIGHT_ID, "10" },
		{ "Config file:", "58%", INPUT_LEVEL_CONFIG_ID, "Pick a config file" }
	};
}

void NewLevelDialog::customOpenCode() {
	if (ini.hasSection("Editor") && ini["Editor"].hasKey("configPath")) {
		auto box = gui.get<tgui::EditBox>(INPUT_LEVEL_CONFIG_ID);
		box->setText(ini["Editor"]["configPath"].asString());
	}
}

std::string ResizeDialog::getDialogId() const {
	return "ResizeLevelDialog";
}

std::vector<GenericSettingsDialog::OptionLine> ResizeDialog::getLines() const {
	return {
		{ "Level width:", "66%", INPUT_LEVEL_WIDTH_ID, "20" },
		{ "Level height:", "66%", INPUT_LEVEL_HEIGHT_ID, "10" }
	};
}

void ResizeDialog::customOpenCode() {
}
