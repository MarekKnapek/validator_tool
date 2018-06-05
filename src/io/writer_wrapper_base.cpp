#include "writer_wrapper_base.h"

#include "../utils/verify.h" // VERIFY

#include <utility> // std::move std::swap


writer_wrapper_base::writer_wrapper_base() noexcept
{
}

writer_wrapper_base::writer_wrapper_base(writer_wrapper_base&& other) noexcept :
	writer_wrapper_base()
{
	*this = std::move(other);
}

writer_wrapper_base& writer_wrapper_base::operator=(writer_wrapper_base&& other) noexcept
{
	this->swap(other);
	return *this;
}

writer_wrapper_base::~writer_wrapper_base()
{
}

void writer_wrapper_base::swap(writer_wrapper_base& other) noexcept
{
	using std::swap;
	(void)other;
}


void writer_wrapper_base_destroyer::operator()(writer_wrapper_base* const& ptr) const
{
	VERIFY(ptr);
	ptr->destroy();
}
