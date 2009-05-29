// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef APPLY_DWA200957_HPP
# define APPLY_DWA200957_HPP

// Apply a metafunction F to a set of arguments.  Not sure we even need this.
extern struct apply_
{
    template <class F, class...Args>
    auto operator()(type_<F>&, Args&... a) -> decltype(F()(a...));
} &apply;

#endif // APPLY_DWA200957_HPP
