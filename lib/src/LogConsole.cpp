#include "include/LogConsole.hpp"

Log &Log::get() {
	static Log instance;
	return instance;
}

void Log::create(tgui::Theme &theme, const tgui::Layout2d &pos, const tgui::Layout2d& size) {
	auto child = tgui::ChildWindow::create();
	child->setRenderer(theme.getRenderer("ChildWindow"));
	child->setResizable(true);
	child->getRenderer()->setTitleBarHeight(gui->getView().getSize().y * 0.03f);
	child->setSize(size);
	child->setPosition(pos);
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
	console->setTextSize(unsigned(gui->getView().getSize().y * 0.015f));
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
