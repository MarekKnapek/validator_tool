#pragma once
#ifndef writer_inl_included
#define writer_inl_included


#include "writer.h"

#include "writer_wrapper.h" // writer_wrapper

#include <type_traits> // std::alignment_of_v
#include <utility> // std::forward
#include <new> // placement new


template<bool>
struct is_small : std::false_type
{
};

template<>
struct is_small<true> : std::true_type
{
};


template<typename T, typename>
writer::writer(T&& t) :
	writer()
{
	init(std::forward<T>(t), is_small<(sizeof(writer_wrapper<T, true>) <= sizeof(m_storage)) && ((alignof(writer_wrapper<T, true>) % alignof(decltype(t))) == 0)>{});
}

template<typename T>
void writer::init(T&& t, std::true_type const&)
{
	auto const ptr = reinterpret_cast<writer_wrapper_base*>(reinterpret_cast<char*>(&m_storage));
	::new(ptr) writer_wrapper<T, true>(std::forward<T>(t));
	m_writer_wrapper.reset(ptr);
}

template<typename T>
void writer::init(T&& t, std::false_type const&)
{
	m_writer_wrapper.reset(std::make_unique<writer_wrapper<T, false>>(std::forward<T>(t)).release());
}


#endif
