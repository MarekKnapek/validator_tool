#pragma once
#ifndef type_eraser_h_included
#define type_eraser_h_included

#include "verify.h" // VERIFY

#include <type_traits> // std::enable_if
#include <utility> // std::declval std::move
#include <memory> // std::addressof std::unique_ptr


#pragma push_macro("small")
#undef small


class type_eraser_true_type
{
public:
	static constexpr bool value = true;
};

class type_eraser_false_type
{
public:
	static constexpr bool value = false;
};


template<bool>
class type_eraser_bool_to_type
{
};

template<>
class type_eraser_bool_to_type<false> : public type_eraser_false_type
{
};

template<>
class type_eraser_bool_to_type<true> : public type_eraser_true_type
{
};


template<typename T, T V>
class type_eraser_value
{
public:
	static constexpr T value = V;
	typedef T type;
};


template<typename R, typename... A>
class type_eraser_wrapper_base
{
public:
	type_eraser_wrapper_base() noexcept = default;
	type_eraser_wrapper_base(type_eraser_wrapper_base const&) = delete;
	type_eraser_wrapper_base(type_eraser_wrapper_base&&) noexcept = default;
	type_eraser_wrapper_base& operator=(type_eraser_wrapper_base const&) = delete;
	type_eraser_wrapper_base& operator=(type_eraser_wrapper_base&&) noexcept = default;
	~type_eraser_wrapper_base() noexcept = default;
public:
	void move_to(type_eraser_wrapper_base* const& storage)
	{
		return do_move_to(storage);
	}
	void destroy()
	{
		return do_destroy();
	}
	R call(A const&... a)
	{
		return do_call(a...);
	}
protected:
	virtual void do_move_to(type_eraser_wrapper_base* const& storage) = 0;
	virtual void do_destroy() = 0;
	virtual R do_call(A const&... a) = 0;
};

template<typename... A>
class type_eraser_wrapper_base<void, A...>
{
public:
	type_eraser_wrapper_base() noexcept = default;
	type_eraser_wrapper_base(type_eraser_wrapper_base const&) = delete;
	type_eraser_wrapper_base(type_eraser_wrapper_base&&) noexcept = default;
	type_eraser_wrapper_base& operator=(type_eraser_wrapper_base const&) = delete;
	type_eraser_wrapper_base& operator=(type_eraser_wrapper_base&&) noexcept = default;
	~type_eraser_wrapper_base() noexcept = default;
public:
	void move_to(type_eraser_wrapper_base* const& storage)
	{
		return do_move_to(storage);
	}
	void destroy()
	{
		return do_destroy();
	}
	void call(A const&... a)
	{
		return do_call(a...);
	}
protected:
	virtual void do_move_to(type_eraser_wrapper_base* const& storage) = 0;
	virtual void do_destroy() = 0;
	virtual void do_call(A const&... a) = 0;
};


class type_eraser_wrapper_base_deleter
{
public:
	template<typename R, typename... A>
	void operator()(type_eraser_wrapper_base<R, A...>* const& p_wrapper) const
	{
		VERIFY(p_wrapper);
		p_wrapper->destroy();
	}
};


template<typename T, typename F, bool B, typename R, typename... A>
class type_eraser_wrapper : public type_eraser_wrapper_base<R, A...>
{
public:
	type_eraser_wrapper() noexcept = default;
	type_eraser_wrapper(type_eraser_wrapper const&) = delete;
	type_eraser_wrapper(type_eraser_wrapper&&) noexcept = default;
	type_eraser_wrapper& operator=(type_eraser_wrapper const&) = delete;
	type_eraser_wrapper& operator=(type_eraser_wrapper&&) noexcept = default;
	~type_eraser_wrapper() noexcept = default;
public:
	type_eraser_wrapper(T&& t) :
		type_eraser_wrapper_base<R, A...>(),
		m_wrapped(std::move(t))
	{
	}
public:
	virtual void do_move_to(type_eraser_wrapper_base<R, A...>* const& p_storage) override
	{
		VERIFY((dynamic_cast<type_eraser_wrapper<T, F, B, R, A...>*>(this)));
		::new(p_storage) type_eraser_wrapper<T, F, B, R, A...>(std::move(*static_cast<type_eraser_wrapper<T, F, B, R, A...>*>(this)));
	}
	virtual void do_destroy() override
	{
		destroy(type_eraser_bool_to_type<B>{});
	}
	virtual R do_call(A const&... a) override
	{
		return (m_wrapped.*F::value)(a...);
	}
private:
	void destroy(type_eraser_true_type const&)
	{
		this->~type_eraser_wrapper<T, F, B, R, A...>();
	}
	void destroy(type_eraser_false_type const&)
	{
		delete this;
	}
private:
	T m_wrapped;
};

template<typename T, typename F, bool B, typename... A>
class type_eraser_wrapper<T, F, B, void, A...> : public type_eraser_wrapper_base<void, A...>
{
public:
	type_eraser_wrapper() noexcept = default;
	type_eraser_wrapper(type_eraser_wrapper const&) = delete;
	type_eraser_wrapper(type_eraser_wrapper&&) noexcept = default;
	type_eraser_wrapper& operator=(type_eraser_wrapper const&) = delete;
	type_eraser_wrapper& operator=(type_eraser_wrapper&&) noexcept = default;
	~type_eraser_wrapper() noexcept = default;
public:
	type_eraser_wrapper(T&& t) :
		type_eraser_wrapper_base<void, A...>(),
		m_wrapped(std::move(t))
	{
	}
public:
	virtual void do_move_to(type_eraser_wrapper_base<void, A...>* const& p_storage) override
	{
		::new(p_storage) type_eraser_wrapper<T, F, B, void, A...>(std::move(*this));
	}
	virtual void do_destroy() override
	{
		destroy(type_eraser_bool_to_type<B>{});
	}
	virtual void do_call(A const&... a) override
	{
		(m_wrapped.*F::value)(a...);
	}
private:
	void destroy(type_eraser_true_type const&)
	{
		this->~type_eraser_wrapper<T, F, B, void, A...>();
	}
	void destroy(type_eraser_false_type const&)
	{
		delete this;
	}
private:
	T m_wrapped;
};


template<typename R, typename... A>
class type_eraser
{
public:
	type_eraser() noexcept :
		m_p_wrapper(),
		m_storage()
	{
	}
	template<typename T, typename F = type_eraser_value<decltype(&T::operator()), &T::operator()>>
	explicit type_eraser(T&& t, F const& f = {}) :
		m_p_wrapper(),
		m_storage()
	{
		init(std::move(t), f, type_eraser_bool_to_type<(sizeof(type_eraser_wrapper<T, F, true, R, A...>) <= sizeof(m_storage.m_ptrs)) && (alignof(type_eraser_wrapper<T, F, true, R, A...>) <= alignof(decltype(m_storage.m_ptrs)))>{});
	}
	type_eraser(type_eraser const&) = delete;
	type_eraser(type_eraser&& other) :
		type_eraser()
	{
		*this = std::move(other);
	}
	type_eraser& operator=(type_eraser const&) = delete;
	type_eraser& operator=(type_eraser&& other) noexcept
	{
		VERIFY(this != std::addressof(other));
		clear();
		if(!other.empty())
		{
			if(other.small())
			{
				auto const& this_storage = reinterpret_cast<type_eraser_wrapper_base<R, A...>*>(std::addressof(m_storage.m_chars));
				other.m_p_wrapper->move_to(this_storage);
				other.m_p_wrapper.reset();
				m_p_wrapper.reset(this_storage);
			}
			else
			{
				m_p_wrapper.reset(other.m_p_wrapper.release());
			}
		}
		return *this;
	}
	void swap(type_eraser& other) noexcept
	{
		VERIFY(this != std::addressof(other));
		std::swap(*this, other);
	}
	~type_eraser()
	{
	}
	R operator()(A const&... a)
	{
		VERIFY(m_p_wrapper);
		return m_p_wrapper->call(a...);
	}
	bool empty() const
	{
		return !m_p_wrapper;
	}
	bool small() const
	{
		VERIFY(!empty());
		return m_p_wrapper.get() == reinterpret_cast<type_eraser_wrapper_base<R, A...> const*>(std::addressof(m_storage.m_chars));
	}
	void clear()
	{
		m_p_wrapper.reset();
	}
public:
	type_eraser_wrapper_base<R, A...>* get() const
	{
		// for debugging and testing purposes only
		return m_p_wrapper.get();
	}
private:
	template<typename T, typename F>
	void init(T&& t, F const&, type_eraser_true_type const&)
	{
		auto const ptr = reinterpret_cast<type_eraser_wrapper_base<R, A...>*>(std::addressof(m_storage.m_chars));
		::new(ptr) type_eraser_wrapper<T, F, true, R, A...>(std::move(t));
		m_p_wrapper.reset(ptr);
	}
	template<typename T, typename F>
	void init(T&& t, F const&, type_eraser_false_type const&)
	{
		m_p_wrapper.reset(new type_eraser_wrapper<T, F, false, R, A...>{std::move(t)});
	}
private:
	union storage_t
	{
		unsigned char m_chars[sizeof(void*) * 2];
		void* m_ptrs[2];
	};
private:
	std::unique_ptr<type_eraser_wrapper_base<R, A...>, type_eraser_wrapper_base_deleter> m_p_wrapper;
	storage_t m_storage;
};

template<typename R, typename... A>
void swap(type_eraser<R, A...>& a, type_eraser<R, A...>& b) noexcept { a.swap(b); }


#pragma pop_macro("small")


#endif
