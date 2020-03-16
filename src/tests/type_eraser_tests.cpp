#ifdef TYPE_ERASER_WANT_TESTS


#include "type_eraser_tests.h"

#include "type_eraser_tests_1.h"
#include "type_eraser_tests_2.h"
#include "type_eraser_tests_3.h"
#include "type_eraser_tests_4.h"
#include "type_eraser_tests_5.h"
#include "type_eraser_tests_6.h"
#include "type_eraser_tests_move.h"


void type_eraser_tests()
{
	type_eraser_tests_1();
	type_eraser_tests_2();
	type_eraser_tests_3();
	type_eraser_tests_4();
	type_eraser_tests_5();
	type_eraser_tests_6();
	type_eraser_tests_move();
}


#else


void type_eraser_tests()
{
}


#endif
