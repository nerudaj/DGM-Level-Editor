#include "include/Commands/CreateItemCommand.hpp"
#include "include/Commands/DeleteObjectCommand.hpp"
#include "include/LogConsole.hpp"

CreateItemCommand::CreateItemCommand(
	LevelD::Things& target,
	sf::Vector2i position,
	unsigned itemType)
	: target(target)
{
	const auto itemToCreate = ItemToCreate{
		.arrayPosition = target.size(),
		.object = LevelD::Thing{
			.id = itemType,
			.tag = 0,
			.x = static_cast<unsigned>(position.x),
			.y = static_cast<unsigned>(position.y),
			.flags = 0,
			.metadata = ""
		}
	};

	itemsToCreate.push_back(itemToCreate);
}

void CreateItemCommand::exec()
{
	idsOfAddedItems = CommandHelper::addObjectsToTarget(
		target,
		itemsToCreate);
}

[[nodiscard]]
std::unique_ptr<CommandInterface> CreateItemCommand::getInverse() const
{
	return std::make_unique<DeleteItemCommand>(
		target,
		*idsOfAddedItems);
}
