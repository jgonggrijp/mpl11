// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ASSERT_SAME_DWA20121129_HPP
# define ASSERT_SAME_DWA20121129_HPP
//
//   assert_same -- a low-rent assertion
//__________________________________________________________
template <class T, class U> struct assert_same;
template <class T> struct assert_same<T,T> {};

#endif // ASSERT_SAME_DWA20121129_HPP
