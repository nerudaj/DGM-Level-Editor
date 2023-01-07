#pragma once

#include "include/Commands/CommandInterface.hpp"
#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Editor/Editor.hpp"

#include <LevelD.hpp>

class ShrinkToFitCommandInverse final : public CommandInterface
{
public:
	[[nodiscard]]
	ShrinkToFitCommandInverse(
		Editor& editor,
		LevelD snapshot) noexcept
		: editor(editor)
		, snapshot(snapshot)
	{}

public:
	void exec() override;

protected:
	Editor& editor;
	LevelD snapshot;
};

class ShrinkToFitCommand final : public UndoableCommandInterface
{
public:
	[[nodiscard]]
	ShrinkToFitCommand(
		Editor& editor) noexcept
		: editor(editor)
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;

protected:
	Editor& editor;
	LevelD levelSnapshot;
};
