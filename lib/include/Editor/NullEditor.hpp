#pragma once

#include "include/Editor/EditorInterface.hpp"

class NullEditor final : public EditorInterface
{
public:
	[[nodiscard]]
	bool isInitialized() const noexcept override { return false; }

	void draw() override {}

	void init(unsigned, unsigned, const std::string&)  override {}

	void handleEvent(const sf::Event&, const sf::Vector2i&)  override {}

	[[nodiscard]]
	LevelD save() const override { return {}; }

	void loadFrom(
		const LevelD&,
		bool skipInit)  override
	{}

	void switchTool(EditorState) override {}

	void resizeDialog() override {}

	void resize(unsigned, unsigned) override {}

	void shrinkToFit() override {}
};
