#pragma once

#include "include/Interfaces/UndoableCommandInterface.hpp"
#include "include/Commands/CommandHelper.hpp"

#include <vector>
#include <set>
#include <SFML/System/Vector2.hpp>


template<TriggerOrThing ObjectType>
class MoveObjectCommand final : public UndoableCommandInterface
{
protected:
	std::vector<ObjectType>& target;
	std::set<std::size_t> selectedItemIds;
	DragContext dragContext;
	sf::Vector2i initialPosition;
	sf::Vector2i finalPosition;
	sf::Vector2u boundary;

public:
	[[nodiscard]]
	constexpr MoveObjectCommand(
		std::vector<ObjectType>& target,
		const std::set<std::size_t>& selectedItemIds,
		const DragContext& dragContext,
		const sf::Vector2i& initialPosition,
		const sf::Vector2i& finalPosition,
		const sf::Vector2u& boundary) noexcept
		: target(target)
		, selectedItemIds(selectedItemIds)
		, dragContext(dragContext)
		, initialPosition(initialPosition)
		, finalPosition(finalPosition)
		, boundary(boundary)
	{}

public:
	virtual void exec() override
	{
		CommandHelper::moveSelectedObjectsTo(
			target,
			selectedItemIds,
			dragContext,
			finalPosition,
			boundary);
	}

	virtual Box<CommandInterface> getInverse() const override
	{
		return Box<MoveObjectCommand>(
			target,
			selectedItemIds,
			dragContext,
			finalPosition,
			initialPosition,
			boundary);
	}
};

typedef MoveObjectCommand<LevelD::Thing> MoveItemCommand;

typedef MoveObjectCommand<LevelD::Trigger> MoveTriggerCommand;
