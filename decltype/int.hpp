// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef INT_DWA200957_HPP
# define INT_DWA200957_HPP

# include "./type.hpp"

template <long x>
struct _int_;

template <long x>
type_<_int_<x> >&
int_();

template <long...x>
struct _ints_;

template <long...x>
type_<_ints_<x...> >&
ints_();



#endif // INT_DWA200957_HPP
