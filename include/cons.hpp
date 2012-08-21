// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef CONS_DWA201243_HPP
# define CONS_DWA201243_HPP

template <class T, class S> struct cons;

template <template <class...> class S, class T, class ...T1s>
struct cons<T, S<T1s...> >
    : S<T,T1s...>
{};

template <class T, T I, class S> struct cons_c;

template <template <class T, T...> class S, class T, T I, T ...Is>
struct cons_c<T, I, S<T, Is...> >
  : S<T,I,Is...>
{};

#endif // CONS_DWA201243_HPP
