// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef FOLD_DWA200957_HPP
# define FOLD_DWA200957_HPP

# include "./list.hpp"
# include "./empty.hpp"
# include "./front.hpp"
# include "./pop_front.hpp"
# include "./disable_if.hpp"

extern struct fold_
{
    template <class Fn, class X, class Seq>
    type_<X>& // auto
    operator()(Fn& fn, type_<X>& x, type_<Seq>& s)
    //        -> decltype(type<X>())
        ;

    template <class Fn, class X, class Seq>
    auto
    operator()(type_<Fn>& fn, type_<X>& x, type_<Seq>& s)
        -> decltype(
                fold(
                    disable_if(empty_impl(type<Seq>()), type<Fn>()),                          // fn
                    Fn()(type<X>(), front_impl(type<Seq>())), // fn(x,front(s))
                    pop_front_impl(type<Seq>())               // pop_front(s)
                    )
            );
} & fold;

#endif // FOLD_DWA200957_HPP
