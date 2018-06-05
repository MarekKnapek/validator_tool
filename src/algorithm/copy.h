#pragma once
#ifndef copy_h_included
#define copy_h_included


template<typename T, typename U, unsigned N = 64>
void copy(T& from, U& to);


#include "copy.inl"


#endif
