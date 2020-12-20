#include "NewLevelDialog.hpp"
#include "FileApi.hpp"

void NewLevelDialog::open(std::function<void()> confirmCallback) {
	auto modal = tgui::ChildWindow::create("New level");
	modal->setSize("30%", "50%");
	modal->setPosition("35%", "25%");
	gui.add(modal, "ModalNewLevel");

	const auto ROW_HEIGHT = "6%";
	const auto ROW_Y_OFFSET = 2;

	// Labels
	std::vector<std::string> labels = {
		"Level width:",
		"Level height:",
		"Config file:"
	};

	for (unsigned i = 0; i < labels.size(); i++) {
		auto label = tgui::Label::create(labels[i]);
		label->setSize("26%", ROW_HEIGHT);
		label->setPosition("2%", std::to_string(i * 10 + ROW_Y_OFFSET) + "%");
		label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		modal->add(label);
	}

	// Textboxes
	std::vector<std::vector<std::string>> boxes = {
		{"66%", "20", "InputLevelWidth"},
		{"66%", "10", "InputLevelHeight"},
		{"58%", "Pick a config file", "InputLevelConfig"},
	};

	for (unsigned i = 0; i < boxes.size(); i++) {
		auto box = tgui::EditBox::create();
		box->setSize(boxes[i][0], ROW_HEIGHT);
		box->setPosition("32%", std::to_string(i * 10 + ROW_Y_OFFSET) + "%");
		box->setText(boxes[i][1]);
		modal->add(box, boxes[i][2]);
	}

	// ## Buttons
	// Selection of config file
	auto btn = tgui::Button::create("...");
	btn->setSize("8%", "6%");
	btn->setPosition("90%", "22%");
	btn->connect("clicked", [this]() {
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
	btn->connect("clicked", [this, confirmCallback]() {
		confirmCallback();
		close();
	});
	modal->add(btn);

	btn = tgui::Button::create("Cancel");
	btn->setSize("20%", "8%");
	btn->setPosition("78%", "90%");
	btn->connect("clicked", [this]() {
		close();
	});
	modal->add(btn);
}

void NewLevelDialog::close() {
	auto modal = gui.get<tgui::ChildWindow>("ModalNewLevel");
	modal->close();
}