#include "include/Commands/ShrinkToFitCommand.hpp"
#include "include/Commands/RestoreFromSnapshotCommand.hpp"

void ShrinkToFitCommand::exec()
{
	levelSnapshot = editor.save();
	editor.shrinkToFit();
}

std::unique_ptr<CommandInterface> ShrinkToFitCommand::getInverse() const
{
	return std::make_unique<RestoreFromSnapshotCommand>(
		editor,
		levelSnapshot);
}
