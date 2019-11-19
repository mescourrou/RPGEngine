#pragma once

#include <type_traits>
#include <cstdint>

namespace epstl {

typedef uint32_t size_t;
typedef int64_t ssize_t;

class container
{
public:
	container() = default;
	virtual ~container() = default;

	virtual size_t size() const noexcept = 0;
};

template <typename T1, typename T2>
bool less(const T1& t1, const T2& t2)
{
	return t1 < t2;
}

enum sort_order {
	ASCENDING,
	DESCENDING
};

template <typename T>
class linear_container : public container
{
public:
	linear_container() = default;
	virtual ~linear_container() = default;

	virtual size_t push_back(T) = 0;
	virtual size_t pop_back() = 0;

	virtual void sort(bool ascending = true, bool (*less_operator)(const T&, const T&) = &less) = 0;
	virtual const T* at(int index) const noexcept = 0;
	virtual T* at(int index) noexcept = 0;
	virtual void swap(epstl::size_t a, epstl::size_t b) = 0;
};

} // namespace epstl
