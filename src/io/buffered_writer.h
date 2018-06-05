#pragma once
#ifndef buffered_writer_h_included
#define buffered_writer_h_included


#include "../utils/type_traits.h" // is_writer_trait
#include "../utils/windows_memory.h" // virtual_free_deleter
#include "writer.h" // writer

#include <cstdint> // std::uint8_t std::uint32_t
#include <type_traits> // std::true_type


class buffered_writer
{
public:
	buffered_writer() noexcept;
	buffered_writer(buffered_writer const&) = delete;
	buffered_writer(buffered_writer&& other) noexcept;
	buffered_writer& operator=(buffered_writer const&) = delete;
	buffered_writer& operator=(buffered_writer&&) noexcept;
	~buffered_writer();
	void swap(buffered_writer& other) noexcept;
public:
	buffered_writer(writer&& byte_writer);
	writer& get(); // unit tests and debug only
	writer const& get() const; // unit tests and debug only
	void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size);
private:
	void write_bytes_nocheck(std::uint8_t const* const& buffer, std::uint32_t const& size);
private:
	writer m_byte_writer;
	std::unique_ptr<std::uint8_t, virtual_free_deleter> m_buffer;
	std::uint32_t m_idx;
};


inline void swap(buffered_writer& a, buffered_writer& b) noexcept { a.swap(b); }


template<>
struct is_writer_trait<buffered_writer> : public std::true_type
{
};


#endif
