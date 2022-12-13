#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"

#include <vector>
#include <memory>

class CommandHistory final
{
	std::vector<std::unique_ptr<UndoableCommandInterface>> commands;
	unsigned index = 0;

public:
	void add(std::unique_ptr<UndoableCommandInterface>&& command);

	void undo();

	void redo();

public:
	const auto& test_getCommands() const
	{
		return commands;
	}

	const auto& test_getIndex() const
	{
		return index;
	}
};