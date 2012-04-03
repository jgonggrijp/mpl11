
template <class ...Ts> struct vector
{
    typedef vector type;
};

template <class ...Ts> struct pair
{
    typedef pair type;
};

template <class S> struct head_;
template <class S> struct tail_;

template <class T, class ...Ts>
struct head_<vector<T,Ts...>>
{
    typedef T type;
};

template <class T, class ...Ts>
struct tail_<vector<T,Ts...>>
{
    typedef vector<Ts...> type;
};

template <class S>
using head = typename head_<S>::type;

template <class S>
using tail = typename tail_<S>::type;

template <class T, class S>
struct cons_;
template <class T, class S>
using cons = typename cons_<T,S>::type;
template <class T, class ...Ts>
struct cons_<T, vector<Ts...> >
  : vector<T,Ts...>
{};

template <class ...Ss>
struct append_;
template <class ...Ss>
using append = typename append_<Ss...>::type;


template <class ...T1s>
struct append_<vector<T1s...> >
    : vector<T1s...> {};

template <class ...T1s, class ...T2s>
struct append_<vector<T1s...>, vector<T2s...> >
    : vector<T1s...,T2s...> {};

template <class ...T1s, class ...Ss>
struct append_<vector<T1s...>, Ss...>
    : append_<vector<T1s...>, append<Ss...> > {};

template <class S1, class S2>
struct product_;
template <class S1, class S2>
using product = typename product_<S1,S2>::type;

template <class T1, class ...T1s, class T2, class ...T2s>
struct product_<vector<T1,T1s...>, vector<T2, T2s...> >
  : append_<
        vector<
             pair<T1,T2>
           , pair<T1,T2s>...
           , pair<T1s,T2>...
        >
    , product<vector<T1s...>, vector<T2s...> > >
{};

template <class S>
struct product_<S, vector<> > : vector<> {};

template <class S>
struct product_<vector<>,S > : vector<> {};

template <>
struct product_<vector<>,vector<> > : vector<> {};

struct A {};
struct B {};
struct C {};

struct D {};
struct E {};
struct F {};

product<vector<A>,vector<D> > x0 = 1;
product<vector<A,B>,vector<D> > x1 = 1;
product<vector<A>,vector<D,E> > x2 = 1;
product<vector<A,B>,vector<D,E> > x3 = 1;
product<vector<A,B,C>,vector<D,E,F> > x = 1;
