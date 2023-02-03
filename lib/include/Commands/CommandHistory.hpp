#pragma once

#include "include/Interfaces/UndoableCommandInterface.hpp"
#include "include/Utilities/GC.hpp"

#include <vector>
#include <memory>

class CommandHistory final
{
	std::vector<GC<UndoableCommandInterface>> commands;
	unsigned index = 0;

public:
	void add(const GC<UndoableCommandInterface>& command);

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