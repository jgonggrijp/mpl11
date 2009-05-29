// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef INC_DWA200957_HPP
# define INC_DWA200957_HPP

# include "./type.hpp"
# include "./int.hpp"

// increment an integer
extern struct dec_
{
    template <long i>
    type_<int_<i-1> >& operator()(type_<int_<i> >&);
}
    &dec;

#endif // INC_DWA200957_HPP
