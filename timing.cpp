// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "./assert_same.hpp"
#include "./eval.hpp"
#include "./fold.hpp"
#include "./count.hpp"

extern struct counter_
{
    template <long i, class T>
    type_<_int_<(i+1)> >&
    operator()(type_<_int_<i> >&, T&);
}
    &counter;


ASSERT_SAME(
    eval(fold(type(counter), int_<0>(), count(int_<STEPS>()))),
    _int_<(STEPS)>);

int main() {}

