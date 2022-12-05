#include "include/Commands/CommandQueue.hpp"

void CommandQueue::processAll()
{
	while (!commands.empty())
	{
		auto command = std::move(commands.front());
		commands.pop();
		command->exec();
		history.add(std::move(command));
	}
}
