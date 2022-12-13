#include "include/Commands/CreateItemCommand.hpp"
#include "include/Commands/DeleteItemCommand.hpp"
#include "include/LogConsole.hpp"

void CreateItemCommand::exec()
{
	LevelD::Thing item;
	item.id = itemType;
	item.x = position.x;
	item.y = position.y;
	item.tag = 0;
	item.flags = 0;
	item.metadata = "";

	target.push_back(item);
	addedItemId = target.size() - 1;
}

[[nodiscard]]
std::unique_ptr<CommandInterface> CreateItemCommand::getInverse() const
{
	return std::make_unique<DeleteItemCommand>(
		target,
		*addedItemId);
}
