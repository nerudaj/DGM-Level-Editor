#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Utilities/Box.hpp"

#include <vector>
#include <memory>

class CommandHistory final
{
	std::vector<Box<UndoableCommandInterface>> commands;
	unsigned index = 0;

public:
	void add(const Box<UndoableCommandInterface>& command);

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