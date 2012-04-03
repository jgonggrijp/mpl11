// Copyright 2012 Marc.Glisse

// This file is just
// http://geometrica.saclay.inria.fr/team/Marc.Glisse/tmp/sorted.cc,
// reformatted and commented a bit

#include <iostream>
#include <tuple>
#include <utility>
#include <array>

template<int...> struct Indices{};

template<class>
struct Next_increasing_indices;

template<int...I>
struct Next_increasing_indices<Indices<I...> > {
	typedef Indices<I...,sizeof...(I)> type;
};

template<int N>
struct N_increasing_indices
  : Next_increasing_indices<
        typename N_increasing_indices<N-1>::type
    >
{};

template<> struct N_increasing_indices<0> {
    typedef Indices<> type;
};

constexpr bool compare(const char*x, const char*y){
	return (*x==0)?(*y!=0):(*y==0)?false:(*x==*y)?compare(x+1,y+1):(*x<*y);
}
constexpr std::array<const char*,4> x={{"cd","c","ab","ac"}};
constexpr bool c=compare(x[0],x[1]);

// Array-ify a permutation of a tuple.
template<int D, class T, int...I>
constexpr std::array<const char*,D>
g( T t, Indices<I...> )
{
    return {{ std::get<I>(t)... }};
}

// f<D>(n, tuple): array-ify a tuple of length D
template<int D, class T>
constexpr std::array<const char*,D>
f(int N, T t)
{
    return g<D>(t, typename N_increasing_indices<D>::type());
}

// f<D>(n, tuple, elt): array-ify cat(tuple, tuple(elt))
template<int D,class T>
constexpr std::array<const char*,D>
f(int N, T t, const char* mini)
{
    return f<D>(
        0, std::tuple_cat(t,std::make_tuple(mini)));
}

// f<D>(n, tuple, e0, e1, ...):
//
// Bubble sort.
template<int D,class T,class...U>
constexpr std::array<const char*,D>
f(int N, T t, const char* mini, const char* u0, U...u)
{
    return (N==sizeof...(U)+1)
        ? f<D>(
            0, std::tuple_cat(
                t, std::make_tuple(mini))
            ,u0 , u...
            )
        : compare(mini,u0)
          ? f<D>(N+1, t, mini, u..., u0  )
          : f<D>(N+1, t, u0,   u..., mini);
}

template<class...U>
constexpr std::array<const char*,sizeof...(U)>
make_sorted_array(U...u)
{
    return f<sizeof...(U)>(0,std::make_tuple(),u...);
}

constexpr auto y=make_sorted_array("cd","c","ab","ac");

int main(){
    for(auto s:y) std::cout << s << '\n';
}
