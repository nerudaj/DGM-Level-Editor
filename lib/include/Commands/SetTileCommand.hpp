#pragma once

#include "include/Commands/CommandInterface.hpp"
#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/LevelMesh/DrawableLeveldMesh.hpp"
#include "include/Utilities/InitGuard.hpp"

#include <SFML/System/Vector2.hpp>

class SetTileCommand final : public UndoableCommandInterface
{
protected:
	DrawableLeveldMesh& targetMesh;
	sf::Vector2u tilePos;
	unsigned value = 0;
	bool blocking = false;
	InitGuard<unsigned> oldValue;
	InitGuard<bool> oldBlocking;

public:
	SetTileCommand(
		DrawableLeveldMesh& targetMesh,
		const sf::Vector2u& tilePos,
		unsigned value,
		bool blocking)
		: targetMesh(targetMesh)
		, tilePos(tilePos)
		, value(value)
		, blocking(blocking)
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};
