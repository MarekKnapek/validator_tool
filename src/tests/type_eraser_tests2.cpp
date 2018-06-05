#ifdef TYPE_ERASER_WANT_TESTS


#include "type_eraser_tests2.h"

#include "../utils/type_eraser.h"


class type_eraser_test2_1
{
public:
	int operator()()
	{
		return 42;
	}
};


class type_eraser_test2_2
{
public:
	int operator()() const
	{
		return 42;
	}
};


class type_eraser_test2_3
{
public:
	int operator()()
	{
		VERIFY(false);
		return 0;
	}
	int operator()() const
	{
		return 42;
	}
};


class type_eraser_test2_4
{
public:
	int operator()()
	{
		return 42;
	}
	int operator()() const
	{
		VERIFY(false);
		return 0;
	}
};


// int(T::*)(), operator(), no state
void type_eraser_tests2()
{
	{
		type_eraser<int> e1;
		type_eraser<int> e2{};
		VERIFY(!e1.get());
		VERIFY(!e2.get());
	}
	{
		type_eraser<int> e1{type_eraser_test2_1{}};
		type_eraser<int> e2(type_eraser_test2_1{});
		auto const r1 = e1();
		auto const r2 = e2();
		VERIFY(r1 == 42);
		VERIFY(r2 == 42);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_test2_1, type_eraser_value<int(type_eraser_test2_1::*)(), &type_eraser_test2_1::operator()>, true, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_test2_1, type_eraser_value<int(type_eraser_test2_1::*)(), &type_eraser_test2_1::operator()>, true, int>*>(e2.get())));
	}
	{
		type_eraser<int> e1{type_eraser_test2_2{}};
		type_eraser<int> e2(type_eraser_test2_2{});
		auto const r1 = e1();
		auto const r2 = e2();
		VERIFY(r1 == 42);
		VERIFY(r2 == 42);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_test2_2, type_eraser_value<int(type_eraser_test2_2::*)() const, &type_eraser_test2_2::operator()>, true, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_test2_2, type_eraser_value<int(type_eraser_test2_2::*)() const, &type_eraser_test2_2::operator()>, true, int>*>(e2.get())));
	}
	{
		type_eraser<int> e1{type_eraser_test2_3{}, type_eraser_value<int(type_eraser_test2_3::*)() const, &type_eraser_test2_3::operator()>{}};
		type_eraser<int> e2(type_eraser_test2_3{}, type_eraser_value<int(type_eraser_test2_3::*)() const, &type_eraser_test2_3::operator()>{});
		auto const r1 = e1();
		auto const r2 = e2();
		VERIFY(r1 == 42);
		VERIFY(r2 == 42);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_test2_3, type_eraser_value<int(type_eraser_test2_3::*)() const, &type_eraser_test2_3::operator()>, true, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_test2_3, type_eraser_value<int(type_eraser_test2_3::*)() const, &type_eraser_test2_3::operator()>, true, int>*>(e2.get())));
	}
	{
		type_eraser<int> e1{type_eraser_test2_4{}, type_eraser_value<int(type_eraser_test2_4::*)(), &type_eraser_test2_4::operator()>{}};
		type_eraser<int> e2(type_eraser_test2_4{}, type_eraser_value<int(type_eraser_test2_4::*)(), &type_eraser_test2_4::operator()>{});
		auto const r1 = e1();
		auto const r2 = e2();
		VERIFY(r1 == 42);
		VERIFY(r2 == 42);
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_test2_4, type_eraser_value<int(type_eraser_test2_4::*)(), &type_eraser_test2_4::operator()>, true, int>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_test2_4, type_eraser_value<int(type_eraser_test2_4::*)(), &type_eraser_test2_4::operator()>, true, int>*>(e2.get())));
	}

}


#endif
