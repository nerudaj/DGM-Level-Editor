#pragma once

#include "include/ShortcutEngineInterface.hpp"
#include "include/ShortcutCombo.hpp"
#include <map>

class ShortcutEngine final : public ShortcutEngineInterface
{
protected:
	typedef std::function<void(void)> Callback;

protected:
	bool ctrlIsPressed = false;
	bool shiftIsPressed = false;
	std::map<ShortcutCombo, Callback> shortcuts;

protected:
	void evaluateShortcut(sf::Keyboard::Key key);

public:
	void handleEvent(sf::Event& event) override;

	void registerShortcut(bool requiresCtrl,
		bool requiresShift,
		sf::Keyboard::Key key,
		std::function<void(void)> callback) override;
};
