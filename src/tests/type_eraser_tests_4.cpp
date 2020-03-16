#ifdef TYPE_ERASER_WANT_TESTS


#include "type_eraser_tests_4.h"

#include "../utils/type_eraser.h"


class type_eraser_tests_4_1
{
public:
	int do_stuff(int i)
	{
		return i + 42;
	}
};


class type_eraser_tests_4_2
{
public:
	int do_stuff(int i) const
	{
		return i + 42;
	}
};


class type_eraser_tests_4_3
{
public:
	int do_stuff(int i)
	{
		VERIFY(false);
		return i + 0;
	}
	int do_stuff(int i) const
	{
		return i + 42;
	}
};


class type_eraser_tests_4_4
{
public:
	int do_stuff(int i)
	{
		return i + 42;
	}
	int do_stuff(int i) const
	{
		VERIFY(false);
		return i + 0;
	}
};


// int(T::*)(int), cusom function, no state
void type_eraser_tests_4()
{
	{
		type_eraser<int, int> e1;
		type_eraser<int, int> e2{};
		VERIFY(!e1.get());
		VERIFY(!e2.get());
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_4_1{}, type_eraser_value<int(type_eraser_tests_4_1::*)(int), &type_eraser_tests_4_1::do_stuff>{}};
		type_eraser<int, int> e2(type_eraser_tests_4_1{}, type_eraser_value<int(type_eraser_tests_4_1::*)(int), &type_eraser_tests_4_1::do_stuff>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_4_1, type_eraser_value<int(type_eraser_tests_4_1::*)(int), &type_eraser_tests_4_1::do_stuff>, true, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_4_1, type_eraser_value<int(type_eraser_tests_4_1::*)(int), &type_eraser_tests_4_1::do_stuff>, true, int, int>*>(e2.get())));
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_4_2{}, type_eraser_value<int(type_eraser_tests_4_2::*)(int) const, &type_eraser_tests_4_2::do_stuff>{}};
		type_eraser<int, int> e2(type_eraser_tests_4_2{}, type_eraser_value<int(type_eraser_tests_4_2::*)(int) const, &type_eraser_tests_4_2::do_stuff>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_4_2, type_eraser_value<int(type_eraser_tests_4_2::*)(int) const, &type_eraser_tests_4_2::do_stuff>, true, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_4_2, type_eraser_value<int(type_eraser_tests_4_2::*)(int) const, &type_eraser_tests_4_2::do_stuff>, true, int, int>*>(e2.get())));
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_4_3{}, type_eraser_value<int(type_eraser_tests_4_3::*)(int) const, &type_eraser_tests_4_3::do_stuff>{}};
		type_eraser<int, int> e2(type_eraser_tests_4_3{}, type_eraser_value<int(type_eraser_tests_4_3::*)(int) const, &type_eraser_tests_4_3::do_stuff>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_4_3, type_eraser_value<int(type_eraser_tests_4_3::*)(int) const, &type_eraser_tests_4_3::do_stuff>, true, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_4_3, type_eraser_value<int(type_eraser_tests_4_3::*)(int) const, &type_eraser_tests_4_3::do_stuff>, true, int, int>*>(e2.get())));
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_4_4{}, type_eraser_value<int(type_eraser_tests_4_4::*)(int), &type_eraser_tests_4_4::do_stuff>{}};
		type_eraser<int, int> e2(type_eraser_tests_4_4{}, type_eraser_value<int(type_eraser_tests_4_4::*)(int), &type_eraser_tests_4_4::do_stuff>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_4_4, type_eraser_value<int(type_eraser_tests_4_4::*)(int), &type_eraser_tests_4_4::do_stuff>, true, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_4_4, type_eraser_value<int(type_eraser_tests_4_4::*)(int), &type_eraser_tests_4_4::do_stuff>, true, int, int>*>(e2.get())));
	}
}


#endif
