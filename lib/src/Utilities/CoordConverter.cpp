#include "include/Utilities/CoordConverter.hpp"

TileRect CoordConverter::convertCoordToTileRect(const CoordRect& box) const noexcept
{
	return TileRect
	{
		.left = box.left / tileSize.x,
		.top = box.top / tileSize.y,
		.right = box.right / tileSize.x,
		.bottom = box.bottom / tileSize.y
	};
}
