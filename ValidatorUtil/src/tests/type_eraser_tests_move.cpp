#ifdef TYPE_ERASER_WANT_TESTS


#include "type_eraser_tests_move.h"

#include "../utils/type_eraser.h"


class type_eraser_tests_move_1
{
public:
	type_eraser_tests_move_1() noexcept :
		m_int(42)
	{
	}
	type_eraser_tests_move_1(type_eraser_tests_move_1 const&) = delete;
	type_eraser_tests_move_1& operator=(type_eraser_tests_move_1 const&) = delete;
	type_eraser_tests_move_1(type_eraser_tests_move_1&& other) noexcept :
		m_int(std::move(other.m_int))
	{
	}
	type_eraser_tests_move_1& operator=(type_eraser_tests_move_1&&) = delete;
	unsigned char const* operator()()
	{
		return reinterpret_cast<unsigned char*>(&m_int);
	}
private:
	int m_int;
};


class type_eraser_tests_move_2
{
public:
	type_eraser_tests_move_2() :
		m_flt(42.0 * 1000.0)
	{
	}
	type_eraser_tests_move_2(type_eraser_tests_move_2 const&) = delete;
	type_eraser_tests_move_2& operator=(type_eraser_tests_move_2 const&) = delete;
	type_eraser_tests_move_2(type_eraser_tests_move_2&& other) noexcept :
		m_flt(std::move(other.m_flt))
	{
	}
	type_eraser_tests_move_1& operator=(type_eraser_tests_move_1&&) = delete;
	unsigned char const* operator()()
	{
		return reinterpret_cast<unsigned char*>(&m_flt);
	}
private:
	float m_flt;
};


void type_eraser_tests_move()
{
	{
		type_eraser<unsigned char const*> e1;

		VERIFY(!e1.get());

		type_eraser<unsigned char const*> e2 = std::move(e1);

		VERIFY(!e1.get());

		VERIFY(!e2.get());
	}
	{
		type_eraser<unsigned char const*> e1{type_eraser_tests_move_1{}};

		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_move_1, type_eraser_value<unsigned char const*(type_eraser_tests_move_1::*)(), &type_eraser_tests_move_1::operator()>, true, unsigned char const*>*>(e1.get())));
		VERIFY(*reinterpret_cast<int const*>(e1()) == 42);

		type_eraser<unsigned char const*> e2 = std::move(e1);

		VERIFY(!e1.get());

		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_move_1, type_eraser_value<unsigned char const*(type_eraser_tests_move_1::*)(), &type_eraser_tests_move_1::operator()>, true, unsigned char const*>*>(e2.get())));
		VERIFY(*reinterpret_cast<int const*>(e2()) == 42);
	}
	{
		type_eraser<unsigned char const*> e1{type_eraser_tests_move_1{}};

		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_move_1, type_eraser_value<unsigned char const*(type_eraser_tests_move_1::*)(), &type_eraser_tests_move_1::operator()>, true, unsigned char const*>*>(e1.get())));
		VERIFY(*reinterpret_cast<int const*>(e1()) == 42);

		type_eraser<unsigned char const*> e2{type_eraser_tests_move_2{}};

		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_move_2, type_eraser_value<unsigned char const*(type_eraser_tests_move_2::*)(), &type_eraser_tests_move_2::operator()>, true, unsigned char const*>*>(e2.get())));
		VERIFY(*reinterpret_cast<float const*>(e2()) == 42.0 * 1000.0);

		e1 = std::move(e2);

		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_move_2, type_eraser_value<unsigned char const*(type_eraser_tests_move_2::*)(), &type_eraser_tests_move_2::operator()>, true, unsigned char const*>*>(e1.get())));
		VERIFY(*reinterpret_cast<float const*>(e1()) == 42.0 * 1000.0);

		VERIFY(!e2.get());
	}
	{
		type_eraser<unsigned char const*> e1{type_eraser_tests_move_1{}};

		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_move_1, type_eraser_value<unsigned char const*(type_eraser_tests_move_1::*)(), &type_eraser_tests_move_1::operator()>, true, unsigned char const*>*>(e1.get())));
		VERIFY(*reinterpret_cast<int const*>(e1()) == 42);

		type_eraser<unsigned char const*> e2{type_eraser_tests_move_2{}};

		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_move_2, type_eraser_value<unsigned char const*(type_eraser_tests_move_2::*)(), &type_eraser_tests_move_2::operator()>, true, unsigned char const*>*>(e2.get())));
		VERIFY(*reinterpret_cast<float const*>(e2()) == 42.0 * 1000.0);

		swap(e1, e2);

		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_move_2, type_eraser_value<unsigned char const*(type_eraser_tests_move_2::*)(), &type_eraser_tests_move_2::operator()>, true, unsigned char const*>*>(e1.get())));
		VERIFY(*reinterpret_cast<float const*>(e1()) == 42.0 * 1000.0);

		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_move_1, type_eraser_value<unsigned char const*(type_eraser_tests_move_1::*)(), &type_eraser_tests_move_1::operator()>, true, unsigned char const*>*>(e2.get())));
		VERIFY(*reinterpret_cast<int const*>(e2()) == 42);
	}
}


#endif
