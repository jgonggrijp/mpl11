#include "./assert_same.hpp"
#include "./eval.hpp"
#include "./fold.hpp"
#include "./pop_front.hpp"
#include "./list.hpp"
#include "./count.hpp"
#include "./add.hpp"

ASSERT_SAME(eval(pop_front(
                     list(type<int>(),type<char>(),type<long>()))),
            eval(list(type<char>(),type<long>())));

template <class T, class U> struct cell;

extern struct cons_
{
    template <class T, class U>
    type_<cell<T,U> >& operator()(type_<T>&, type_<U>&);
}& cons;

ASSERT_SAME(eval(pop_front(
                     list(type<int>(),type<char>(),type<long>()))),
            eval(list(type<char>(),type<long>())));

ASSERT_SAME(eval(front(
                     list(type<int>(),type<char>(),type<long>()))),
            int);

ASSERT_SAME(eval(fold(type(cons), type<int>(), list())), int);

typedef cell<void,int> x;
ASSERT_SAME(eval(fold(type(cons), type<void>(), list(type<int>()))), x);

typedef cell<cell<void,int>,long> y;
ASSERT_SAME(eval(fold(type(cons), type<void>(), list(type<int>(),type<long>()))), y);

ASSERT_SAME(eval(front(
                     list(type<int>(),type<char>(),type<long>()))),
            int);


ASSERT_SAME(eval(front(count(int_<5>()))), _int_<0>);
ASSERT_SAME(eval(empty(count(int_<5>()))), bool_<false>);
ASSERT_SAME(eval(empty(count(int_<0>()))), bool_<true>);
ASSERT_SAME(eval(empty(count(int_<5>(),int_<5>()))), bool_<true>);

ASSERT_SAME(
    eval(pop_front(count(int_<5>()))),
    eval(count(int_<1>(), int_<5>()))
    );


ASSERT_SAME(
    eval(fold(type(add), int_<0>(), count(int_<5>()))),
    _int_<(52+0+1+2+3+4)>);

int main() {}
