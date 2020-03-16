#pragma once
#ifndef writer_h_included
#define writer_h_included


#include "../utils/type_eraser.h" // type_eraser
#include "../utils/type_traits.h" // is_writer_v is_writer_trait

#include <cstdint> // std::uint8_t std::uint32_t
#include <type_traits> // std::enable_if_t std::true_type


class writer
{
public:
	writer() noexcept;
	writer(writer const&) = delete;
	writer(writer&& other) noexcept;
	writer& operator=(writer const&) = delete;
	writer& operator=(writer&& other) noexcept;
	~writer() noexcept;
	void swap(writer& other) noexcept;
public:
	template<typename T, typename = std::enable_if_t<is_writer_v<T>>>
	explicit writer(T&& writer);
	void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size);
private:
	type_eraser<void, std::uint8_t const* const&, std::uint32_t const&> m_type_eraser;
};

inline void swap(writer& a, writer& b) noexcept { a.swap(b); }

template<>
struct is_writer_trait<writer> : public std::true_type
{
};


#include "writer.inl"


#endif
