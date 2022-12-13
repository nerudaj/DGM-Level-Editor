#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Utilities/InitGuard.hpp"

#include <SFML/System/Vector2.hpp>
#include <LevelD.hpp>
#include <vector>

struct CreateItemCommandData
{
	sf::Vector2i position;
	unsigned itemType;
};

class CreateItemCommand : public UndoableCommandInterface
{
protected:
	std::vector<LevelD::Thing>& target;
	std::vector<CreateItemCommandData> itemsToCreate;
	InitGuard<std::vector<unsigned>> idsOfAddedItems;

public:
	CreateItemCommand(
		std::vector<LevelD::Thing>& target,
		sf::Vector2i position,
		unsigned itemType)
		: target(target)
		, itemsToCreate({ { position, itemType } })
	{}

	CreateItemCommand(
		std::vector<LevelD::Thing>& target,
		const std::vector<CreateItemCommandData>& itemsToCreate)
		: target(target)
		, itemsToCreate(itemsToCreate)
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};
