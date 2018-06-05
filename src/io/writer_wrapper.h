#pragma once
#ifndef writer_wrapper_h_included
#define writer_wrapper_h_included


#include "../utils/type_traits.h" // is_writer_trait
#include "writer_wrapper_base.h" // writer_wrapper_base
#include "../utils/type_traits.h" // is_writer

#include <cstdint> // std::uint8_t std::uint32_t
#include <type_traits> // std::true_type


template<typename T, bool B>
class writer_wrapper : public writer_wrapper_base
{
public:
	writer_wrapper() noexcept;
	writer_wrapper(writer_wrapper const&) = delete;
	writer_wrapper(writer_wrapper&& other) noexcept;
	writer_wrapper& operator=(writer_wrapper const&) = delete;
	writer_wrapper& operator=(writer_wrapper&& other) noexcept;
	~writer_wrapper();
	void swap(writer_wrapper& other) noexcept;
public:
	writer_wrapper(T&& t) noexcept;
public:
	T& get(); // unit tests and debug only
	T const& get() const; // unit tests and debug only
	virtual void destroy() override;
	virtual void move_construct(writer_wrapper_base* const& ptr) override;
	virtual void write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size) override;
private:
	void destroy(std::true_type const&);
	void destroy(std::false_type const&);
private:
	T m_orig_writer;
};

template<typename T, bool B>
void swap(writer_wrapper<T, B>& a, writer_wrapper<T, B>& b) noexcept { a.swap(b); }

template<typename T, bool B>
struct is_writer_trait<writer_wrapper<T, B>> : public std::true_type
{
};


#include "writer_wrapper.inl"


#endif
