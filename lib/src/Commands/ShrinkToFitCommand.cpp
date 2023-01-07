#include "include/Commands/ShrinkToFitCommand.hpp"

void ShrinkToFitCommandInverse::exec()
{
	editor.loadFrom(snapshot, true);
}

void ShrinkToFitCommand::exec()
{
	levelSnapshot = editor.save();
	editor.shrinkToFit();
}

std::unique_ptr<CommandInterface> ShrinkToFitCommand::getInverse() const
{
	return std::make_unique<ShrinkToFitCommandInverse>(
		editor,
		levelSnapshot);
}
