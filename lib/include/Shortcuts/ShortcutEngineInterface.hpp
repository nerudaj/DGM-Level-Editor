#pragma once

#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "include/Shortcuts/ShortcutCombo.hpp"

class ShortcutEngineInterface
{
public:
	virtual void handleEvent(sf::Event& event) = 0;

	virtual void registerShortcut(
		const std::string& groupName,
		const ShortcutCombo& combo,
		std::function<void(void)> callback) = 0;

	// Removes all shortcuts registered with matching group name
	virtual void unregisterShortcutGroup(
		const std::string& groupName) = 0;

	virtual ~ShortcutEngineInterface() = default;
};
