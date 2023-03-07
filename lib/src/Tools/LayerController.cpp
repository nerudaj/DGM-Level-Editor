#include "include/Tools/LayerController.hpp"

std::size_t LayerController::getCurrentLayerId() const noexcept
{
	return layerId;
}

std::size_t LayerController::getLayerCount() const noexcept
{
	return 3;
}

void LayerController::moveUp()
{
	if (layerId + 1 < getLayerCount())
	{
		++layerId;
	}
}

void LayerController::moveDown()
{
	if (layerId > 0)
	{
		--layerId;
	}
}
