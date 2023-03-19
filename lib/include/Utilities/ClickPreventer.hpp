#pragma once

#include <DGM/dgm.hpp>

class ClickPreventer
{
public:
	void preventClickFor(const sf::Time& time)
	{
		timeLeft = time.asSeconds();
	}

	void update(const dgm::Time& elapsedTime)
	{
		timeLeft = std::clamp(timeLeft - elapsedTime.getDeltaTime(), 0.f, timeLeft);
	}

	bool shouldPreventEvents() const noexcept
	{
		return timeLeft > 0.f;
	}

private:
	float timeLeft = 0.f;
};