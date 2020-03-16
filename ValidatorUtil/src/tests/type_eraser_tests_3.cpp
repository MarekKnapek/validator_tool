#ifdef TYPE_ERASER_WANT_TESTS


#include "type_eraser_tests_3.h"

#include "../utils/type_eraser.h"


class type_eraser_tests_3_1
{
public:
	int operator()(int i)
	{
		return i + 42;
	}
};


class type_eraser_tests_3_2
{
public:
	int operator()(int i) const
	{
		return i + 42;
	}
};


class type_eraser_tests_3_3
{
public:
	int operator()(int i)
	{
		VERIFY(false);
		return i + 0;
	}
	int operator()(int i) const
	{
		return i + 42;
	}
};


class type_eraser_tests_3_4
{
public:
	int operator()(int i)
	{
		return i + 42;
	}
	int operator()(int i) const
	{
		VERIFY(false);
		return i + 0;
	}
};


// int(T::*)(int), operator(), no state
void type_eraser_tests_3()
{
	{
		type_eraser<int, int> e1;
		type_eraser<int, int> e2{};
		VERIFY(!e1.get());
		VERIFY(!e2.get());
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_3_1{}};
		type_eraser<int, int> e2(type_eraser_tests_3_1{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_3_1, type_eraser_value<int(type_eraser_tests_3_1::*)(int), &type_eraser_tests_3_1::operator()>, true, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_3_1, type_eraser_value<int(type_eraser_tests_3_1::*)(int), &type_eraser_tests_3_1::operator()>, true, int, int>*>(e2.get())));
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_3_2{}};
		type_eraser<int, int> e2(type_eraser_tests_3_2{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_3_2, type_eraser_value<int(type_eraser_tests_3_2::*)(int) const, &type_eraser_tests_3_2::operator()>, true, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_3_2, type_eraser_value<int(type_eraser_tests_3_2::*)(int) const, &type_eraser_tests_3_2::operator()>, true, int, int>*>(e2.get())));
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_3_3{}, type_eraser_value<int(type_eraser_tests_3_3::*)(int) const, &type_eraser_tests_3_3::operator()>{}};
		type_eraser<int, int> e2(type_eraser_tests_3_3{}, type_eraser_value<int(type_eraser_tests_3_3::*)(int) const, &type_eraser_tests_3_3::operator()>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_3_3, type_eraser_value<int(type_eraser_tests_3_3::*)(int) const, &type_eraser_tests_3_3::operator()>, true, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_3_3, type_eraser_value<int(type_eraser_tests_3_3::*)(int) const, &type_eraser_tests_3_3::operator()>, true, int, int>*>(e2.get())));
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_3_4{}, type_eraser_value<int(type_eraser_tests_3_4::*)(int), &type_eraser_tests_3_4::operator()>{}};
		type_eraser<int, int> e2(type_eraser_tests_3_4{}, type_eraser_value<int(type_eraser_tests_3_4::*)(int), &type_eraser_tests_3_4::operator()>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_3_4, type_eraser_value<int(type_eraser_tests_3_4::*)(int), &type_eraser_tests_3_4::operator()>, true, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_3_4, type_eraser_value<int(type_eraser_tests_3_4::*)(int), &type_eraser_tests_3_4::operator()>, true, int, int>*>(e2.get())));
	}
}


#endif
