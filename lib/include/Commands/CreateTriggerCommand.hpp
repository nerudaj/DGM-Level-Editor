#pragma once

#include "include/Commands/UndoableCommandInterface.hpp"
#include "include/Commands/CommandHelper.hpp"
#include "include/Utilities/InitGuard.hpp"

#include <SFML/System/Vector2.hpp>
#include <LevelD.hpp>
#include <vector>

typedef ObjectToCreate<LevelD::Trigger> TriggerToCreate;

class CreateTriggerCommand : public UndoableCommandInterface
{
protected:
	LevelD::Triggers& target;
	std::vector<TriggerToCreate> triggersToCreate;
	InitGuard<std::vector<std::size_t>> idsOfAddedTriggers;

public:
	CreateTriggerCommand(
		LevelD::Triggers& target,
		const LevelD::Trigger& triggerToCreate)
		: target(target)
	{
		triggersToCreate.push_back(TriggerToCreate{
			.arrayPosition = target.size(),
			.object = triggerToCreate
		});
	}

	// Used by DeleteTriggerCommand
	CreateTriggerCommand(
		LevelD::Triggers& target,
		const std::vector<TriggerToCreate>& triggersToCreate)
		: target(target)
		, triggersToCreate(triggersToCreate)
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};
