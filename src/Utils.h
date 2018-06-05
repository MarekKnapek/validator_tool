#pragma once

#include <array>
#include <cassert>
#include <cstdint> // std:: uint8_t std::uint32_t


static constexpr char const s_hex_alphabet[] = "0123456789ABCDEF";
static constexpr std::uint8_t const s_CR = 0x0D;
static constexpr std::uint8_t const s_LF = 0x0A;


struct hex_byte_t
{
	std::array<std::uint8_t, 2> m_data;
};


struct address24_t
{
	address24_t();
	explicit address24_t(std::uint32_t const& e);
	std::uint32_t get() const;
	void set(std::uint32_t const& e);
	address24_t& operator+=(address24_t const& e);
	address24_t& operator+=(std::uint32_t const& e) { return this->operator+=(address24_t(e)); }
	std::array<std::uint8_t, 3> m_data;
};

address24_t operator+(address24_t const& a, address24_t const& b);
inline address24_t operator+(address24_t const& a, std::uint32_t const& b) { return a + address24_t(b); }
inline address24_t operator+(std::uint32_t const& a, address24_t const& b) { return address24_t(a) + b; }

bool operator==(address24_t const& a, address24_t const& b);
inline bool operator!=(address24_t const& a, address24_t const& b) { return !(a == b); }
inline bool operator==(address24_t const& a, std::uint32_t const& b) { return a == address24_t(b); }
inline bool operator!=(address24_t const& a, std::uint32_t const& b) { return !(a == b); }
inline bool operator==(std::uint32_t const& a, address24_t const& b) { return address24_t(a) == b; }
inline bool operator!=(std::uint32_t const& a, address24_t const& b) { return !(a == b); }


struct hex_address24_t
{
	std::array<hex_byte_t, 3> m_data;
};


bool is_hex(std::uint8_t const& e);
bool is_hex(hex_byte_t const& e);
std::uint8_t hex_to_bin(std::uint8_t const& e);
std::uint8_t hex_to_bin(hex_byte_t const& e);
address24_t hex_to_bin(hex_address24_t const& e);
hex_byte_t bin_to_hex(std::uint8_t const& e);
hex_address24_t bin_to_hex(address24_t const& e);