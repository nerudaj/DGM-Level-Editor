#include <catch.hpp>
#include "include/Utilities/Box.hpp"
#include <iostream>
#include <vector>
#include <queue>

class Base
{
public:
	virtual void foo() = 0;
	constexpr virtual ~Base() = default;
};

class Derived final : public Base
{
public:
	void foo() override
	{
		std::cout << "Derived::foo()" << std::endl;
	}
};

[[nodiscard]]
constexpr bool create_update_test()
{
	Box<int> intBox = MakeBox<int>(10);
	*intBox += 10;
	return *intBox == 20;
}

[[nodiscard]]
constexpr bool constructs_from_derived()
{
	Box<Base> box = MakeBox<Derived>();
	Box<Base> box2 = box;
	Box<Derived> box3 = MakeBox<Derived>();
	Box<Base> box4 = box3;
	return true;
}

[[nodiscard]]
constexpr bool can_be_rebound()
{
	Box<int> a = MakeBox<int>(10);
	Box<int> b = MakeBox<int>(20);
	a = b;
	return *a == 20;
}

TEST_CASE("[Box]")
{
	SECTION("constexpr tests")
	{
		static_assert(create_update_test());
		static_assert(constructs_from_derived());
		static_assert(can_be_rebound());
	}

	SECTION("Works with std::vector")
	{
		std::vector<Box<int>> ints;
		ints.push_back(MakeBox<int>(10));
		ints.push_back(MakeBox<int>(20));
		ints.erase(ints.begin());
		REQUIRE(ints.size() == 1);
		REQUIRE(*ints.front() == 20);
	}

	SECTION("Works with std::queue")
	{
		std::queue<Box<int>> ints;
		ints.push(MakeBox<int>(10));
		ints.push(MakeBox<int>(20));
		ints.pop();
		REQUIRE(ints.size() == 1);
		REQUIRE(*ints.front() == 20);
	}
}
