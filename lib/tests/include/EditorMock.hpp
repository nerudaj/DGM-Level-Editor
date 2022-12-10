#pragma once

#include "include/Editor/Editor.hpp"

struct EditorMockState
{
	unsigned drawCallCounter = 0;
	unsigned initCallCounter = 0;
	unsigned handleEventCallCounter = 0;
	unsigned saveToFileCallCounter = 0;
	unsigned loadFromFileCallCounter = 0;
	unsigned switchToolCallCounter = 0;
	unsigned resizeDialogCallCounter = 0;
	unsigned shrinkToFitCallCounter = 0;
};

class EditorMock final : public EditorInterface
{
public:
	EditorMockState* state;

	virtual bool isInitialized() const override
	{
		return state->initCallCounter > 0;
	}

	virtual void draw() override
	{
		state->drawCallCounter++;
	}

	virtual void init(unsigned, unsigned, const std::string&) override
	{
		state->initCallCounter++;
	}

	virtual void handleEvent(const sf::Event&, const sf::Vector2i&) override
	{
		state->handleEventCallCounter++;
	}

	virtual void saveToFile(const std::string&) override
	{
		state->saveToFileCallCounter++;
	}

	virtual void loadFromFile(const std::string&) override
	{
		state->saveToFileCallCounter++;
	}

	virtual void switchTool(EditorState) override
	{
		state->switchToolCallCounter++;
	}

	virtual void resizeDialog() override
	{
		state->resizeDialogCallCounter++;
	}

	virtual void shrinkToFit() override
	{
		state->shrinkToFitCallCounter++;
	}

	EditorMock(EditorMockState* state) : state(state) {}
};