#include "include/Commands/CreateTriggerCommand.hpp"
#include "include/Commands/DeleteObjectCommand.hpp"

void CreateTriggerCommand::exec()
{
	idsOfAddedTriggers = CommandHelper::addObjectsToTarget(
		target,
		triggersToCreate);
}

[[nodiscard]]
std::unique_ptr<CommandInterface> CreateTriggerCommand::getInverse() const
{
	return std::make_unique<DeleteTriggerCommand>(
		target,
		*idsOfAddedTriggers);
}
