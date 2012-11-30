// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "returns.hpp"
#include "indices.hpp"
#include <utility>
#include <memory>
#include <tuple>
#include <iostream>
#include <type_traits>

template <class T>
struct value_t
{
    value_t(T&& x)
        : value(std::move(x)) {}
    
    value_t(T const& x) : value(x) {}

    value_t const* operator->() const
    {
        return this;
    }
    
    T value;
};

template <class T, T value>
struct constant_t
{
    value_t<T> operator->() const
    {
        return value_t<T>(value);
    }
};

template <template <class...> class thing, class...Args>
auto mk(Args&&... a)
    RETURNS(
        thing<
        typename std::remove_const<
        typename std::remove_reference<Args>::type
        >::type...>(std::forward<Args>(a)...)
    );


template <class F, class... Args>
struct lazy
{
    typedef std::tuple<Args...> args_t;
    F f;
    args_t args;
    
    template <class...Args2>
    lazy(F&& f, Args2&&...args2)
        : f(std::forward<F>(f))
        , args(std::forward<Args2>(args2)...)
    {}

    typedef typename make_indices<sizeof...(Args)>::type positions;
    
    template <unsigned... Ns>
    auto eval(indices<Ns...>) const
    RETURNS(f(std::get<Ns>(args)...));
    
    auto operator->() const
        RETURNS(eval(positions()));
};

namespace strict
{
  struct or_
  {
      template <class T, class U>
      auto operator()(T const& x, U const& y) const
          -> decltype(mk<value_t>(x->value ? x->value : y->value))
      {
          std::cout << "evaluating OR LHS:..." << std::endl;
          auto xx = x->value;
          std::cout << "...Done" << std::endl;
          return mk<value_t>(xx ? xx : y->value);
      };
  };

  struct and_
  {
      template <class T, class U>
      auto operator()(T const& x, U const& y) const
          -> decltype(mk<value_t>(x->value ? y->value : x->value))
      {
          std::cout << "evaluating AND LHS:..." << std::endl;
          auto xx = x->value;
          std::cout << "...Done" << std::endl;
          return mk<value_t>(xx ? y->value : xx);
      };
  };
}

template <class T, class U>
auto or_(T x, U y)
    RETURNS(mk<lazy>(strict::or_(), x,y));

template <class T, class U>
auto and_(T x, U y)
    RETURNS(mk<lazy>(strict::and_(), x,y));

namespace strict
{
  struct next
  {
      template <class T>
      auto operator()(T x) const
          RETURNS(mk<value_t>(x->value + 1));
  };

  struct prev
  {
      template <class T>
      auto operator()(T x) const
          RETURNS(mk<value_t>(x->value - 1));
  };
}

template <class T>
auto next(T x) RETURNS(mk<lazy>(strict::next(), x));

template <class T>
auto prev(T x) RETURNS(mk<lazy>(strict::prev(), x));

template <class T>
struct instrumented
{
        
    T value;

    instrumented(instrumented const& x)
        : value(x.value)
    {
        std::cout << "copied " << *this << " <- " << x << std::endl;
    }
    
    instrumented& operator=(instrumented const& x)
    {
        std::cout << "assigning " << *this << " <- " << x << std::endl;
        value = x.value;
        return *this;
    }

    auto operator+(int n) const -> instrumented<decltype(value + n)>
    {
        std::cout << "adding " << *this << " + " << n << std::endl;
        return value + n;
    }
    
    auto operator-(int n) const -> instrumented<decltype(value - n)>
    {
        std::cout << "subtracting " << *this << " - " << n << std::endl;
        return value - n;
    }
    
    template <class U>
    instrumented(U&& x) : value(std::forward<U>(x))
    {
        std::cout << "constructed " << *this << std::endl;
    }

    explicit operator bool() const
    {
        return value;
    }
    
    friend std::ostream& operator<<(std::ostream& s, instrumented const& x)
    {
        return s << x.value << " @" << &x;
    }
};

int main()
{
    value_t<instrumented<int> > zero = instrumented<int>(0);

    std::cout << "---------- building thunk ---------" << std::endl;
    auto res = or_(next(zero), prev(zero));
    std::cout << "---------- evaluating ---------" << std::endl;
    instrumented<int> result = res->value;
    std::cout << "---------- done ---------" << std::endl;
    
    std::cout << "result=" << result.value << std::endl;
}
