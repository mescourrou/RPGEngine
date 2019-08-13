#pragma once

#include "container.hpp"

#include <cmath>
#include <initializer_list>

namespace epstl {

template <typename T, epstl::size_t ALLOCATION_BATCH = 5>
class vector : public linear_container<T>
{
	template<typename IT_TYPE, bool ASCENDING>
	class iteratorT
	{
	public:
		iteratorT(IT_TYPE* data) : m_data(data) {}

		iteratorT& operator++() {
			if (ASCENDING)
				m_data ++;
			else
				m_data --;
			return *this;
		}

		IT_TYPE* operator->() const { return m_data; }
		IT_TYPE* operator*() const { return m_data; }

		bool operator!=(const iteratorT<IT_TYPE, ASCENDING>& cmp) const { return m_data != cmp.m_data; }

	private:
		IT_TYPE* m_data;
	};
public:
	typedef iteratorT<T, true> iterator;
	typedef iteratorT<const T, true> const_iterator;
	typedef iteratorT<T, false> reverse_iterator;
	typedef iteratorT<const T, false> const_reverse_iterator;
	vector() = default;
	vector(std::initializer_list<T> list);
	~vector() override;

	epstl::size_t push_back(T e) override;
	epstl::size_t pop_back() override;
	epstl::size_t size() const noexcept override;

	const T* at(int index) const noexcept override;
	T* at(int index) noexcept override;

	epstl::size_t allocated() const noexcept;

	iterator begin() const { return iterator(m_data); }
	iterator end() const { return iterator(m_data + m_size);}

	const_iterator cbegin() const { return const_iterator(m_data); }
	const_iterator cend() const { return const_iterator(m_data + m_size);}

	reverse_iterator rbegin() const { return reverse_iterator(m_data+m_size-1);}
	reverse_iterator rend() const { return reverse_iterator(m_data? m_data-1:nullptr);}

	void sort(bool ascending = true, bool (*less_operator)(const T &, const T &) = &less) override;
	void swap(epstl::size_t a, epstl::size_t b) override;

private:
	epstl::size_t m_allocated = 0;
	epstl::size_t m_size = 0;
	T* m_data = nullptr;

	void quick_sort(epstl::size_t begin, epstl::size_t end, bool ascending, bool (*less_operator)(const T &, const T &) = &less);
	epstl::size_t partition(epstl::size_t begin, epstl::size_t end, bool ascending, bool (*less_operator)(const T &, const T &) = &less);
};

template<typename T, epstl::size_t ALLOCATION_BATCH>
const T* vector<T,ALLOCATION_BATCH>::at(int index) const noexcept
{
	if (std::abs(index) >= m_size)
		return nullptr;
	if (index < 0)
		index = m_size - index;

	return &m_data[index];

}

template<typename T, epstl::size_t ALLOCATION_BATCH>
T* vector<T,ALLOCATION_BATCH>::at(int index) noexcept
{
	if (std::abs(index) >= m_size)
		return nullptr;
	if (index < 0)
		index = m_size - index;

	return &m_data[index];
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
inline epstl::size_t vector<T,ALLOCATION_BATCH>::allocated() const noexcept
{
	return m_allocated;
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
vector<T,ALLOCATION_BATCH>::vector(std::initializer_list<T> list)
{
	if (list.size() == 0)
		return;
	m_size = list.size();
	m_allocated = (std::ceil((float)m_size/(float)ALLOCATION_BATCH))*ALLOCATION_BATCH;
	m_data = new T[m_allocated];

	epstl::size_t i = 0;
	for (const auto& item : list)
	{
		m_data[i] = T(item);
		i++;
	}
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
vector<T,ALLOCATION_BATCH>::~vector()
{
	delete[] m_data;
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
epstl::size_t vector<T,ALLOCATION_BATCH>::push_back(T e)
{
	if (m_size +1 > m_allocated)
	{
		m_allocated = (std::ceil((float)(m_size+1)/(float)ALLOCATION_BATCH))*ALLOCATION_BATCH;
		T* new_vector = new T[m_allocated];
		for (epstl::size_t i = 0; i < m_size; i++)
		{
			new_vector[i] = std::move(m_data[i]);
		}
		delete[] m_data;
		m_data = new_vector;
	}
	m_data[m_size] = std::move(e);

	return ++m_size;
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
epstl::size_t vector<T,ALLOCATION_BATCH>::pop_back()
{
	m_size--;
	if (m_size <= m_allocated - ALLOCATION_BATCH)
	{
		m_allocated = (std::ceil((float)m_size/(float)ALLOCATION_BATCH))*ALLOCATION_BATCH;
		T* new_vector = new T[m_allocated];
		for (epstl::size_t i = 0; i < m_size; i++)
		{
			new_vector[i] = std::move(m_data[i]);
		}
		delete[] m_data;
		m_data = new_vector;
	}
	return m_size;
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
inline epstl::size_t vector<T,ALLOCATION_BATCH>::size() const noexcept
{
	return m_size;
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
void vector<T,ALLOCATION_BATCH>::sort(bool ascending, bool (*less_operator)(const T &, const T &))
{
	quick_sort(0, m_size-1, ascending, less_operator);
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
void vector<T,ALLOCATION_BATCH>::swap(size_t a, size_t b)
{
	T tmp(std::move(m_data[a]));
	m_data[a] = std::move(m_data[b]);
	m_data[b] = std::move(tmp);
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
void vector<T,ALLOCATION_BATCH>::quick_sort(size_t begin, size_t end,
											bool ascending, bool (*less_operator)(const T &, const T &))
{
	if (begin < end)
	{
		epstl::size_t pivot_index = partition(begin, end, ascending, less_operator);
		if (pivot_index >= 1)
			quick_sort(begin, pivot_index-1, ascending, less_operator);
		quick_sort(pivot_index+1, end, ascending, less_operator);
	}
}

template<typename T, epstl::size_t ALLOCATION_BATCH>
size_t vector<T,ALLOCATION_BATCH>::partition(size_t begin, size_t end,
											 bool ascending, bool (*less_operator)(const T &, const T &))
{
	// We choose the pivot as the last element of the partition
	T& pivot = m_data[end];

	epstl::ssize_t smallerElement = begin - 1;
	for (size_t i = begin; i < end; i++)
	{
		if (less_operator(m_data[i], pivot) == ascending)
		{
			smallerElement++;
			swap(smallerElement, i);
		}
	}
	swap(smallerElement+1, end);
	return smallerElement+1;
}

}
