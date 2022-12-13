#include "include/Commands/DeleteItemCommand.hpp"
#include "include/Commands/CreateItemCommand.hpp"

void DeleteItemCommand::exec()
{
	// TODO: sort this from highest to lowers and then erase
	auto iterToErase = target.begin() + itemId;

	position = sf::Vector2i(iterToErase->x, iterToErase->y);
	itemType = iterToErase->id;

	target.erase(iterToErase);
}

[[nodiscard]]
std::unique_ptr<CommandInterface> DeleteItemCommand::getInverse() const
{
	return std::make_unique<CreateItemCommand>(
		target,
		*position,
		*itemType);
}
