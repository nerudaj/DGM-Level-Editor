#include "include/Utilities.hpp"

std::tuple<sf::Image, dgm::Clip> Utilities::createChessboardTexture(const sf::Color& accentColor)
{
	sf::Image img;
	img.create(2, 1);
	img.setPixel(0, 0, sf::Color::Transparent);
	img.setPixel(1, 0, accentColor);
	return { img, dgm::Clip({ 1, 1 }, { 0, 0, 2, 1 }) };
}