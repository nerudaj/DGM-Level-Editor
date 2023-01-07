#include <catch.hpp>
#include "include/Utilities/Utilities.hpp"

constexpr bool operator==(const sf::IntRect& a, const sf::IntRect& b)
{
	return a.left == b.left && a.top == b.top && a.width == b.width && a.height == b.height;
}

TEST_CASE("[Utilities]")
{
	SECTION("unifyRects")
	{
		SECTION("Returns empty optional if both params are empty optionals")
		{
			REQUIRE(!Utilities::unifyRects({}, {}).has_value());
		}

		SECTION("Returns first rect if only second one is empty optional")
		{
			sf::IntRect r({ 10, 20 }, { 30, 40 });
			REQUIRE(Utilities::unifyRects(r, {}) == r);
		}

		SECTION("Returns second rect if only first one is empty optional")
		{
			sf::IntRect r({ 10, 20 }, { 30, 40 });
			REQUIRE(Utilities::unifyRects({}, r) == r);
		}

		SECTION("Properly unifies both valid rectangles")
		{
			sf::IntRect r1({ 10, 20 }, { 5, 5 });
			sf::IntRect r2({ 30, 40 }, { 5, 5 });
			sf::IntRect unified({ 10, 20 }, { 25, 25 });

			auto result = Utilities::unifyRects(r1, r2);
			REQUIRE(result == unified);
		}
	}
}