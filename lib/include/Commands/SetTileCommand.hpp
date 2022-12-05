#pragma once

#include "include/Commands/CommandInterface.hpp"
#include "include/LevelMesh/DrawableLeveldMesh.hpp"
#include <SFML/System/Vector2.hpp>

class SetTileCommand final : public CommandInterface
{
protected:
	DrawableLeveldMesh& targetMesh;
	sf::Vector2u tilePos;
	unsigned value = 0;
	bool blocking = false;
	unsigned oldValue = 0;
	bool oldBlocking = false;

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