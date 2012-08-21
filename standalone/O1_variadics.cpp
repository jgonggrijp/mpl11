// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//
//   assert_same -- a low-rent assertion
//__________________________________________________________
template <class T, class U> struct assert_same;
template <class T> struct assert_same<T,T> {};

//
//   places -- a wrapper for "N variadic parameters"
//__________________________________________________________

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

// Generate places<_,_,_,..._> with N arguments in O(log N)
template <unsigned N>
struct make_places
  : append_places<
        typename make_places<N/2>::type, 
        typename make_places<N-N/2>::type
    >
{};

template <> struct make_places<0> { typedef places<> type; };
template <> struct make_places<1> { typedef places<_> type; };

inline void test_places()
{
  assert_same<make_places<2>::type, places<_,_> > a2;
  assert_same<make_places<3>::type, places<_,_,_> > a3;
  assert_same<make_places<4>::type, places<_,_,_,_> > a4;
  assert_same<make_places<5>::type, places<_,_,_,_,_> > a5;
}

//
//   types -- a basic type sequence
//__________________________________________________________
template <class...T> struct types
{
    typedef types type;
};

//
//   nth_type -- get the Nth type in a sequence in O(1)
//__________________________________________________________

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
template <unsigned N, class S> struct nth_type;

template <unsigned N, class...T>
struct nth_type<N, types<T...> >
  : decltype(
      nth_type_impl<typename make_places<N>::type>::result(
          (no_decay<T>*)0 ...))
{};

inline void test_nth_type()
{
    using seq = types<void(int),char[3],long>;
    
    {  assert_same<nth_type<0,seq>::type, void(int)> a; }
    {  assert_same<nth_type<1,seq>::type, char[3]> a; }
    {  assert_same<nth_type<2,seq>::type, long> a; }
}
    
//
//   drop -- drop N elements from the front of a type sequence in O(1)
//______________________________________________________________________


// inner beauty
template <class T> struct drop_impl;

template <place...X>
struct drop_impl<places<X...> >
{
    template <class...Tail>
    static types<Tail...> result(eat<X>..., no_decay<Tail>*...);
};

template <unsigned N, class S> struct drop;

template <unsigned N, class...T>
struct drop<N, types<T...> >
  : decltype(
      drop_impl<typename make_places<N>::type>::result(
          (no_decay<T>*)0 ...))
{};

inline void test_drop()
{
    using seq = types<void(int),char[3],long>;
    
    assert_same<drop<0,seq>::type, types<void(int),char[3],long> > a0;
    assert_same<drop<1,seq>::type, types<char[3],long> > a1;
    assert_same<drop<2,seq>::type, types<long> > a2;
    assert_same<drop<3,seq>::type, types<> > a3;
}

//
//   indices -- a sequence of unsigned integers
//__________________________________________________________
template <unsigned...N> struct indices
{
    typedef indices type;
};

// Glue two sets of indices together
template <class I1, class I2> struct append_indices;

template <unsigned...N1, unsigned...N2>
struct append_indices<indices<N1...>, indices<N2...> >
{
    typedef indices<N1..., (sizeof...(N1)+N2)...> type;
};

// generate indices [0,N) in O(log(N)) time
template <unsigned N>
struct make_indices
  : append_indices<
        typename make_indices<N/2>::type
      , typename make_indices<N-N/2>::type
    >
{};
template <> struct make_indices<0> { typedef indices<> type; };
template <> struct make_indices<1> { typedef indices<0> type; };


inline void test_make_indices()
{
    assert_same<make_indices<12>::type, indices<0,1,2,3,4,5,6,7,8,9,10,11> > a0;
}

//
//   take -- return the first N types in the sequence
//   Note: this is an O(N) algorithm.
//___________________________________________________________
template <class I, class S> struct take_impl;

template <unsigned...N, class S>
struct take_impl<indices<N...>, S>
{
    typedef types<typename nth_type<N,S>::type...> type;
};

template <unsigned N, class S>
struct take
  : take_impl<typename make_indices<N>::type, S>
{};

inline void test_take()
{
    using seq = types<void(int),char[3],long,double>;
    
    assert_same<take<0,seq>::type, types<> > a0;
    assert_same<take<1,seq>::type, types<void(int)> > a1;
    assert_same<take<2,seq>::type, types<void(int),char[3]> > a2;
    assert_same<take<3,seq>::type, types<void(int),char[3],long> > a3;
    assert_same<take<4,seq>::type, seq> a4;
}

int main()
{
}
