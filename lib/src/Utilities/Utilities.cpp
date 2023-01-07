#include "include/Utilities/Utilities.hpp"
#include <algorithm>

std::tuple<sf::Image, dgm::Clip> Utilities::createChessboardTexture(const sf::Color& accentColor)
{
	sf::Image img;
	img.create(2, 1);
	img.setPixel(0, 0, sf::Color::Transparent);
	img.setPixel(1, 0, accentColor);
	return { img, dgm::Clip({ 1, 1 }, { 0, 0, 2, 1 }) };
}

sf::Vector2i Utilities::clipNegativeCoords(const sf::Vector2i& v)
{
	return sf::Vector2i(
		std::max(0, v.x),
		std::max(0, v.y)
	);
}

std::optional<sf::IntRect> Utilities::unifyRects(const std::optional<sf::IntRect>& a, const std::optional<sf::IntRect>& b) noexcept
{
	if (a.has_value() && !b.has_value())
		return a;
	else if (!a.has_value() && b.has_value())
		return b;
	else if (!a.has_value() && !b.has_value())
		return {};

	const auto aVal = a.value();
	const auto bVal = b.value();

	const int left = std::min(aVal.left, bVal.left);
	const int top = std::min(aVal.top, bVal.top);
	const int right = std::max(aVal.left + aVal.width, bVal.left + bVal.width);
	const int bottom = std::max(aVal.top + aVal.height, bVal.top + bVal.height);

	return sf::IntRect(
		sf::Vector2i(left, top),
		sf::Vector2i(right - left, bottom - top));
}
