#pragma once

#include "include/Interfaces/UndoableCommandInterface.hpp"
#include "include/Interfaces/EditorInterface.hpp"

#include <LevelD.hpp>

class ResizeCommand final : public UndoableCommandInterface
{
protected:
	unsigned width;
	unsigned height;
	EditorInterface& editor;
	LevelD levelSnapshot;

public:
	[[nodiscard]]
	ResizeCommand(EditorInterface& editor, unsigned width, unsigned height) noexcept
		: editor(editor), width(width), height(height)
	{}

public:
	void exec() override;

	[[nodiscard]]
	Box<CommandInterface> getInverse() const override;
};
