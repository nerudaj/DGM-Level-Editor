#include "include/Commands/SetTileCommand.hpp"

std::unique_ptr<CommandInterface> SetTileCommand::exec()
{
	std::unique_ptr< CommandInterface> undoCommand = std::make_unique<SetTileCommand>(
		targetMesh,
		tilePos,
		targetMesh.getTileValue(tilePos),
		targetMesh.isTileSolid(tilePos));

	targetMesh.setTileValue(tilePos, value);
	targetMesh.setTileSolid(tilePos, blocking);

	return undoCommand;
}
