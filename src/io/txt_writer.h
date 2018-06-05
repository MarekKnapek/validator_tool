#pragma once
#ifndef txt_writer_h_included
#define txt_writer_h_included

#include "writer.h" // writer
#include "../utils/type_traits.h" // is_writer

#include <cstdint> // std::uint8_t std::uint32_t


class txt_writer
{
public:
	txt_writer() noexcept;
	txt_writer(writer&& wrtr);
	txt_writer(txt_writer const&) = delete;
	txt_writer(txt_writer&& other) noexcept;
	txt_writer& operator=(txt_writer const&) = delete;
	txt_writer& operator=(txt_writer&& other) noexcept;
	~txt_writer() noexcept;
	void swap(txt_writer& other) noexcept;
	void close() noexcept;
public:
	void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size);
private:
	writer m_writer;
};

inline void swap(txt_writer& a, txt_writer& b) noexcept { a.swap(b); }

template<>
struct is_writer_trait<txt_writer> : public std::true_type
{
};


#endif
