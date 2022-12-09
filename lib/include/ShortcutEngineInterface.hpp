#pragma once

#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

class ShortcutEngineInterface
{
public:
	virtual void handleEvent(sf::Event& event) = 0;

	virtual void registerShortcut(
		bool requiresCtrl,
		bool requiresShift,
		sf::Keyboard::Key key,
		std::function<void(void)> callback) = 0;

	virtual ~ShortcutEngineInterface() = default;
};
