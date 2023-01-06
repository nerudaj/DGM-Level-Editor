#pragma once

#include "include/Commands/CommandInterface.hpp"

#include <memory>

class UndoableCommandInterface : public CommandInterface
{
public:
	[[nodiscard]]
	virtual std::unique_ptr<CommandInterface> getInverse() const = 0;
};

template<class T>
concept UndoableCommand = std::derived_from<T, UndoableCommandInterface>;
