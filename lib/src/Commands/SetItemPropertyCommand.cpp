#include "include/Commands/SetItemPropertyCommand.hpp"

#include <algorithm>

void SetItemPropertyCommand::exec()
{
	oldData = target[itemId];
	target[itemId] = data;
	target[itemId].x = std::clamp<unsigned>(
		target[itemId].x,
		0u,
		levelSize.x);
	target[itemId].y = std::clamp<unsigned>(
		target[itemId].y,
		0u,
		levelSize.y);
}

Box<CommandInterface> SetItemPropertyCommand::getInverse() const
{
	return Box<SetItemPropertyCommand>(
		target,
		itemId,
		*oldData,
		levelSize);
}
