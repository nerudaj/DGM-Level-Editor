#pragma once

#include "include/Commands/CommandInterface.hpp"

#include <vector>
#include <memory>

class CommandHistory final
{
	std::vector<std::unique_ptr<CommandInterface>> commands;
	unsigned index = 0;

public:
	void add(std::unique_ptr<CommandInterface>&& command);

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