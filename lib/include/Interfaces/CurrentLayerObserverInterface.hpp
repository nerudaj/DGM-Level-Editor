#pragma once

#include <limits>

class CurrentLayerObserverInterface
{
public:
	[[nodiscard]]
	virtual std::size_t getCurrentLayerId() const noexcept = 0;
};
