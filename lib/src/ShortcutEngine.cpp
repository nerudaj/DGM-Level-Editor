#include "include/ShortcutEngine.hpp"
#include "include/LogConsole.hpp"
#include <cassert>

void ShortcutEngine::evaluateShortcut(sf::Keyboard::Key key)
{
	if (!ctrlIsPressed && !shiftIsPressed)
		return;

	ShortcutCombo combo = {
		.ctrlRequired = ctrlIsPressed,
		.shiftRequired = shiftIsPressed,
		.key = key
	};

	if (shortcuts.contains(combo))
		shortcuts.at(combo)();
	else
		Log::write2("Unknown shortcut: {}", std::to_string(combo));
}

void ShortcutEngine::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::LControl)
			ctrlIsPressed = true;
		else if (event.key.code == sf::Keyboard::LShift)
			shiftIsPressed = true;
		else
			evaluateShortcut(event.key.code);
	}
	else if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::LControl)
			ctrlIsPressed = false;
		else if (event.key.code == sf::Keyboard::LShift)
			shiftIsPressed = false;
	}
}

void ShortcutEngine::registerShortcut(
		bool requiresCtrl,
		bool requiresShift,
		sf::Keyboard::Key key,
		std::function<void(void)> callback)
{
	ShortcutCombo combo = {
		.ctrlRequired = requiresCtrl,
		.shiftRequired = requiresShift,
		.key = key
	};

	if (shortcuts.contains(combo))

	assert(!shortcuts.contains(combo));
	shortcuts[combo] = callback;
}
