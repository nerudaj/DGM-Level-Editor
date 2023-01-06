#pragma once

#include <LevelD.hpp>
#include "include/Editor/EditorState.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

class EditorInterface
{
public:
	[[nodiscard]]
	virtual bool isInitialized() const noexcept = 0;

	virtual void draw() = 0;

	virtual void init(unsigned levelWidth, unsigned levelHeight, const std::string& configPath) = 0;

	virtual void handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) = 0;

	[[nodiscard]]
	virtual LevelD save() const = 0;

	virtual void loadFrom(
		const LevelD& lvd,
		bool skipInit = false) = 0;

	virtual void switchTool(EditorState state) = 0;

	virtual void resizeDialog() = 0;

	virtual void resize(unsigned width, unsigned height) = 0;

	virtual void shrinkToFit() = 0;

	virtual ~EditorInterface() = default;
};
