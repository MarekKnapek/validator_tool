#ifdef TYPE_ERASER_WANT_TESTS


#include "type_eraser_tests_1.h"

#include "../utils/type_eraser.h"


class type_eraser_tests_1_1
{
public:
	void operator()()
	{
	}
};


class type_eraser_tests_1_2
{
public:
	void operator()() const
	{
	}
};


class type_eraser_tests_1_3
{
public:
	void operator()()
	{
		VERIFY(false);
	}
	void operator()() const
	{
	}
};


class type_eraser_tests_1_4
{
public:
	void operator()()
	{
	}
	void operator()() const
	{
		VERIFY(false);
	}
};


// void(T::*)(), operator(), no state
void type_eraser_tests_1()
{
	{
		type_eraser<void> e1;
		type_eraser<void> e2{};
		VERIFY(!e1.get());
		VERIFY(!e2.get());
	}
	{
		type_eraser<void> e1{type_eraser_tests_1_1{}};
		type_eraser<void> e2(type_eraser_tests_1_1{});
		e1();
		e2();
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_1_1, type_eraser_value<void(type_eraser_tests_1_1::*)(), &type_eraser_tests_1_1::operator()>, true, void>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_1_1, type_eraser_value<void(type_eraser_tests_1_1::*)(), &type_eraser_tests_1_1::operator()>, true, void>*>(e2.get())));
	}
	{
		type_eraser<void> e1{type_eraser_tests_1_2{}};
		type_eraser<void> e2(type_eraser_tests_1_2{});
		e1();
		e2();
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_1_2, type_eraser_value<void(type_eraser_tests_1_2::*)() const, &type_eraser_tests_1_2::operator()>, true, void>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_1_2, type_eraser_value<void(type_eraser_tests_1_2::*)() const, &type_eraser_tests_1_2::operator()>, true, void>*>(e2.get())));
	}
	{
		type_eraser<void> e1{type_eraser_tests_1_3{}, type_eraser_value<void(type_eraser_tests_1_3::*)() const, &type_eraser_tests_1_3::operator()>{}};
		type_eraser<void> e2(type_eraser_tests_1_3{}, type_eraser_value<void(type_eraser_tests_1_3::*)() const, &type_eraser_tests_1_3::operator()>{});
		e1();
		e2();
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_1_3, type_eraser_value<void(type_eraser_tests_1_3::*)() const, &type_eraser_tests_1_3::operator()>, true, void>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_1_3, type_eraser_value<void(type_eraser_tests_1_3::*)() const, &type_eraser_tests_1_3::operator()>, true, void>*>(e2.get())));
	}
	{
		type_eraser<void> e1{type_eraser_tests_1_4{}, type_eraser_value<void(type_eraser_tests_1_4::*)(), &type_eraser_tests_1_4::operator()>{}};
		type_eraser<void> e2(type_eraser_tests_1_4{}, type_eraser_value<void(type_eraser_tests_1_4::*)(), &type_eraser_tests_1_4::operator()>{});
		e1();
		e2();
		VERIFY((e1.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_1_4, type_eraser_value<void(type_eraser_tests_1_4::*)(), &type_eraser_tests_1_4::operator()>, true, void>*>(e1.get())));
		VERIFY((e2.get() && dynamic_cast<type_eraser_wrapper<type_eraser_tests_1_4, type_eraser_value<void(type_eraser_tests_1_4::*)(), &type_eraser_tests_1_4::operator()>, true, void>*>(e2.get())));
	}
}


#endif
