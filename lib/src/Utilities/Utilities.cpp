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

std::optional<TileRect> Utilities::unifyRects(
		const std::optional<TileRect>& a,
		const std::optional<TileRect>& b) noexcept
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
	const int right = std::max(aVal.right, bVal.right);
	const int bottom = std::max(aVal.bottom, bVal.bottom);

	return TileRect(left, top, right, bottom);
}
