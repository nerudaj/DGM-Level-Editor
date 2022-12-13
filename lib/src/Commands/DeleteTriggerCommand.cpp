#include "include/Commands/DeleteTriggerCommand.hpp"

void DeleteTriggerCommand::exec()
{}

[[nodiscard]]
std::unique_ptr<CommandInterface> DeleteTriggerCommand::getInverse() const
{
	return nullptr;
}
