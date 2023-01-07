#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <DGM/classes/Clip.hpp>
#include <tuple>
#include <optional>

namespace Utilities
{
	std::tuple<sf::Image, dgm::Clip> createChessboardTexture(const sf::Color& accentColor);

	// Takes input integer vector and replaces any
	// negative coordinates with zeroes
	sf::Vector2i clipNegativeCoords(const sf::Vector2i& v);

	std::optional<sf::IntRect> unifyRects(
		const std::optional<sf::IntRect>& a,
		const std::optional<sf::IntRect>& b) noexcept;
}