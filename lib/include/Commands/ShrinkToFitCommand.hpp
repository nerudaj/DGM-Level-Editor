#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Editor/Editor.hpp"

#include <LevelD.hpp>

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
	Box<CommandInterface> getInverse() const override;

protected:
	Editor& editor;
	LevelD levelSnapshot;
};
