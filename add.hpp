// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ADD_DWA200958_HPP
# define ADD_DWA200958_HPP

# include "./type.hpp"
# include "./int.hpp"

extern struct add_
{
    template <long i, long j>
    type_<_int_<(i+j)> >&
    operator()(type_<_int_<i> >&, type_<_int_<j> >&);
}
    &add;


#endif // ADD_DWA200958_HPP
