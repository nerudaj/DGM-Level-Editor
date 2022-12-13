#include "include/Commands/SetTileCommand.hpp"

void SetTileCommand::exec()
{
	oldValue = targetMesh.getTileValue(tilePos);
	oldBlocking = targetMesh.isTileSolid(tilePos);

	targetMesh.setTileValue(tilePos, value);
	targetMesh.setTileSolid(tilePos, blocking);
}

std::unique_ptr<CommandInterface> SetTileCommand::getInverse() const
{
	return std::make_unique<SetTileCommand>(
		targetMesh,
		tilePos,
		*oldValue,
		*oldBlocking);
}
