#pragma once
#ifndef writer_wrapper_inl_included
#define writer_wrapper_inl_included


#include "writer_wrapper.h"

#include "../utils/type_traits.h" // unwrap
#include "../utils/verify.h" // VERIFY
#include "../utils/aligned_alloc.h" // aligned_alloc aligned_free

#include <memory> // std::addressof
#include <utility> // std::move std::swap
#include <cstddef> // std::max_align_t
#include <cstdint>
#include <new> // placement new


template<bool>
struct is_small2 : std::false_type
{
};

template<>
struct is_small2<true> : std::true_type
{
};


template<typename T, bool B>
writer_wrapper<T, B>::writer_wrapper() noexcept :
	writer_wrapper_base(),
	m_orig_writer()
{
}

template<typename T, bool B>
writer_wrapper<T, B>::writer_wrapper(writer_wrapper&& other) noexcept :
	writer_wrapper_base(static_cast<writer_wrapper_base&&>(other)),
	m_orig_writer(std::move(other.m_orig_writer))
{
}

template<typename T, bool B>
writer_wrapper<T, B>& writer_wrapper<T, B>::operator=(writer_wrapper<T, B>&& other) noexcept
{
	VERIFY(this != std::addressof(other));
	this->swap(other);
	return *this;
}

template<typename T, bool B>
writer_wrapper<T, B>::~writer_wrapper()
{
}

template<typename T, bool B>
void writer_wrapper<T, B>::swap(writer_wrapper<T, B>& other) noexcept
{
	VERIFY(this != std::addressof(other));
	using std::swap;
	swap(static_cast<writer_wrapper_base&>(this), static_cast<writer_wrapper_base&>(other));
	swap(this->m_orig_writer, other.m_orig_writer);
}

template<typename T, bool B>
writer_wrapper<T, B>::writer_wrapper(T&& t) noexcept :
	writer_wrapper_base(),
	m_orig_writer(std::move(t))
{
}

template<typename T, bool B>
T& writer_wrapper<T, B>::get()
{
	// unit tests and debug only
	return m_orig_writer;
}

template<typename T, bool B>
T const& writer_wrapper<T, B>::get() const
{
	// unit tests and debug only
	return m_orig_writer;
}

template<typename T, bool B>
void writer_wrapper<T, B>::destroy()
{
	destroy(is_small2<B>{});
}

template<typename T, bool B>
void writer_wrapper<T, B>::move_construct(writer_wrapper_base* const& ptr)
{
	::new(ptr)writer_wrapper<T, B>(std::move(*this));
}

template<typename T, bool B>
void writer_wrapper<T, B>::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	unwrap(m_orig_writer).write_bytes(buffer, size);
}

template<typename T, bool B>
void writer_wrapper<T, B>::destroy(std::true_type const&)
{
	this->~writer_wrapper();
}

template<typename T, bool B>
void writer_wrapper<T, B>::destroy(std::false_type const&)
{
	delete this;
}


#endif
