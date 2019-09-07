#include "AppStateMenuOptions.hpp"

static const std::vector<std::pair<sf::Vector2u, std::string>> RESOLUTIONS = {
	{{640, 480}, "640x480"},
	{{800, 600}, "800x600"},
	{{1024, 768}, "1024x768"},
	{{1280, 720}, "1280x720"},
	{{1366, 768}, "1366x768"},
	{{1600, 900}, "1600x900"},
	{{1920, 1080}, "1920x1080"}
};

std::string getWindowResolutionAsString(const dgm::Window& window) {
	auto size = window.getSize();
	return std::to_string(size.x) + "x" + std::to_string(size.y);
}

void AppStateMenuOptions::createOptionLabel(tgui::Layout x, tgui::Layout y, const std::string& text) {
	tgui::Label::Ptr label = tgui::Label::create(text);
	label->setAutoSize(true);
	label->setSize("30%", "5%");
	label->setPosition(x, y);
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	gui.add(label);
}

void AppStateMenuOptions::buildLayout() {
	tgui::Label::Ptr title = tgui::Label::create("Options");
	title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	title->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	title->setTextSize(72);
	title->setSize("100%", "25%");
	title->setPosition("0%", "5%");
	gui.add(title);

	// Fullscreen toggle box
	tgui::CheckBox::Ptr fullscreenCheckbox = tgui::CheckBox::create();
	fullscreenCheckbox->setSize("5%", "5%");
	fullscreenCheckbox->setPosition("60%", "35%");
	fullscreenCheckbox->setChecked(app->window.isFullscreen());
	gui.add(fullscreenCheckbox, "CheckboxFullscreen");
	fullscreenCheckbox->connect("checked", [this]() {
		app->window.toggleFullscreen();
	});
	fullscreenCheckbox->connect("unchecked", [this]() {
		app->window.toggleFullscreen();
	});
	createOptionLabel("30%", "35%", "Toggle fullscreen");

	// Slider for adjusting sound volume
	tgui::Slider::Ptr sliderSoundVolume = tgui::Slider::create(0.f, 100.f);
	sliderSoundVolume->setSize("15%", "3%");
	sliderSoundVolume->setPosition("60%", "43%");
	sliderSoundVolume->setStep(1.f);
	sliderSoundVolume->setValue(settings.soundVolume);
	sliderSoundVolume->connect("ValueChanged", [this]() {
		settings.soundVolume = gui.get<tgui::Slider>("SliderSoundVolume")->getValue();
	});
	gui.add(sliderSoundVolume, "SliderSoundVolume");
	createOptionLabel("30%", "42%", "Sound volume");

	// Slider for adjusting music volume
	tgui::Slider::Ptr sliderMusicVolume = tgui::Slider::create(0.f, 100.f);
	sliderMusicVolume->setSize("15%", "3%");
	sliderMusicVolume->setPosition("60%", "49%");
	sliderMusicVolume->setStep(1.f);
	sliderMusicVolume->setValue(settings.musicVolume);
	sliderMusicVolume->connect("ValueChanged", [this]() {
		settings.musicVolume = gui.get<tgui::Slider>("SliderMusicVolume")->getValue();
	});
	gui.add(sliderMusicVolume, "SliderMusicVolume");
	createOptionLabel("30%", "48%", "Music volume");

	// Dropdown for selecting resolution
	tgui::ComboBox::Ptr comboResolution = tgui::ComboBox::create();
	comboResolution->setSize("15%", "5%");
	comboResolution->setPosition("60%", "55%");
	for (auto& res : RESOLUTIONS) {
		comboResolution->addItem(res.second, res.second);
	}
	comboResolution->setSelectedItem(getWindowResolutionAsString(app->window));
	comboResolution->connect("ItemSelected", [this]() {
		auto item = gui.get<tgui::ComboBox>("ComboResolution");
		auto index = item->getSelectedItemIndex();
		if (index == -1) return;
		bool fs = app->window.isFullscreen();
		//auto title = app->window.getTitle();

		// Restart window with new resolution
		app->window.close();
		app->window.open(RESOLUTIONS[index].first, "null", fs);

		// Force gui to update viewport and resolution
		gui.setView(app->window.getWindowContext().getView());
	});
	gui.add(comboResolution, "ComboResolution");
	createOptionLabel("30%", "54%", "Set resolution");

	tgui::Button::Ptr backButton = tgui::Button::create("Back");
	backButton->setSize("15%", "5%");
	backButton->setPosition("84%", "94%");
	backButton->connect("pressed", [this]() { app->popState(); });
	gui.add(backButton, "ButtonExit");
}

void AppStateMenuOptions::input() {
	sf::Event event;
	while (app->window.pollEvent(event)) {
		gui.handleEvent(event);
	}
}

void AppStateMenuOptions::update() {}

void AppStateMenuOptions::draw() {
	app->window.beginDraw(sf::Color::White);

	gui.draw();

	app->window.endDraw();
}

bool AppStateMenuOptions::init() {
	sf::Font &font = resmgr.get<sf::Font>("cruft.ttf");

	gui.setFont(font);
	gui.setTarget(app->window.getWindowContext());

	buildLayout();

	return true;
}
