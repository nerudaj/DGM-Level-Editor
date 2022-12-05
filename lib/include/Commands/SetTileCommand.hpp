#pragma once

#include "include/Commands/CommandInterface.hpp"
#include "include/LevelMesh/DrawableLeveldMesh.hpp"
#include <SFML/System/Vector2.hpp>

class SetTileCommand final : public CommandInterface
{
protected:
	DrawableLeveldMesh& targetMesh;
	const sf::Vector2u& tilePos;
	unsigned value;
	bool blocking;

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
	[[nodiscard]]
	std::unique_ptr<CommandInterface> exec() override;
};