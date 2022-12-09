#include <catch.hpp>
#include "include/ShortcutEngine.hpp"

void nullCallback() {}

TEST_CASE("[ShortcutEngine]")
{
	ShortcutEngine engine;

	SECTION("Construction")
	{
		SECTION("Allows adding different bindings")
		{
			// No exception or assertions should happen
			engine.registerShortcut(true, false, sf::Keyboard::A, nullCallback);
			engine.registerShortcut(true, true, sf::Keyboard::A, nullCallback);
			engine.registerShortcut(false, false, sf::Keyboard::A, nullCallback);
		}
	}

	SECTION("Invocation")
	{
		SECTION("Invokes a known shortcut")
		{
			bool called = false;
			engine.registerShortcut(
				true,
				false,
				sf::Keyboard::A,
				[&] { called = true; });
			
			sf::Event event;
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::LControl;
			engine.handleEvent(event);

			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::A;
			engine.handleEvent(event);

			REQUIRE(called);
		}
	}
}
