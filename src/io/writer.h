#pragma once
#ifndef writer_h_included
#define writer_h_included


#include "writer_wrapper_base.h" // writer_wrapper_base
#include "../utils/type_traits.h" // is_writer_trait is_writer_v

#include <cstdint> // std::uint8_t std::uint32_t
#include <memory> // std::unique_ptr
#include <type_traits> // std::enable_if_t std::true_type


class writer
{
public:
	writer() noexcept;
	writer(writer const&) = delete;
	writer(writer&& other) noexcept;
	writer& operator=(writer const&) = delete;
	writer& operator=(writer&& other) noexcept;
	~writer();
	void swap(writer& other) noexcept;
public:
	template<typename T, typename = std::enable_if_t<is_writer_v<T>>>
	explicit writer(T&& t);
public:
	void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size);
	writer_wrapper_base* get() const; // unit tests and debug only
private:
	template<typename T>
	void init(T&& t, std::true_type const&);
	template<typename T>
	void init(T&& t, std::false_type const&);
	std::unique_ptr<writer_wrapper_base, writer_wrapper_base_destroyer> m_writer_wrapper;
	void* m_storage[2];
};

inline void swap(writer& a, writer& b) noexcept { a.swap(b); }

template<>
struct is_writer_trait<writer> : public std::true_type
{
};


#include "writer.inl"


#endif
