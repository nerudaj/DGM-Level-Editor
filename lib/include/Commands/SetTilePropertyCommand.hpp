#pragma once

#include "include/Interfaces/UndoableCommandInterface.hpp"
#include "include/LevelMesh/DrawableLeveldMesh.hpp"
#include "include/Utilities/InitGuard.hpp"

class SetTilePropertyCommand : public UndoableCommandInterface
{
public:
	SetTilePropertyCommand(
		DrawableLeveldMesh& map,
		sf::Vector2u const& tileCoord,
		unsigned tileValue,
		bool solid)
		: map(map)
		, tileCoord(tileCoord)
		, tileValue(tileValue)
		, solid(solid)
	{}

public:
	virtual void exec() override;

	virtual Box<CommandInterface> getInverse() const override;

protected:
	DrawableLeveldMesh& map;
	sf::Vector2u tileCoord;
	unsigned tileValue;
	bool solid;
	InitGuard<unsigned> oldTileValue;
	InitGuard<bool> oldSolid;
};