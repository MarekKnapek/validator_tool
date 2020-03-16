#pragma once
#ifndef memory_writer_h_included
#define memory_writer_h_included


#include "../utils/type_traits.h" // is_writer_trait

#include <cstdint> // std::uint8_t std::uint32_t
#include <type_traits> // std::true_type


class memory_writer
{
public:
	memory_writer() noexcept;
	memory_writer(memory_writer const&) = delete;
	memory_writer(memory_writer&& other) noexcept;
	memory_writer& operator=(memory_writer const&)= delete;
	memory_writer& operator=(memory_writer&& other) noexcept;
	~memory_writer() noexcept;
	void swap(memory_writer& other) noexcept;
public:
	explicit memory_writer(std::uint8_t* const& destination);
	void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size);
private:
	std::uint8_t* m_destination;
};

inline void swap(memory_writer& a, memory_writer& b) noexcept { a.swap(b); }

template<>
struct is_writer_trait<memory_writer> : public std::true_type
{
};


#endif
