#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"

class DeleteTriggerCommand : public UndoableCommandInterface
{
public:
	DeleteTriggerCommand() {}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};
