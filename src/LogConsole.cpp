#include "LogConsole.hpp"

Log *Log::instance = nullptr;

Log &Log::get() {
	if (!instance) instance = new Log();

	return *instance;
}

void Log::create(tgui::Theme &theme, const sf::Vector2f &position) {
	auto child = tgui::ChildWindow::create();
	child->setRenderer(theme.getRenderer("ChildWindow"));
	child->setResizable(true);
	child->setSize(400.f, 200.f);
	child->setPosition(position);
	child->setTitle("Console");
	child->setTitleButtons(tgui::ChildWindow::TitleButton::Close);
	child->connect("Closed", [this]() {
		auto child = gui->get<tgui::ChildWindow>("ConsoleWindow");
		child->setVisible(false);
		});
	gui->add(child, "ConsoleWindow");

	auto console = tgui::ChatBox::create();
	console->setRenderer(theme.getRenderer("ChatBox"));
	console->setSize("100%", "100%");
	console->setTextSize(14);
	console->setLinesStartFromTop();
	console->addLine("This is a log console");
	child->add(console, "LogConsole");
}

void Log::toggle() {
	auto child = gui->get<tgui::ChildWindow>("ConsoleWindow");
	child->setVisible(!child->isVisible());
}

void Log::write(const std::string& message) {
	auto &ctx = get();
	auto logger = ctx.gui->get<tgui::ChatBox>("LogConsole");
	logger->addLine(message);
}

void Log::write(const std::string& label, const sf::Vector2i& vec) {
	write(label + ": [" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + "]");
}
