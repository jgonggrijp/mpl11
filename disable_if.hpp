// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef DISABLE_IF_DWA200957_HPP
# define DISABLE_IF_DWA200957_HPP

# include "./bool.hpp"

extern struct
disable_if_
{
    template <class T>
    T& operator()(_false_&, T&);
}& disable_if;

#endif // DISABLE_IF_DWA200957_HPP
