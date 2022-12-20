#include "include/Tools/ToolProperty.hpp"
#include "include/Tools/ToolInterface.hpp"

tgui::Panel::Ptr ToolProperty::getRowBackground(unsigned y, const std::string& tooltip)
{
	auto tt = tgui::Label::create(tooltip);
	tt->getRenderer()->setBackgroundColor(sf::Color::White);
	tt->getRenderer()->setBorders(tgui::Borders::Outline(1));
	tt->getRenderer()->setBorderColor(sf::Color::Black);

	auto row = tgui::Panel::create();
	row->setSize("100%", ROW_HEIGHT);
	row->setPosition({ "0%", y * ROW_HEIGHT });
	row->setToolTip(tt);
	row->getRenderer()->setBackgroundColor(y % 2 == 0 ? ROW_BGR_DARK : ROW_BGR_LIGHT);
	return row;
}

tgui::Label::Ptr ToolProperty::getLabel(const std::string& label)
{
	auto l = tgui::Label::create(label);
	l->setSize(LABEL_WIDTH, "100%");
	l->setPosition(LABEL_LEFT_MARGIN, "0%");
	l->setTextSize(LABEL_FONT_SIZE);
	return l;
}

void ToolProperty::addOption(TargetPanel& target, const std::string& label, const std::string& tooltip, bool& val, unsigned ypos, bool enabled)
{
	auto row = getRowBackground(ypos, tooltip);
	target->add(row);

	auto lbl = getLabel(label);
	row->add(lbl);

	auto checkbox = tgui::CheckBox::create();
	checkbox->setSize(row->getSize().y, row->getSize().y);
	checkbox->setPosition(VALUE_LEFT_MARGIN, "0%");
	checkbox->setChecked(val);
	checkbox->setEnabled(enabled);
	checkbox->connect("Changed", [&val] (bool newVal) { val = newVal; });

	row->add(checkbox);
}


template<typename T>
inline void ToolProperty::addOptionUint(tgui::Gui& gui, TargetPanel& target, const std::string& label, const std::string& tooltip, T& val, unsigned ypos, bool enabled, bool tag)
{
	auto row = getRowBackground(ypos, tooltip);
	target->add(row);

	auto lbl = getLabel(label);
	row->add(lbl);

	auto edit = tgui::EditBox::create();
	edit->setSize(VALUE_WIDTH, "100%");
	edit->setPosition(VALUE_LEFT_MARGIN, "0%");
	edit->setText(std::to_string(val));
	edit->setEnabled(enabled);
	edit->getRenderer()->setBorderColor(sf::Color::Black);
	edit->setInputValidator(tgui::EditBox::Validator::UInt);

	if (enabled)
	{
		edit->connect("TextChanged", [&, label] (const std::string& newVal)
 {
	 auto edit = gui.get<tgui::EditBox>("EditBox" + label);
		try
		{
			std::size_t endpos;
			unsigned long value = std::stoul(newVal, &endpos);
			if (value > T(-1)) throw 1;
			val = T(value);
			edit->getRenderer()->setBorderColor(sf::Color::Black);
			formValid = true;
		}
		catch (...)
		{
			edit->getRenderer()->setBorderColor(sf::Color::Red);
			formValid = false;
		}
		});
	}

	if (tag)
	{
		auto btn = tgui::Button::create("New tag");
		btn->setSize(VALUE_WIDTH, "100%");
		btn->setPosition(TAG_LEFT_MARGIN, "0%");
		btn->setEnabled(enabled);
		btn->connect("pressed", [&, label]
 {
	 auto edit = gui.get<tgui::EditBox>("EditBox" + label);
		edit->setText(std::to_string(PropertyTag::get().getNewTag()));
		});
		row->add(btn);
	}

	row->add(edit, "EditBox" + label);
}


void ToolProperty::addOption(tgui::Gui& gui, TargetPanel& target, const std::string& label, const std::string& tooltip, uint32_t& val, unsigned ypos, bool enabled, bool tag)
{
	addOptionUint(gui, target, label, tooltip, val, ypos, enabled, tag);
}

void ToolProperty::addOption(tgui::Gui& gui, TargetPanel& target, const std::string& label, const std::string& tooltip, uint16_t& val, unsigned ypos, bool enabled)
{
	addOptionUint(gui, target, label, tooltip, val, ypos, enabled, false);
}

void ToolProperty::addOption(TargetPanel& target, const std::string& label, const std::string& tooltip, std::string& val, unsigned ypos, bool enabled)
{
	auto row = getRowBackground(ypos, tooltip);
	target->add(row);

	auto lbl = getLabel(label);
	row->add(lbl);

	auto edit = tgui::EditBox::create();
	edit->setSize(VALUE_WIDTH, "100%");
	edit->setPosition(VALUE_LEFT_MARGIN, "0%");
	edit->setText(val);
	edit->setMaximumCharacters(255);
	edit->setEnabled(enabled);
	edit->getRenderer()->setBorderColor(sf::Color::Black);

	if (enabled)
	{
		edit->connect("TextChanged", [this, &val, label] (const std::string& newVal) { val = newVal; });
	}

	row->add(edit, "EditBox" + label);
}

void ToolProperty::buildModal(
	tgui::Gui& gui,
	tgui::Theme& theme,
	ToolInterface& submitTarget)
{
	// Create wrapper window
	auto modal = tgui::ChildWindow::create("Tile Properties");
	modal->setRenderer(theme.getRenderer("ChildWindow"));
	modal->setSize("50%", "80%");
	modal->setPosition("25%", "10%");
	gui.add(modal, "ToolPropertyModal");

	// Create scrollable group inside of this window
	auto group = tgui::Panel::create();
	group->setSize("100%", "88%");
	modal->add(group);

	// Actual content
	buildModalSpecifics(gui, group);

	// Bottom buttons
	auto close = [] (tgui::Gui& gui)
	{
		auto modal = gui.get<tgui::ChildWindow>("ToolPropertyModal");
		modal->close();
		gui.remove(modal);
	};

	// TODO: Sizers
	auto btn = tgui::Button::create("Ok");
	btn->setSize("20%", "8%");
	btn->setPosition("56%", "90%");
	btn->connect("clicked", [&]
 {
	 if (!formValid) return;
	submitTarget.setProperty(*this);
	close(gui);
	});
	modal->add(btn);

	btn = tgui::Button::create("Cancel");
	btn->setSize("20%", "8%");
	btn->setPosition("78%", "90%");
	btn->connect("clicked", [&] { close(gui); });
	modal->add(btn);

	formValid = true;
}

void ImageToolProperty::buildModalSpecifics(tgui::Gui& gui, tgui::Panel::Ptr& panel)
{
	const float SCROLLBAR_WIDTH = 20.f;

	const float IMAGE_SIZE = panel->getSize().x / 4.f;
	const unsigned VERTICAL_OFFSET = 20;

	// Display image with preview
	auto imagePanel = tgui::Panel::create();
	imagePanel->setSize(IMAGE_SIZE, IMAGE_SIZE);
	imagePanel->setPosition((panel->getSize().x - IMAGE_SIZE) / 2.f, VERTICAL_OFFSET);
	imagePanel->getRenderer()->setTextureBackground(imageTexture);
	panel->add(imagePanel);

	auto sp = tgui::ScrollablePanel::create();
	sp->getRenderer()->setScrollbarWidth(SCROLLBAR_WIDTH);
	sp->setPosition("0%", IMAGE_SIZE + 2 * VERTICAL_OFFSET);
	sp->setSize("100%", panel->getSize().y - (IMAGE_SIZE + 2 * VERTICAL_OFFSET));
	panel->add(sp);

	// Do property specific stuff
	buildModalSpecifics(gui, sp);
}
