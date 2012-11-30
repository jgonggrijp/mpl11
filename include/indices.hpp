// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef INDICES_DWA20121129_HPP
# define INDICES_DWA20121129_HPP

# include "assert_same.hpp"

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

#endif // INDICES_DWA20121129_HPP
