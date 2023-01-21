#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Utilities/InitGuard.hpp"

#include <LevelD.hpp>
#include <SFML/System/Vector2.hpp>

class SetItemPropertyCommand : public UndoableCommandInterface
{
public:
	SetItemPropertyCommand(
		std::vector<LevelD::Thing>& target,
		unsigned id,
		LevelD::Thing const& data,
		sf::Vector2u const& levelSize)
		: target(target)
		, itemId(id)
		, data(data)
		, levelSize(levelSize)
	{}

public:
	virtual void exec() override;

	virtual Box<CommandInterface> getInverse() const override;

protected:
	std::vector<LevelD::Thing>& target;
	unsigned itemId;
	LevelD::Thing data;
	sf::Vector2u levelSize;
	InitGuard<LevelD::Thing> oldData;
};
