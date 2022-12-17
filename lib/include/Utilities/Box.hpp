#pragma once

#include <concepts>
#include <memory>

/**
 *  \brief Single-threaded smart pointer with following properties:
 *
 *  * Non-nullable
 *  * Non-movable
 *  * Has to be initialized
 *  * Owns memory
 *  * Can be copied (reference counting)
 *
 *  Due to this, following errors can never occur:
 *  * Use after free
 *  * Use after move
 *  * Null dereference
 */
template<class T>
class Box
{
public:
	Box() = delete;
	Box(std::nullptr_t = nullptr) = delete;

	constexpr Box(T* ptr) noexcept
	{
		this->ptr = ptr;
		this->referenceCounter = new unsigned long(1);
	}

	constexpr Box(const Box& other) noexcept
	{
		ptr = other.ptr;
		referenceCounter = other.referenceCounter;
		++(*referenceCounter);
	}

	constexpr Box& operator=(Box other) noexcept
	{
		swap(other);
		return *this;
	}

	template<class Derived>
		requires std::derived_from<Derived, T>
	constexpr Box(const Box<Derived>& other) noexcept
	{
		ptr = dynamic_cast<T*>(other.ptr);
		referenceCounter = other.referenceCounter;
		++(*referenceCounter);
	}

	constexpr ~Box() noexcept
	{
		--(*referenceCounter);

		if (*referenceCounter > 0)
			return;

		std::default_delete<T>()(ptr);
		delete referenceCounter;
		ptr = nullptr;
		referenceCounter = nullptr;
	}

public:
	[[nodiscard]]
	constexpr T& operator*() noexcept
	{
		return *ptr;
	}

	[[nodiscard]]
	constexpr const T& operator*() const noexcept
	{
		return *ptr;
	}

	[[nodiscard]]
	constexpr T* operator->() noexcept
	{
		return ptr;
	}

	[[nodiscard]]
	constexpr T const* const operator->() const noexcept
	{
		return ptr;
	}

protected:
	template<class>
	friend class Box;

	constexpr void swap(Box& other) noexcept
	{
		using std::swap;
		swap(ptr, other.ptr);
		swap(referenceCounter, other.referenceCounter);
	}

protected:
	T* ptr = nullptr;
	unsigned long* referenceCounter = nullptr;
};

template<class T, class ... Args>
	requires std::constructible_from<T, Args...>
constexpr Box<T> MakeBox(Args&&... args) noexcept
{
	return Box<T>(new T(std::forward<Args>(args)...));
}
