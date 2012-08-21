#include "../include/returns.hpp"
#include "../include/cons.hpp"
#include "../include/vector.hpp"

template <class T, std::size_t n, std::size_t ...Is>
constexpr auto array2vec(T const (&x)[n], vector_c<std::size_t, Is...>) 
    RETURNS(vector_c<T, x[Is]...>);

int z = array2vec("foo", vector_c<std::size_t, 0, 1, 2>());

int main(){}
