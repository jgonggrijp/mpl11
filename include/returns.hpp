// Copyright Dave Abrahams 2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef RETURNS_DWA201243_HPP
# define RETURNS_DWA201243_HPP

#include <utility>

#define RETURNS(...)                                                    \
    noexcept(noexcept(decltype(__VA_ARGS__)(std::move(__VA_ARGS__))))   \
    -> decltype(__VA_ARGS__)                                            \
    { return (__VA_ARGS__); }                                           \
    static_assert(true, "") /* makes uses accept a trailing ";" */
    
#endif // RETURNS_DWA201243_HPP
