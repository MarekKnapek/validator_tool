#pragma once
#ifndef writer_inl_included
#define writer_inl_included


#include "writer.h"

#include "../utils/type_traits.h" // unwrap

#include <utility> // std::move
#include <cstdint> // std::uint8_t std::uint32_t


template<typename T, typename>
writer::writer(T&& writer) :
	m_type_eraser([w = std::move(writer)](std::uint8_t const* const& buffer, std::uint32_t const& size) mutable -> void { return unwrap(w).write_bytes(buffer, size); })
{
}


#endif
