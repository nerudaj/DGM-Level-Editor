#include "include/Commands/ResizeCommand.hpp"
#include "include/Commands/RestoreFromSnapshotCommand.hpp"

void ResizeCommand::exec()
{
	levelSnapshot = editor.save();
	editor.resize(width, height);
}

std::unique_ptr<CommandInterface> ResizeCommand::getInverse() const
{
	return std::make_unique<RestoreFromSnapshotCommand>(
		editor,
		levelSnapshot);
}
