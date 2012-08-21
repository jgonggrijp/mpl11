// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef TYPE_DWA2012819_HPP
# define TYPE_DWA2012819_HPP

// A generic wrapper for type T
template <class T>
struct type
{
    // A way to get a sequence of values of type T
    template <T ...Vs>
    struct value;
};

template <char... n>
using char_ = type<char>::value<n...>;

template <int... n>
using int_ = type<int>::value<n...>;

template <std::size_t... n>
using size_t_ = type<std::size_t>::value<n...>;

template <class T>
struct unwrap { typedef T type; };

template <class T>
struct unwrap<type<T> > { typedef T type; };

#endif // TYPE_DWA2012819_HPP
