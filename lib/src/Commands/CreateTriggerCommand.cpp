#include "include/Commands/CreateTriggerCommand.hpp"

void CreateTriggerCommand::exec()
{}

[[nodiscard]]
std::unique_ptr<CommandInterface> CreateTriggerCommand::getInverse() const
{
	return nullptr;
}
