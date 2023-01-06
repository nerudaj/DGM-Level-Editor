#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Commands/CommandHistory.hpp"
#include "include/Utilities/GC.hpp"

#include <queue>
#include <memory>

class CommandQueue final
{
protected:
	GC<CommandHistory> history;
	std::queue<GC<UndoableCommandInterface>> commands;

public:
	CommandQueue(GC<CommandHistory> history)
		: history(history)
	{}
	CommandQueue(CommandQueue&&) = delete;
	CommandQueue(const CommandQueue&) = delete;

public:
	template<UndoableCommand T, class ...Args>
		requires std::constructible_from<T, Args...>
	void push(Args&& ... args)
	{
		commands.push(GC<T>(std::forward<Args>(args)...));
	}

	void processAll();

	[[nodiscard]]
	bool isEmpty() const noexcept
	{
		return commands.empty();
	}
};
