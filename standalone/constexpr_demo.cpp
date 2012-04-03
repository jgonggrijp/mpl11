// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
//
// Utilities
//

// RETURNS() is used to avoid writing boilerplate "->decltype(x) { return x; }" phrases.
//
// USAGE: auto function(<arguments>) RETURNS(<some-expression>);
//
// Note: we end with a unique typedef so the function can be followed
// by a semicolon.  If we omit the semicolon, editors get confused and
// think we haven't completed the function declaration.
#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); } typedef int RETURNS_CAT(RETURNS_, __LINE__)
// Standard PP concatenation formula
#define RETURNS_CAT_0(x, y) x ## y
#define RETURNS_CAT(x, y) RETURNS_CAT_0(x,y)

// Strangely, the message argument to static_assert is mandatory; 99%
// of the time I just want the text of the test.  This macro makes
// static_assert usable.
#define static_assert_(x) static_assert(x, #x)

// assert_type(<expression>, <type>)
//
// A simple static assertion that decltype(<expression>) is <type>
template <class T, class U> struct ERROR_not_same_type;
template <class T> struct ERROR_not_same_type<T,T> { static bool const pass = true; };
#define assert_type(expr, ...) \
    static_assert(ERROR_not_same_type<__VA_ARGS__, decltype(expr)>::pass, "dummy")


//
// Calculate pi via series expansion.  See
// http://en.wikipedia.org/wiki/Pi
//

// This is the traditional Gregory-Leibniz series, which converges
// quite slowly.
constexpr double pi0_(unsigned j)
{
    return j == 0 ? 0.0
        : pi0_(j-1) + (j%2?4.0:-4.0)/(2*j-1);
}

// This one applies the van Wijngaarden transformation
constexpr double pi_(unsigned i, unsigned j)
{
    return i == 0
        ? pi0_(j)
        : ( pi_(i-1,j) + pi_(i-1,j+1) )/2;
}

// This one keeps going deeper until the value stops changing,
// signifying we've run out of precision in a double
constexpr double calcpi2(unsigned depth)
{
    return pi_(depth,depth) == pi_(depth+1,depth+1)
        ? pi_(depth,depth)
        : calcpi2(depth+1);
}

// Calculate pi as accurately as possible.
constexpr double pi = calcpi2(1);

// A compile-time linked list
struct node
{
    constexpr node(int n, node const* next = nullptr)
        : n{n}, next{next} {}
    constexpr node push(int n) { return node(n, this); }

    int n;
    node const* next;
};

constexpr node a(0);
constexpr node b = a.push(1);
constexpr node c = b.push(2);
int x0_[a.n];
int x1_[c.next->n];
int x2_[c.next->next->n];

//  constexpr node d0 = node(4, c.push(3));
//  constexpr node d1 = c.push(3).push(4);
//  int x2[d.next->n];



//
// A more general lisp-style cons list with length and contents
// encoded in its type.
//
struct nil_ { constexpr nil_() {} };
std::ostream& operator<<(std::ostream& s, nil_ const&) { return s << "nil"; }

constexpr nil_ nil;

template <class H, class T = nil_>
struct cons_
{
    using head_ = H;
    using tail_ = T;

    constexpr cons_() {}
    constexpr cons_(H const& x, T const& y) : head(x), tail(y) {}

    head_ head;
    tail_ tail;
};
template <class H, class T = nil_>
constexpr cons_<H,T> cons(H const& h, T const& t = nil)
{ return cons_<H,T>(h,t); }

// Head
template <class L>
using head_ = typename L::head_;
template <class L>
constexpr head_<L> const& head(L const& l) { return l.head; }

// Tail
template <class L>
using tail_ = typename L::tail_;
template <class L>
constexpr tail_<L> const& tail(L const& l) { return l.tail; }

//
// Size
//
template <class List>
constexpr unsigned size(List const& l) { return 0; }

template <class H, class T>
constexpr unsigned size(cons_<H,T> const& l) { return 1 + size(l.tail); }

template <class List>
struct size_
{
    static constexpr unsigned value
#if 0 // awkwardly, you can't do this, even with a hand-coded
      // constexpr declval.  One must pull the value back from
      // compile-time land with a specialization (see below)
    = size(declval<List>());
#else
    = 0;
#endif 
};

template <class H, class T>
struct size_<cons_<H,T> >
{
    static constexpr unsigned value = 1 + size_<T>::value;
};


//
// Iterators - For now we make the assumption that the list is
// homogeneous (except for the terminating nil) and that the aggregate
// layout rules guarantee in-memory compatibility with an array of Ts.
// Attempt to use these iterators when those assumptions are false and
// you get undefined behavior.
//
template <class H, class T>
constexpr auto begin(nil_)
    RETURNS(nullptr);

template <class H, class T>
constexpr auto end(nil_)
    RETURNS(nullptr);

template <class H, class T>
constexpr auto begin(cons_<H,T> const& x)
    RETURNS(&head(x));

template <class H, class T>
constexpr auto end(cons_<H,T> const& x)
    -> H const* { return end(tail(x)); }

template <class H>
constexpr auto end(cons_<H,nil_> const& x)
    RETURNS(begin(x)+1);


//
// Easy list construction
//
constexpr auto list() RETURNS( nil );

template <class T>
constexpr auto list(T x) RETURNS( cons(x, nil) );

template <class... T>
struct list_t;

template <class H, class... T>
struct list_t<H,T...>
{
    using type = cons_<H,typename list_t<T...>::type>;
};
template <class H>
struct list_t<H>
{
    using type = cons_<H,nil_>;
};

#if 0

// It should be this simple, but GCC has a bug: see
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=51501
template <class H, class ...T>
constexpr auto list(H h, T ...args)
    RETURNS(cons( h, list( args... ) ));

#else

// Workaround: list type computation.
template <class... T> struct list_;
template <class H, class... T>
struct list_<H,T...>
{
    using type = cons_<H,typename list_<T...>::type>;
};
template <> struct list_<>
{
    using type = nil_;
};

// Used to prove we're doing the workaround correctly.
template <class T, class U> struct assert_same;
template <class T> struct assert_same<T,T>{};

template <class H, class ...T>
constexpr auto list(H h, T ...args)
    -> typename list_<H,T...>::type
{
    return assert_same<
        decltype(cons( h, list(args...) )),
        typename list_<H,T...>::type
    >(), // <== comma operator
        cons( h, list(args...) );
}

#endif

constexpr auto list0 = list();
constexpr auto list1 = list('a');
constexpr auto list2 = list('a', 2);
constexpr auto list3 = list('a', 2, "booze");


//
// s-expression printing
// 
template <class X>
void print_list_tail(std::ostream& s, X const& x)
{
    s << ". " << x;
}

template <class H>
void print_list_tail(std::ostream& s, cons_<H,nil_> const& x)
{
    s << head(x);
}

template <class H, class T>
void print_list_tail(std::ostream& s, cons_<H,T> const& x)
{
    s << head(x) << " ";
    print_list_tail(s, tail(x));
}

template <class H, class T>
std::ostream& operator<<(std::ostream& s, cons_<H,T> const& x)
{
    s << "( ";
    print_list_tail(s, x);
    return s << " )";
}


//
// Split a list into two parts, the first of which has length n
//

// Unfortunately it seems impossible to make n be a function parameter
// or to fully deduce the result type.  We resort here to old-style
// TMP.
template <unsigned, class List> struct split_;

template <unsigned n, class List>
constexpr auto split(List const& x) RETURNS(split_<n,List>::call(x));

// Handle the case where n != 0
template <unsigned n, class List>
struct split_
{
    using next_result_type = decltype(split<n-1>(tail(std::declval<List>())));

    static constexpr auto
    call2(List const& l, next_result_type const& next)
        RETURNS(
            cons(cons(head(l), head(next)), tail(next))
            );

    static constexpr auto
    call(List const& l)
        RETURNS(
        call2( l, split<(n-1)>(tail(l)) )
        );
};

// Basis case
template <class List>
struct split_<0, List>
{
      static constexpr auto
      call(List const& l) RETURNS( cons(nil,l) );
};

constexpr auto x0 = split<0>(cons(1, cons(2, cons(3, nil))));
constexpr auto x1 = split<1>(cons(1, cons(2, cons(3, nil))));
constexpr auto x2 = split<2>(cons(1, cons(2, cons(3, nil))));


//
// Comparisons
//
template <class T, class U>
constexpr bool less(T const& l, U const& r) { return l < r; }

// string comparison
constexpr bool less(char const*s1, char const*s2)
{
    return *s1 < *s2 || *s1 == *s2 && *s1 && less(s1+1, s2+1);
}

static_assert_(less("foo","fool"));
static_assert_(less("abc","abd"));
static_assert_(!less("foo","foo"));
static_assert_(!less("fool","foo"));
static_assert_(!less("abd","abc"));

//
// Make all strings less than non-strings.  That will allow us to test
// heterogeneous sorting (which doesn't work---see below).
//
template <class L>
constexpr bool less(L const&, char const*)
{
    return false;
}

template <class R>
constexpr bool less(char const*, R const&)
{
    return true;
}

static_assert_(less("foo",33));
static_assert_(!less(22,"abd"));


//
// Merge two sorted lists
//
#if 0
// This version should work, but it looks like GCC has a bug(?)
constexpr auto merge(nil_, nil_) RETURNS(nil);

template <class L2>
constexpr auto merge(nil_, L2 const& l2)
    RETURNS(l2);

template <class L1>
constexpr auto merge(L1 const& l1,nil_)
    RETURNS(l1);

template <class L1, class L2>
constexpr auto merge(L1 const& l1,L2 const& l2)
    RETURNS(
        less(head(l1), head(l2))
        ? cons(head(l1), merge(tail(l1), l2))
        : cons(head(l2), merge(l1, tail(l2))));
#else
template <class L1, class L2>
struct merge_
{
    constexpr static auto call(L1 const& l1,L2 const& l2)
        RETURNS(
            less(head(l1), head(l2))
            ? cons(head(l1), merge(tail(l1), l2))
            : cons(head(l2), merge(l1, tail(l2))));
};

template <class L1>
struct merge_<L1,nil_>
{
    constexpr static auto call(L1 const& l1,nil_) RETURNS(l1);
};

template <class L2>
struct merge_<nil_,L2>
{
    constexpr static auto call(nil_,L2 const& l2) RETURNS(l2);
};

template <>
struct merge_<nil_,nil_>
{
    constexpr static auto call(nil_,nil_) RETURNS(nil);
};

template <class L1, class L2>
constexpr auto merge(L1 const& l1,L2 const& l2)
    RETURNS(merge_<L1,L2>::call(l1,l2));
#endif

constexpr auto x4 = merge(cons(1, nil), cons(2,nil));
constexpr auto x5 = merge(cons(2, nil), cons(1,nil));
assert_type(x4, decltype(x5));


//
// Mergesort implementation
//

// Take a cons of two lists, sort each one, and merge them
template <class Pair>
constexpr auto merge2(Pair const& p)
    RETURNS( merge(sort(head(p)),sort(tail(p))) );

#if 0
// This version should work, but it looks like GCC has a bug
template <class List>
constexpr auto sort(List const& l)
    RETURNS(
        // A list of length <= 1 is already sorted
        size_<List>::value <= 1 ? l
        : merge2(split<(size_<List>::value/2)>(l) ) );
#else
template <class List, bool trivial = (size_<List>::value <= 1)>
struct sort_
{
    constexpr static auto call(List const& l)
        RETURNS( merge2(split<(size_<List>::value/2)>(l) ) );
};

template <class List>
struct sort_<List,true>
{
    constexpr static auto call(List const& l) RETURNS(l);
};

template <class List>
constexpr auto sort(List const& l)
    RETURNS(sort_<List>::call(l));
#endif 

// See test() function below where we print these out.
constexpr auto numbers = sort(list(1, 7, 2, 4, 50, 9, 23, 11, 13, 15, 20, 50));
constexpr auto strings = sort(list("the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"));


// This doesn't work, but ought to be possible in principle:
//
//   constexpr auto numstrings = sort(list(10, "toes", "and", 5, "fat", "fingers"));
//
// Heterogeneous sorting seems to be up against the obstacle of
// crossing the weird "runtime-compile-time" boundary.  The problem is
// that the type you want to return from merge() depends on the
// *values* being merged, and those values aren't usable to compute
// the return type.
//
// I am beginning to doubt there's a way around this issue in the
// world of constexpr.


int main()
{
    std::cout << "length(numbers) = " << (end(numbers) - begin(numbers)) << std::endl;
    std::cout << "length(strings) = " << (end(strings) - begin(strings)) << std::endl;
    std::cout << numbers << std::endl;
    std::cout << strings << std::endl;
    // std::cout << numstrings << std::endl;
    std::cout << std::endl;
      
    for (auto x : strings)
        std::cout << x <<  " ";
    std::cout << std::endl;
      
    std::cout << std::setprecision(100);
    std::cout << "pi is " << pi << std::endl;
    
    // This shows that Gregory-Leibniz doesn't converge very quickly
    // by comparison.  512 is GCC's default maximum constexpr depth.
    std::cout << "pi0 is " << pi0_(511) << std::endl;
}
