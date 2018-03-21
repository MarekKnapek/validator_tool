#pragma once
#ifndef verify_h_included
#define verify_h_included


#include <cassert> // assert


/* Warning C6319 Use of the comma-operator in a tested expression causes the left argument to be ignored when it has no side-effects. */
#define VERIFY(X) do{ if(!(X)){ assert(!(#X)); verify_failed(__LINE__); } }while(false)


void verify_failed(int const& line);


#endif
