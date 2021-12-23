#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include <format>

class Log {
private:
	tgui::Gui *gui = nullptr;

	Log() {}

public:
	static Log &get();

	void create(tgui::Theme& theme, const tgui::Layout2d& pos, const tgui::Layout2d& size);

	void toggle();

	static void write(const std::string& text);

	static void write(const std::string& label, const sf::Vector2i &vec);

	template<class... Args>
	static void write2(const char* fmt, Args... args) {
		write(std::format(fmt, std::forward<Args>(args)...));
	}

	void init(tgui::Gui *gui) {
		Log::gui = gui;
	}
};
