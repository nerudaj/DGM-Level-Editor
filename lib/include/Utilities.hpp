#pragma once

#include <SFML/Graphics/Image.hpp>
#include <DGM/classes/Clip.hpp>
#include <tuple>

namespace Utilities
{
	std::tuple<sf::Image, dgm::Clip> createChessboardTexture(const sf::Color& accentColor);

	// Takes input integer vector and replaces any
	// negative coordinates with zeroes
	sf::Vector2i clipNegativeCoords(const sf::Vector2i& v);
}