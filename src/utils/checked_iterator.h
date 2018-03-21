#pragma once
#ifndef checked_iterator_h_included
#define checked_iterator_h_included


#ifdef _MSC_VER


#include <iterator>


template<typename T>
using checked_iterator = stdext::checked_array_iterator<T>;

template<typename T>
checked_iterator<T> make_checked_iterator(T const& t, std::uint32_t const& size);


#else


template<typename T>
using checked_iterator = T;

template<typename T>
checked_iterator<T> make_checked_iterator(T const& t, std::uint32_t const& size);


#endif


#include "checked_iterator.inl"


#endif
