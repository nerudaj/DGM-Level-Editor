#pragma once

#include <LevelD.hpp>
#include <vector>
#include <concepts>
#include <algorithm>

template<class T>
concept TriggerOrThing = std::is_same_v<T, LevelD::Thing>
|| std::is_same_v<T, LevelD::Trigger>;

template<TriggerOrThing T>
struct ObjectToCreate
{
	std::size_t arrayPosition;
	T object;
};

class CommandHelper
{
public:
	template<TriggerOrThing T>
	[[nodiscard]]
	constexpr static auto
		addObjectsToTarget(
			std::vector<T>& target,
			std::vector<ObjectToCreate<T>> objectsToAdd) noexcept
		-> std::vector<std::size_t>
	{
		std::vector<std::size_t> idsOfAddedItems;
		for (auto&& objToCreate : objectsToAdd)
		{
			target.insert(
				target.begin() + objToCreate.arrayPosition,
				objToCreate.object);
			idsOfAddedItems.push_back(objToCreate.arrayPosition);
		}
		return idsOfAddedItems;
	}

	template<TriggerOrThing T>
	[[nodiscard]]
	constexpr static auto
		deleteObjectsFromTarget(
			std::vector<T>& target,
			std::vector<std::size_t> idsToDelete) noexcept
		-> std::vector<ObjectToCreate<T>>
	{
		std::vector<ObjectToCreate<T>> itemsToCreate;

		// Sorting from greatest to lowest
		std::sort(idsToDelete.rbegin(), idsToDelete.rend());
		for (auto id : idsToDelete)
		{
			auto itrToErase = target.begin() + id;
			itemsToCreate.push_back({ id, *itrToErase });
			target.erase(itrToErase);
		}

		// Sorting from lowest to greatest
		std::sort(
			itemsToCreate.begin(),
			itemsToCreate.end(),
			[] (auto left, auto right) -> bool
			{
				return left.arrayPosition < right.arrayPosition;
			});

		return itemsToCreate;
	}
};