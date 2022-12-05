#pragma once

#include <SFML/Graphics/Image.hpp>
#include <DGM/classes/Clip.hpp>
#include <tuple>

namespace Utilities
{
	std::tuple<sf::Image, dgm::Clip> createChessboardTexture(const sf::Color& accentColor);
}