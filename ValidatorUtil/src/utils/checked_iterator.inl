#pragma once
#ifndef checked_iterator_inl_included
#define checked_iterator_inl_included


#include "checked_iterator.h"


#ifdef _MSC_VER


template<typename T>
checked_iterator<T> make_checked_iterator(T const& t, std::uint32_t const& size)
{
	return stdext::make_checked_array_iterator(t, size);
}


#else


template<typename T>
checked_iterator<T> make_checked_iterator(T const& t, std::uint32_t const& size)
{
	return t;
}


#endif


#endif
