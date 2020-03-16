#ifdef TYPE_ERASER_WANT_TESTS


#include "type_eraser_tests_6.h"

#include "../utils/type_eraser.h"


class type_eraser_tests_6_0
{
public:
	type_eraser_tests_6_0() :
		m_state()
	{
	}
private:
	void* m_state[2];
};


class type_eraser_tests_6_1 : public type_eraser_tests_6_0
{
public:
	int do_stuff(int i)
	{
		return i + 42;
	}
};


class type_eraser_tests_6_2 : public type_eraser_tests_6_0
{
public:
	int do_stuff(int i) const
	{
		return i + 42;
	}
};


class type_eraser_tests_6_3 : public type_eraser_tests_6_0
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


class type_eraser_tests_6_4 : public type_eraser_tests_6_0
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

// int(T::*)(int), cusom function, two pointers state
void type_eraser_tests_6()
{
	{
		type_eraser<int, int> e1;
		type_eraser<int, int> e2{};
		VERIFY(!e1.get());
		VERIFY(!e2.get());
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_6_1{}, type_eraser_value<int(type_eraser_tests_6_1::*)(int), &type_eraser_tests_6_1::do_stuff>{}};
		type_eraser<int, int> e2(type_eraser_tests_6_1{}, type_eraser_value<int(type_eraser_tests_6_1::*)(int), &type_eraser_tests_6_1::do_stuff>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_6_1, type_eraser_value<int(type_eraser_tests_6_1::*)(int), &type_eraser_tests_6_1::do_stuff>, false, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_6_1, type_eraser_value<int(type_eraser_tests_6_1::*)(int), &type_eraser_tests_6_1::do_stuff>, false, int, int>*>(e2.get())));
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_6_2{}, type_eraser_value<int(type_eraser_tests_6_2::*)(int) const, &type_eraser_tests_6_2::do_stuff>{}};
		type_eraser<int, int> e2(type_eraser_tests_6_2{}, type_eraser_value<int(type_eraser_tests_6_2::*)(int) const, &type_eraser_tests_6_2::do_stuff>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_6_2, type_eraser_value<int(type_eraser_tests_6_2::*)(int) const, &type_eraser_tests_6_2::do_stuff>, false, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_6_2, type_eraser_value<int(type_eraser_tests_6_2::*)(int) const, &type_eraser_tests_6_2::do_stuff>, false, int, int>*>(e2.get())));
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_6_3{}, type_eraser_value<int(type_eraser_tests_6_3::*)(int) const, &type_eraser_tests_6_3::do_stuff>{}};
		type_eraser<int, int> e2(type_eraser_tests_6_3{}, type_eraser_value<int(type_eraser_tests_6_3::*)(int) const, &type_eraser_tests_6_3::do_stuff>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_6_3, type_eraser_value<int(type_eraser_tests_6_3::*)(int) const, &type_eraser_tests_6_3::do_stuff>, false, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_6_3, type_eraser_value<int(type_eraser_tests_6_3::*)(int) const, &type_eraser_tests_6_3::do_stuff>, false, int, int>*>(e2.get())));
	}
	{
		type_eraser<int, int> e1{type_eraser_tests_6_4{}, type_eraser_value<int(type_eraser_tests_6_4::*)(int), &type_eraser_tests_6_4::do_stuff>{}};
		type_eraser<int, int> e2(type_eraser_tests_6_4{}, type_eraser_value<int(type_eraser_tests_6_4::*)(int), &type_eraser_tests_6_4::do_stuff>{});
		auto const r1 = e1(1000);
		auto const r2 = e2(1000);
		VERIFY(r1 == 1042);
		VERIFY(r2 == 1042);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_6_4, type_eraser_value<int(type_eraser_tests_6_4::*)(int), &type_eraser_tests_6_4::do_stuff>, false, int, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_6_4, type_eraser_value<int(type_eraser_tests_6_4::*)(int), &type_eraser_tests_6_4::do_stuff>, false, int, int>*>(e2.get())));
	}
}


#endif
