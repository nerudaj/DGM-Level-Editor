#pragma once

#include "include/Commands/CommandInterface.hpp"
#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Editor/Editor.hpp"

#include <LevelD.hpp>

class ResizeCommandInverse final : public CommandInterface
{
protected:
	Editor& editor;
	LevelD snapshot;

public:
	ResizeCommandInverse(
		Editor& editor,
		LevelD snapshot)
		: editor(editor), snapshot(snapshot)
	{}

public:
	void exec() override;
};

class ResizeCommand final : public UndoableCommandInterface
{
protected:
	unsigned width;
	unsigned height;
	Editor& editor;
	LevelD levelSnapshot;

public:
	ResizeCommand(Editor& editor, unsigned width, unsigned height)
		: editor(editor), width(width), height(height)
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};
