#pragma once
#ifndef type_traits_h_defined
#define type_traits_h_defined


#include <functional> // std::reference_wrapper
#include <utility> // std::forward
#include <type_traits> // std::false_type std::true_type


template<typename T>
class remove_const
{
public:
	typedef T type;
};

template<typename T>
class remove_const<T const>
{
public:
	typedef T type;
};


template<typename T>
class remove_volatile
{
public:
	typedef T type;
};

template<typename T>
class remove_volatile<T volatile>
{
public:
	typedef T type;
};


template<typename T>
class remove_reference
{
public:
	typedef T type;
};

template<typename T>
class remove_reference<T&>
{
public:
	typedef T type;
};


template<typename T>
class remote_rvalue_reference
{
public:
	typedef T type;
};

template<typename T>
class remote_rvalue_reference<T&&>
{
public:
	typedef T type;
};


template<typename T>
class remove_cv_ref_rref
{
public:
	typedef typename remove_const<typename remove_volatile<typename remove_reference<typename remote_rvalue_reference<T>::type>::type>::type>::type type;
};


template<typename, typename>
class is_same : public std::false_type
{
};

template<typename T>
class is_same<T, T> : public std::true_type
{
};


template<typename>
struct is_writer_trait : public std::false_type
{
};

template<typename T>
struct is_writer_trait<std::reference_wrapper<T>> : public is_writer_trait<T>
{
};

template<typename T>
struct is_writer : public is_writer_trait<typename remove_cv_ref_rref<T>::type>
{
};

template<typename T>
/*inline*/ constexpr bool is_writer_v = is_writer<T>::value;


template<typename>
class is_reference_wrapper_impl : public std::false_type
{
};

template<typename T>
class is_reference_wrapper_impl<std::reference_wrapper<T>> : public std::true_type
{
};

template<typename T>
class is_reference_wrapper : public is_reference_wrapper_impl<typename remove_cv_ref_rref<T>::type>
{
};


template<typename T, typename = std::enable_if_t<!is_reference_wrapper<T>::value>>
T&& unwrap(T&& t)
{
	return std::forward<T>(t);
}

template<template<typename> typename T, typename U, typename = std::enable_if_t<is_reference_wrapper<T<U>>::value>>
U& unwrap(T<U> const& t)
{
	return t.get();
}


#endif
