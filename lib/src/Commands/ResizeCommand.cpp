#include "include/Commands/ResizeCommand.hpp"

void ResizeCommandInverse::exec()
{
	editor.loadFrom(snapshot, true);
}

void ResizeCommand::exec()
{
	levelSnapshot = editor.save();
	editor.resize(width, height);
}

std::unique_ptr<CommandInterface> ResizeCommand::getInverse() const
{
	return std::make_unique<ResizeCommandInverse>(
		editor,
		levelSnapshot);
}
