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
	constexpr void add(const Box<UndoableCommandInterface>& command);

	void undo();

	void redo();

public:
	[[nodiscard]]
	constexpr const auto& test_getCommands() const noexcept
	{
		return commands;
	}

	[[nodiscard]]
	constexpr const auto& test_getIndex() const noexcept
	{
		return index;
	}
};