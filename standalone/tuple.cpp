#include <utility>

#define RETURNS(...) noexcept(noexcept(decltype(__VA_ARGS__)(std::move(__VA_ARGS__)))) -> decltype(__VA_ARGS__)  { return (__VA_ARGS__); } typedef int RETURNS_CAT(RETURNS_, __LINE__)

// Standard PP concatenation formula
#define RETURNS_CAT_0(x, y) x ## y
#define RETURNS_CAT(x, y) RETURNS_CAT_0(x,y)

// #define RETURNS(...) decltype((__VA_ARGS__)) { return (__VA_ARGS__); }

struct mul_ {
    int operator()() const { return 1; }

    template <typename A>
    A operator()(A const & a) const noexcept(noexcept(A(std::move(a)))) 
    { return a; }

    static mul_ get() noexcept { return mul_(); }

    template <typename A, typename B, typename... C>
    auto operator()(A const & a, B const & b, C const &... c) const
        RETURNS(get()(a * b, c...));
};

constexpr mul_ mul = {}; // global function object

// --- example --------------------------------------------------------

int x0 = mul();              // 1
int x1 = mul(10);            // 10
int x2 = mul(10, -20, 30.0); // -6000.0

template <class ...Ss>
struct append;

// --- type sequences

template <template <class...> class S, class ...T1s>
struct append<S<T1s...> >
    : S<T1s...> {};

template <template <class...> class S, class ...T1s, class ...T2s>
struct append<S<T1s...>, S<T2s...> >
    : S<T1s...,T2s...> {};

template <template <class...> class S, class ...T1s, class ...Ss>
struct append<S<T1s...>, Ss...>
    : append<S<T1s...>, typename append<Ss...>::type> {};

template <class T, class S> struct cons;

template <template <class...> class S, class T, class ...T1s>
struct cons<T, S<T1s...> >
    : S<T,T1s...>
{};

// ---- non-type, non-template sequences

template <template <class T, T...> class S, class T, T ...I1s>
struct append<S<T, I1s...> >
  : S<T, I1s...> {};

template <template <class T, T...> class S, class T, T ...I1s, T ...I2s>
struct append<S<T, I1s...>, S<T, I2s...> >
  : S<T, I1s..., I2s...> {};

template <template <class T, T...> class S, class T, T ...I1s, class ...Ss>
struct append<S<T, I1s...>, Ss...>
  : append<S<T, I1s...>, typename append<Ss...>::type> {};

template <class T, T I, class S> struct cons_c;

template <template <class T, T...> class S, class T, T I, T ...Is>
struct cons_c<T, I, S<T, Is...> >
  : S<T,I,Is...>
{};

// ------

template <class T, T ...Is>
struct vector_c { typedef vector_c type; };

template <class T, T start, T finish>
struct range_c
    : append<vector_c<T,start>, typename range_c<T, start+1, finish>::type>
{};

template <class T, T start_finish>
struct range_c<T, start_finish, start_finish>
  : vector_c<T>
{};

template <template <class...> class S, class ...Ts>
auto indices(S<Ts...> const &) 
  RETURNS(typename range_c<std::size_t, 0, sizeof...(Ts)>::type());

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

#include <tuple>

template <typename F, typename Tuple , std::size_t ...Is>
auto apply_tuple(F f, Tuple const & t, vector_c<std::size_t, Is...>)
  RETURNS(f(std::get<Is>(t)...));

template <typename F, typename ...T>
auto apply_tuple(F f, std::tuple<T...> const & t)
  RETURNS(apply_tuple(f, t, count<sizeof...(T)>()));

// --- example --------------------------------------------------------

int f(int a, int b) { return a + b; }
int x = apply_tuple(f, std::make_tuple(10, 20)); // 30

auto t = std::make_tuple(10, -20, 30.0);
int y = apply_tuple(mul, t);                     // -6000.0
