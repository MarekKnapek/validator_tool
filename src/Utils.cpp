#include "Utils.h"

#include "utils/verify.h"

#include <algorithm> // std::all_of
#include <cstdlib> // std::exit
#include <memory> // std::addressof
#include <iterator> // std::size


address24_t::address24_t() :
	m_data()
{
}

address24_t::address24_t(std::uint32_t const& e) :
	address24_t()
{
	set(e);
}

std::uint32_t address24_t::get() const
{
	return
		std::uint32_t(m_data[0] << 16) |
		std::uint32_t(m_data[1] << 8) |
		std::uint32_t(m_data[2] << 0)
	;
}

void address24_t::set(std::uint32_t const& e)
{
	m_data[0] =(e >> 16) & 0xFFu;
	m_data[1] =(e >> 8) & 0xFFu;
	m_data[2] =(e >> 0) & 0xFFu;
}

address24_t& address24_t::operator+=(address24_t const& e)
{
	this->set(this->get() + e.get());
	return *this;
}

address24_t operator+(address24_t const& a, address24_t const& b)
{
	return address24_t(a.get() + b.get());
}

bool operator==(address24_t const& a, address24_t const& b)
{
	return a.get() == b.get();
}


bool is_hex(std::uint8_t const& e)
{
	auto const p = reinterpret_cast<char const*>(std::addressof(e));
	return
		(*p >= '0' && *p <= '9') ||
		(*p >= 'a' && *p <= 'f') ||
		(*p >= 'A' && *p <= 'F')
	;
}

bool is_hex(hex_byte_t const& e)
{
	return std::all_of(cbegin(e.m_data), cend(e.m_data), [](auto const& e){ return is_hex(e); });
}

std::uint8_t hex_to_bin(std::uint8_t const& e)
{
	VERIFY(is_hex(e));
	auto const p = reinterpret_cast<char const*>(std::addressof(e));
	if(*p >= '0' && *p <= '9')
	{
		return *p - '0';
	}
	else if(*p >= 'a' && *p <= 'f')
	{
		return 10 + *p - 'a';
	}
	else
	{
		VERIFY(*p >= 'A' && *p <= 'F');
		return 10 + *p - 'A';
	}
}

std::uint8_t hex_to_bin(hex_byte_t const& e)
{
	VERIFY(std::all_of(cbegin(e.m_data), cend(e.m_data), [](auto const& e){ return is_hex(e); }));
	return (hex_to_bin(e.m_data[0]) << 4) | hex_to_bin(e.m_data[1]);
}

address24_t hex_to_bin(hex_address24_t const& e)
{
	VERIFY(std::all_of(cbegin(e.m_data), cend(e.m_data), [](auto const& e){ return is_hex(e); }));
	address24_t ret;
	ret.m_data[0] = hex_to_bin(e.m_data[0]);
	ret.m_data[1] = hex_to_bin(e.m_data[1]);
	ret.m_data[2] = hex_to_bin(e.m_data[2]);
	return ret;
	//return address24_t{hex_to_bin(e.m_data[0]), hex_to_bin(e.m_data[1]), hex_to_bin(e.m_data[2])};
}

hex_byte_t bin_to_hex(std::uint8_t const& e)
{
	VERIFY(std::size(s_hex_alphabet) >= 0x0F);
	return hex_byte_t
	{
		reinterpret_cast<std::uint8_t const*>(static_cast<char const*>(s_hex_alphabet))[(e >> 4) & 0x0Fu],
		reinterpret_cast<std::uint8_t const*>(static_cast<char const*>(s_hex_alphabet))[e & 0x0Fu]
	};
}

hex_address24_t bin_to_hex(address24_t const& e)
{
	return hex_address24_t{bin_to_hex(e.m_data[0]), bin_to_hex(e.m_data[1]), bin_to_hex(e.m_data[2]) };
}
