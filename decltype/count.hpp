// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef COUNT_DWA200957_HPP
# define COUNT_DWA200957_HPP

# include "./type.hpp"
# include "./bool.hpp"
# include "./int.hpp"

extern struct count_
{
    template <long start, long finish, long step>
    type_<count_(_ints_<start, finish, step>)>&
    operator()(type_<_int_<start> >&, type_<_int_<finish> >&, type_<_int_<step> >&);

    template <long start, long finish>
    type_<count_(_ints_<start,finish,1>)>&
    operator()(type_<_int_<start> >&, type_<_int_<finish> >&);

    template <long finish>
    type_<count_(_ints_<0, finish, 1>)>&
    operator()(type_<_int_<finish> >&);
}&
count;

template <long start, long finish, long step>
type_<count_(_ints_<start+step, finish, step>)>&
pop_front_impl(type_<count_(_ints_<start, finish, step>)>&);

template <long start, long finish, long step>
type_<_int_<start> >&
front_impl(type_<count_(_ints_<start, finish, step>)>&);

template <long start, long finish, long step>
type_<bool_<(start>=finish)> >&
empty_impl(type_<count_(_ints_<start, finish, step>)>&);


#endif // COUNT_DWA200957_HPP
