#include "include/Commands/ResizeCommand.hpp"

void ResizeMeshCommand::exec()
{
	auto tileValues = std::vector<int>(newSize.x * newSize.y, 0);
	auto solidValues = std::vector<int>(newSize.x * newSize.y, 0);

	const bool upscalingX = newSize.x > target.getMapDimensions().x;
	const bool upscalingY = newSize.y > target.getMapDimensions().y;
	const unsigned offsetX = upscalingX
		? (newSize.x - target.getMapDimensions().x) / 2u
		: 0;
	const unsigned offsetY = upscalingY
		? (newSize.y - target.getMapDimensions().y) / 2u
		: 0;

	for (unsigned y = 0; y < std::min(newSize.y, target.getMapDimensions().y); y++)
	{
		for (unsigned x = 0; x < std::min(newSize.x, target.getMapDimensions().x); x++)
		{
			const unsigned targetX = x + offsetX;
			const unsigned targetY = y + offsetY;
			const unsigned targetI = targetY * newSize.x + targetX;

			tileValues[targetI] = target.getTileValue({ x, y });
			solidValues[targetI] = target.isTileSolid({ x, y });
		}
	}

	target.build(tileValues, solidValues, { newSize.x, newSize.y });
}

std::unique_ptr<CommandInterface> ResizeMeshCommand::getInverse() const
{
	return nullptr;
}

void ResizeCommand::exec()
{
	// TODO: snapshoting
	meshCommand->exec();
}

std::unique_ptr<CommandInterface> ResizeCommand::getInverse() const
{
	return nullptr;
}
