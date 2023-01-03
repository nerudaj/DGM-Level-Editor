#pragma once

#include <concepts>
#include <exception>

template<class T>
class SafePtr {
public:
	template<class ... Args>
		requires std::constructible_from<T, Args...>
	[[nodiscard]]
	constexpr SafePtr(Args&& ... args)
	{
		ptr = new T(std::forward<Args>(args)...);
		[[unlikely]]
		if (!ptr) throw std::bad_alloc();
	}

	SafePtr(std::nullptr_t) = delete;
	SafePtr(const SafePtr&) = delete;

	[[nodiscard]]
	constexpr SafePtr(SafePtr&& other) noexcept
	{
		std::swap(ptr, other.ptr);
	}

	[[nodiscard]]
	constexpr SafePtr& operator=(SafePtr&& other) noexcept
	{
		std::swap(ptr, other.ptr);
		return *this;
	}

	constexpr ~SafePtr() noexcept
	{
		delete ptr;
		ptr = nullptr;
	}

public: // construct from derived
	template<class D>
		requires std::derived_from<D, T>
	[[nodiscard]]
	constexpr SafePtr(SafePtr<D>&& other) noexcept
	{
		ptr = other.ptr;
		other.ptr = nullptr;
	}

	template<class D>
		requires std::derived_from<D, T>
	[[nodiscard]]
	constexpr SafePtr& operator=(SafePtr<D>&& other) noexcept
	{
		ptr = other.ptr;
		other.ptr = nullptr;
		return *this;
	}

public:
	[[nodiscard]]
	constexpr T* operator->() noexcept {
		return ptr;
	}

	[[nodiscard]]
	constexpr T const* operator->() const noexcept {
		return ptr;
	}

	[[nodiscard]]
	constexpr T& operator*() noexcept {
		return *ptr;
	}

	[[nodiscard]]
	constexpr T const& operator*() const noexcept {
		return *ptr;
	}

private:
	T* ptr = nullptr;

	// must be friend of itself so construction from derived works
	friend class SafePtr;
};
