// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef VECTOR_DWA201243_HPP
# define VECTOR_DWA201243_HPP

template <class ...Ts> struct vector { typedef vector type; };

template <class T, T ...Is> struct vector_c { typedef vector_c type; };

#endif // VECTOR_DWA201243_HPP
