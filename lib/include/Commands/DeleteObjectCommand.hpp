#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Commands/CreateItemCommand.hpp"
#include "include/Commands/CreateTriggerCommand.hpp"
#include "include/Commands/CommandHelper.hpp"
#include "include/Utilities/InitGuard.hpp"

#include <LevelD.hpp>
#include <vector>

template<TriggerOrThing ObjectType, UndoableCommand InverseCommandType>
class DeleteObjectCommand final : public UndoableCommandInterface
{
protected:
	std::vector<ObjectType>& target;
	std::vector<std::size_t> idsToDelete;
	InitGuard<std::vector<ObjectToCreate<ObjectType>>> objectsToCreate;

public:
	DeleteObjectCommand(
		std::vector<ObjectType>& target,
		std::vector<std::size_t> idsToDelete)
		: target(target)
		, idsToDelete(idsToDelete)
	{}

public:
	void exec() override
	{
		objectsToCreate = CommandHelper::deleteObjectsFromTarget(
			target,
			idsToDelete);
	}

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override
	{
		return std::make_unique<InverseCommandType>(
			target,
			*objectsToCreate);
	}
};

typedef DeleteObjectCommand<LevelD::Thing, CreateItemCommand> DeleteItemCommand;

typedef DeleteObjectCommand<LevelD::Trigger, CreateTriggerCommand> DeleteTriggerCommand;
