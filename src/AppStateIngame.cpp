#include "AppStateIngame.hpp"

void AppStateIngame::input() {
	sf::Event event;
	while (app->window.pollEvent(event)) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				app->popState();
			}
		}
	}
}

void AppStateIngame::update() {
}

void AppStateIngame::draw() {
	app->window.beginDraw();

	app->window.endDraw();
}

bool AppStateIngame::init() {
	return true;
}
