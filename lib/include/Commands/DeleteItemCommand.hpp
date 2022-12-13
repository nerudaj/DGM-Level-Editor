#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Utilities/InitGuard.hpp"

#include <SFML/System/Vector2.hpp>
#include <LevelD.hpp>
#include <vector>

class DeleteItemCommand : public UndoableCommandInterface
{
protected:
	std::vector<LevelD::Thing>& target;
	std::vector<unsigned> idsToDelete;
	InitGuard<sf::Vector2i> position;
	InitGuard<unsigned> itemType;

public:
	DeleteItemCommand(
		std::vector<LevelD::Thing>& target,
		std::vector<unsigned> idsToDelete)
		: target(target)
		, idsToDelete(idsToDelete)
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};
