#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"

class CreateTriggerCommand : public UndoableCommandInterface
{
public:
	CreateTriggerCommand() {}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};
