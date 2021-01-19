#include "ToolProperty.hpp"
#include "Tool.hpp"

void ToolProperty::addLabel(tgui::ScrollablePanel::Ptr& target, const std::string& text, unsigned y) {
	auto label = tgui::Label::create(text);
	label->setTextSize(LABEL_FONT_SIZE);
	label->setSize(LABEL_WIDTH, ROW_HEIGHT);
	label->setPosition(LABEL_LEFT_MARGIN, y);
	target->add(label);
}

void ToolProperty::addBoolEdit(tgui::ScrollablePanel::Ptr& target, const std::string& label, unsigned y, bool& var, bool enabled) {
	addLabel(target, label, y);

	auto checkbox = tgui::CheckBox::create();
	checkbox->setSize(ROW_HEIGHT, ROW_HEIGHT);
	checkbox->setPosition(EDIT_LEFT_MARGIN, y);
	checkbox->setChecked(var);
	checkbox->setEnabled(enabled);
	checkbox->connect("Changed", [&var]() { var = !var; });

	target->add(checkbox);
}

void ToolProperty::addIntEdit(tgui::ScrollablePanel::Ptr& target, const std::string& label, unsigned y, uint32_t& var, bool enabled) {
	addLabel(target, label, y);

	auto edit = tgui::EditBox::create();
	edit->setSize(EDIT_WIDTH, ROW_HEIGHT);
	edit->setPosition(EDIT_LEFT_MARGIN, y);
	edit->setEnabled(enabled);
	edit->setText(std::to_string(var));
	edit->getRenderer()->setBorderColor(sf::Color::Black);
	edit->connect("TextChanged", [this, label, &var] () {
		auto edit = gui.get<tgui::EditBox>("EditBox" + label);
		unsigned long num = 0;
		try {
			num = std::stoul(std::string(edit->getText()));
			if (num > uint32_t(-1)) throw 1;
			edit->getRenderer()->setBorderColor(sf::Color::Black);
			var = num;
		}
		catch (...) { edit->getRenderer()->setBorderColor(sf::Color::Red); }
	});
	target->add(edit, "EditBox" + label);
}

void ToolProperty::buildModal() {
	const float SCROLLBAR_WIDTH = 20.f;

	// Create wrapper window
	auto modal = tgui::ChildWindow::create("Tile Properties");
	modal->setSize("50%", "80%");
	modal->setPosition("25%", "10%");
	gui.add(modal, "ToolPropertyModal");

	// Create scrollable group inside of this window
	auto group = tgui::ScrollablePanel::create();
	group->getRenderer()->setScrollbarWidth(SCROLLBAR_WIDTH);
	group->setSize("100%", "88%");
	modal->add(group);

	// Actual content
	buildModalSpecifics(group);

	// Bottom buttons
	auto close = [this]() {
		auto modal = gui.get<tgui::ChildWindow>("ToolPropertyModal");
		modal->close();
	};

	auto btn = tgui::Button::create("Ok");
	btn->setSize("20%", "8%");
	btn->setPosition("56%", "90%");
	btn->connect("clicked", [this, close]() {
		parent->setProperty(*this);
		close();
	});
	modal->add(btn);

	btn = tgui::Button::create("Cancel");
	btn->setSize("20%", "8%");
	btn->setPosition("78%", "90%");
	btn->connect("clicked", [this, close]() { close();  });
	modal->add(btn);
}

void ImageToolProperty::buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) {
	const float IMAGE_SIZE = panel->getSize().x / 4.f;
	const unsigned VERTICAL_OFFSET = 20;

	// Display image with preview
	auto imagePanel = tgui::Panel::create();
	imagePanel->setSize(IMAGE_SIZE, IMAGE_SIZE);
	imagePanel->setPosition((panel->getSize().x - IMAGE_SIZE) / 2.f, VERTICAL_OFFSET);
	imagePanel->getRenderer()->setTextureBackground(imageTexture);
	panel->add(imagePanel);

	// Do property specific stuff
	buildModalSpecifics(panel, VERTICAL_OFFSET, IMAGE_SIZE + 2 * VERTICAL_OFFSET);
}
