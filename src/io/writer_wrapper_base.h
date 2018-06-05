#pragma once
#ifndef writer_wrapper_base_h_included
#define writer_wrapper_base_h_included


#include <cstdint> // std::uint8_t std::uint32_t
#include "../utils/type_traits.h" // is_writer


class writer_wrapper_base
{
public:
	writer_wrapper_base() noexcept;
	writer_wrapper_base(writer_wrapper_base const&) = delete;
	writer_wrapper_base(writer_wrapper_base&& other) noexcept;
	writer_wrapper_base& operator=(writer_wrapper_base const&) = delete;
	writer_wrapper_base& operator=(writer_wrapper_base&& other) noexcept;
	~writer_wrapper_base();
	void swap(writer_wrapper_base& other) noexcept;
public:
	virtual void destroy() = 0;
	virtual void move_construct(writer_wrapper_base* const& ptr) = 0;
	virtual void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size) = 0;
};

inline void swap(writer_wrapper_base& a, writer_wrapper_base& b) noexcept { a.swap(b); }

template<>
struct is_writer_trait<writer_wrapper_base> : public std::true_type
{
};

class writer_wrapper_base_destroyer
{
public:
	void operator()(writer_wrapper_base* const& ptr) const;
};


#endif
