#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Commands/CommandHelper.hpp"
#include "include/Utilities/InitGuard.hpp"

#include <SFML/System/Vector2.hpp>
#include <LevelD.hpp>
#include <vector>

typedef ObjectToCreate<LevelD::Thing> ItemToCreate;

class CreateItemCommand : public UndoableCommandInterface
{
protected:
	LevelD::Things& target;
	std::vector<ItemToCreate> itemsToCreate;
	InitGuard<std::vector<std::size_t>> idsOfAddedItems;

public:
	CreateItemCommand(
		LevelD::Things& target,
		sf::Vector2i position,
		unsigned itemType);

	// Used by DeleteItemCommand
	CreateItemCommand(
		LevelD::Things& target,
		const std::vector<ItemToCreate>& itemsToCreate)
		: target(target)
		, itemsToCreate(itemsToCreate)
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};
