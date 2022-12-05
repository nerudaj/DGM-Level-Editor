#include "include/Commands/CommandHistory.hpp"

void CommandHistory::add(std::unique_ptr<CommandInterface>&& command)
{
	if (index < commands.size())
	{
		commands.erase(commands.begin() + index, commands.end());
	}

	commands.push_back(std::move(command));
	index++;
}

void CommandHistory::undo()
{
	if (index == 0) return;

	index--;
	commands[index]->getInverse()->exec();
}

void CommandHistory::redo()
{}
