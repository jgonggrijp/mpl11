// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef APPEND_DWA201243_HPP
# define APPEND_DWA201243_HPP

template <class ...Ss>
struct append;

// --- type sequences

template <template <class...> class S, class ...T1s>
struct append<S<T1s...> >
    : S<T1s...> {};

template <template <class...> class S, class ...T1s, class ...T2s>
struct append<S<T1s...>, S<T2s...> >
    : S<T1s...,T2s...> {};

template <template <class...> class S, class ...T1s, class ...Ss>
struct append<S<T1s...>, Ss...>
    : append<S<T1s...>, typename append<Ss...>::type> {};

// ---- non-type, non-template sequences

template <template <class T, T...> class S, class T, T ...I1s>
struct append<S<T, I1s...> >
  : S<T, I1s...> {};

template <template <class T, T...> class S, class T, T ...I1s, T ...I2s>
struct append<S<T, I1s...>, S<T, I2s...> >
  : S<T, I1s..., I2s...> {};

template <template <class T, T...> class S, class T, T ...I1s, class ...Ss>
struct append<S<T, I1s...>, Ss...>
  : append<S<T, I1s...>, typename append<Ss...>::type> {};

#endif // APPEND_DWA201243_HPP
