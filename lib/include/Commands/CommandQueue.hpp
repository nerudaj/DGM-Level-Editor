#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Commands/CommandHistory.hpp"
#include "include/Utilities/Box.hpp"

#include <queue>
#include <memory>

class CommandQueue final
{
protected:
	CommandHistory& history;
	std::queue<Box<UndoableCommandInterface>> commands;

public:
	CommandQueue(CommandHistory& history)
		: history(history)
	{}
	CommandQueue(CommandQueue&&) = delete;
	CommandQueue(const CommandQueue&) = delete;

public:
	template<UndoableCommand T, class ...Args>
		requires std::constructible_from<T, Args...>
	void push(Args&& ... args)
	{
		commands.push(MakeBox<T>(std::forward<Args>(args)...));
	}

	void processAll();

	[[nodiscard]]
	bool isEmpty() const noexcept
	{
		return commands.empty();
	}
};
