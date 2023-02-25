#pragma once

#include "include/Interfaces/CurrentLayerObserverInterface.hpp"

class LayerController final : public CurrentLayerObserverInterface
{
public:
	std::size_t getCurrentLayerId() const noexcept override;
};