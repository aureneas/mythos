#pragma once

#include <memory>

template <class T, class D = std::default_delete<T> >
class semi_unique_ptr : public std::unique_ptr<T, D> {

private:
	std::unique_ptr<T, D> mPointer;

public:
	constexpr semi_unique_ptr() noexcept {}
	constexpr semi_unique_ptr(nullptr_t) noexcept {}
	explicit semi_unique_ptr(T* val) noexcept : mPointer(val) {}
	template <class U, class E> semi_unique_ptr(semi_unique_ptr<U, E>& mem) noexcept : mPointer(mem.mPointer) {}

	T* get(void) const noexcept { return mPointer.get(); }
	explicit operator bool() const noexcept { return mPointer.operator bool(); }
};