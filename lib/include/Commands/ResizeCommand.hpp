#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Editor/Editor.hpp"

#include <LevelD.hpp>

class ResizeCommand final : public UndoableCommandInterface
{
protected:
	unsigned width;
	unsigned height;
	Editor& editor;
	LevelD levelSnapshot;

public:
	[[nodiscard]]
	ResizeCommand(Editor& editor, unsigned width, unsigned height) noexcept
		: editor(editor), width(width), height(height)
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};
