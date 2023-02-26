#pragma once

#include "include/Interfaces/CurrentLayerObserverInterface.hpp"

class LayerController final : public LayerObserverInterface
{
public:
	std::size_t getCurrentLayerId() const noexcept override;

	std::size_t getLayerCount() const noexcept override;
};