#pragma once

#include "Utils.h" // hex_byte_t hex_address24_t

#include <algorithm> // std::copy
#include <array>
#include <cstdio> // std::FILE
#include <memory> // std::addressof


template<typename>
class reader_t
{
};

template<>
class reader_t<char>
{
public:
	char operator()(std::FILE* const& strm) const;
};

template<>
class reader_t<hex_byte_t>
{
public:
	hex_byte_t operator()(std::FILE* const& strm) const;
};

template<>
class reader_t<hex_address24_t>
{
public:
	hex_address24_t operator()(std::FILE* const& strm) const;
};

template<typename T>
T read_from(std::FILE* const& strm)
{
	return reader_t<T>{}(strm);
}


template<typename T>
class writer_base_t
{
public:
	template<typename OutItT>
	OutItT operator()(T const& e, OutItT const& out_it)
	{
		auto const p = reinterpret_cast<unsigned char const*>(std::addressof(e));
		auto const s = sizeof(e);
		return std::copy(p, p + s, out_it);
	}
};

template<typename>
class writer_t
{
};

template<>
class writer_t<char> : public writer_base_t<char>
{
};

template<>
class writer_t<std::uint8_t> : public writer_base_t<std::uint8_t>
{
};

template<>
class writer_t<hex_byte_t>
{
public:
	template<typename OutItT>
	OutItT operator()(hex_byte_t const& e, OutItT const& out_it)
	{
		return write_to(e.m_data, out_it);
	}
};

template<>
class writer_t<hex_address24_t>
{
public:
	template<typename OutItT>
	OutItT operator()(hex_address24_t const& e, OutItT const& out_it)
	{
		return write_to(e.m_data, out_it);
	}
};

template<typename T, unsigned N>
class writer_t<std::array<T, N>>
{
public:
	template<typename OutItT>
	OutItT operator()(std::array<T, N> const& arr, OutItT const& out_it)
	{
		auto o = out_it;
		for(auto const& e : arr)
		{
			o = write_to(e, o);
		}
		return o;
	}
};

template<typename T, typename OutItT>
OutItT write_to(T const& e, OutItT const& out_it)
{
	return writer_t<T>{}(e, out_it);
}

template<typename InItT, typename OutItT>
OutItT write_to(InItT const& begin, InItT const& end, OutItT const& out_it)
{
	auto o = out_it;
	for(auto b = begin; b != end; ++b)
	{
		o = write_to(*b, o);
	}
	return o;
}
