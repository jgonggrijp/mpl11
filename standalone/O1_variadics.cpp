// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Placeholders for variadic template trix
enum place { _ };
template <place...>
struct places {};

// Glue two specializations of places together
template <class P1, class P2> struct append_places;

template <place...X1, place...X2>
struct append_places<places<X1...>, places<X2...> >
{
    typedef places<X1...,X2...> type;
};

//
// Generate places<_,_,_,..._> with N arguments in O(log N)
//
template <unsigned N>
struct make_places
  : append_places<
        typename make_places<N/2>::type, 
        typename make_places<N-N/2>::type
    >
{};

template <>
struct make_places<0>
{
    typedef places<> type;
};

template <>
struct make_places<1>
{
    typedef places<_> type;
};

//
// Low-rent assertions
//
template <class T, class U> struct assert_same;
template <class T> struct assert_same<T,T> {};

namespace check_places
{
  assert_same<make_places<2>::type, places<_,_> > a2;
  assert_same<make_places<3>::type, places<_,_,_> > a3;
  assert_same<make_places<4>::type, places<_,_,_,_> > a4;
  assert_same<make_places<5>::type, places<_,_,_,_,_> > a5;
}

// Wrapper to prevent type decay
template <class T>
struct no_decay
{
    typedef T type;
};

// Use as function parameter that eats any POD
template <place ignored>
struct eat
{ eat(...); };

// inner beauty
template <class T> struct nth_type_impl;

template <place...X>
struct nth_type_impl<places<X...> >
{
    template <class U, class...Tail>
    static U result(eat<X>..., U*, Tail*...);
};

// Select the Nth type in O(1) (once all the necessary places<...> have
// been instantiated, which is O(log N) but happens only once.
template <unsigned N, class...T>
struct nth_type
  : decltype(
      nth_type_impl<typename make_places<N>::type>::result(
          (no_decay<T>*)0 ...))
{};

template <class...T> struct types
{
    typedef types type;
};

// inner beauty
template <class T> struct drop_impl;

template <place...X>
struct drop_impl<places<X...> >
{
    template <class...Tail>
    static types<Tail...> result(eat<X>..., no_decay<Tail>*...);
};

template <unsigned N, class...T>
struct drop
  : decltype(
      drop_impl<typename make_places<N>::type>::result(
          (no_decay<T>*)0 ...))
{};


int main()
{
    {  assert_same<nth_type<0,void(int),char[3],long>::type, void(int)> a; }
    {  assert_same<nth_type<1,void(int),char[3],long>::type, char[3]> a; }
    {  assert_same<nth_type<2,void(int),char[3],long>::type, long> a; }

    {  assert_same<
           drop<0,void(int),char[3],long>::type,
           types<void(int),char[3],long> > a; }
    
    {  assert_same<
           drop<1,void(int),char[3],long>::type,
           types<char[3],long> > a; }
    
    {  assert_same<
           drop<2,void(int),char[3],long>::type,
           types<long> > a; }
    
    {  assert_same<
           drop<3,void(int),char[3],long>::type,
           types<> > a; }
}
