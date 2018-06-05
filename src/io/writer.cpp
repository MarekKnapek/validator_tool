#include "writer.h"

#include "../utils/verify.h" // VERIFY

#include <memory> // std::addressof
#include <utility> // std::move std::swap


writer::writer() noexcept :
	m_writer_wrapper(),
	m_storage()
{
}

writer::writer(writer&& other) noexcept :
	writer()
{
	*this = std::move(other);
}

writer& writer::operator=(writer&& other) noexcept
{
	VERIFY(this != std::addressof(other));
	this->swap(other);
	return *this;
}

writer::~writer()
{
}

void writer::swap(writer& other) noexcept
{
	VERIFY(this != std::addressof(other));

	auto const fn_move_assign = [](writer& to, writer&& from)
	{
		auto const from_storage = reinterpret_cast<writer_wrapper_base*>(reinterpret_cast<char*>(&from.m_storage));
		auto const to_storage = reinterpret_cast<writer_wrapper_base*>(reinterpret_cast<char*>(&to.m_storage));
		auto const from_is_small = from.m_writer_wrapper.get() == from_storage;
		if(from_is_small)
		{
			from.m_writer_wrapper->move_construct(to_storage);
			to.m_writer_wrapper.reset(to_storage);
			from.m_writer_wrapper.reset();
		}
		else
		{
			to.m_writer_wrapper.reset(from.m_writer_wrapper.release());
		}
	};

	writer tmp;

	writer& t = tmp;
	writer& a = *this;
	writer& b = other;

	fn_move_assign(t, std::move(a));
	fn_move_assign(a, std::move(b));
	fn_move_assign(b, std::move(t));
}

void writer::write_bytes(std::uint8_t const* const& buffer, std::uint32_t const& size)
{
	m_writer_wrapper->write_bytes(buffer, size);
}

writer_wrapper_base* writer::get() const
{
	// unit tests and debug only
	return m_writer_wrapper.get();
}
