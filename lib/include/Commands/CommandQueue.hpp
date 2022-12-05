#pragma once

#include "include/Commands/CommandInterface.hpp"
#include "include/Commands/CommandHistory.hpp"

#include <queue>
#include <memory>

class CommandQueue final
{
protected:
	CommandHistory& history;
	std::queue<std::unique_ptr<CommandInterface>> commands;

public:
	CommandQueue(CommandHistory& history)
		: history(history)
	{}
	CommandQueue(CommandQueue&&) = delete;
	CommandQueue(const CommandQueue&) = delete;

public:
	void push(std::unique_ptr<CommandInterface>&& command)
	{
		commands.push(std::move(command));
	}

	void processAll();
};
