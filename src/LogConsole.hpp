#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>

class Log {
private:
	tgui::Gui *gui = nullptr;
	static Log *instance;

	Log() {}

public:
	static Log &get();

	void create(tgui::Theme& theme, const sf::Vector2f& position);

	void toggle();

	static void write(const std::string& text);

	void init(tgui::Gui *gui) {
		Log::gui = gui;
	}
};