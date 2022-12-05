#pragma once

#include <memory>

/*
// TODO: some actions cannot be undone simply (like resize)
// but they can be undoed with the help of snapshot
// exec probably needs some snapshoting tool so it can use it
// to both store state and restore state
class SnaphoterInterface
{
public:
	virtual LevelD takeSnapshot() = 0;
	virtual void applySnaphost(Leveld) = 0;
	virtual ~SnaphoterInterface() {}
};
*/

class CommandInterface
{
public:
	CommandInterface() = default;
	CommandInterface(CommandInterface&&) = delete;
	CommandInterface(const CommandInterface&&) = delete;
	virtual ~CommandInterface() {}

public:
	// Execute command and return undo command
	virtual std::unique_ptr<CommandInterface> exec() = 0;
};
