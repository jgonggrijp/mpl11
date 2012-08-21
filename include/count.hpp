// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef COUNT_DWA201243_HPP
# define COUNT_DWA201243_HPP

# include "./cons.hpp"

template <std::size_t N>
struct count_
  : cons_c<std::size_t, N-1, typename count_<N-1>::type>
{};

template <>
struct count_<0>
  : vector_c<std::size_t>
{};

template <std::size_t N>
using count = typename count_<N>::type;

#endif // COUNT_DWA201243_HPP
