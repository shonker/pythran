#ifndef PYTHONIC_INCLUDE_TYPES_COMBINED_HPP
#define PYTHONIC_INCLUDE_TYPES_COMBINED_HPP

#include "pythonic/include/types/traits.hpp"
#include "pythonic/include/types/variant_functor.hpp"

/* special handling for functors
 * as it's based on a trait, template specialization cannot be used
 * so we rely on operator+ specialization
 * { */
template <class T0, class T1>
typename std::enable_if<pythonic::types::is_callable<T0>::value and
                            pythonic::types::is_callable<T1>::value,
                        pythonic::types::variant_functor<T0, T1>>::type
operator+(T0, T1);

template <class T, class... Types>
pythonic::types::variant_functor<T, Types...>
operator+(T, pythonic::types::variant_functor<Types...>);

template <class T, class... Types>
pythonic::types::variant_functor<T, Types...>
operator+(pythonic::types::variant_functor<Types...>, T);

template <class... Types0, class... Types1>
pythonic::types::variant_functor<Types0..., Types1...>
operator+(pythonic::types::variant_functor<Types0...>,
          pythonic::types::variant_functor<Types1...>);

template <class T>
typename std::enable_if<pythonic::types::is_callable<T>::value, T>::type
operator+(T, T);

/* } */

/* specialize remove_cv */
namespace std
{

  template <class K, class V>
  struct remove_cv<std::pair<const K, V>> {
    using type = std::pair<K, V>;
  };
}
/* specialize remove_cv */

/* type inference stuff
*/

template <class T0, class T1, class... Types>
struct __combined {
  using type =
      typename __combined<T0, typename __combined<T1, Types...>::type>::type;
};

template <class T0, class T1>
struct __combined<T0, T1> {
  using type = decltype(std::declval<T0>() + std::declval<T1>());
};

template <class T0, class T1>
struct __combined<const T0, T1> {
  using type = typename std::add_const<typename __combined<T0, T1>::type>::type;
};

template <class T0, class T1>
struct __combined<T0, const T1> {
  using type = typename std::add_const<typename __combined<T0, T1>::type>::type;
};

template <class T0, class T1>
struct __combined<T0 &, T1> {
  using type = typename __combined<T0, T1>::type;
};

template <class T0, class T1>
struct __combined<T0, T1 &> {
  using type = typename __combined<T0, T1>::type;
};

template <class T0, class T1>
struct __combined<T0 &, T1 &> {
  using type = typename std::add_lvalue_reference<
      typename __combined<T0, T1>::type>::type;
};

template <class T0, class T1>
struct __combined<const T0, const T1> {
  using type = typename std::add_const<typename __combined<T0, T1>::type>::type;
};

template <class T0, class T1>
struct __combined<const T0 &, const T1 &> {
  using type = typename std::add_lvalue_reference<
      typename std::add_const<typename __combined<T0, T1>::type>::type>::type;
};

template <class T>
class container
{
public:
  using value_type =
      typename std::remove_cv<typename std::remove_reference<T>::type>::type;

private:
  container();
};

template <class K, class V>
class indexable_container
{
public:
  using key_type =
      typename std::remove_cv<typename std::remove_reference<K>::type>::type;
  using value_type =
      typename std::remove_cv<typename std::remove_reference<V>::type>::type;

private:
  indexable_container();
};

template <class T>
class dict_container
{
public:
  using value_type =
      typename std::remove_cv<typename std::remove_reference<T>::type>::type;

private:
  dict_container();
};

template <class T>
class indexable
{
public:
  using type =
      typename std::remove_cv<typename std::remove_reference<T>::type>::type;

private:
  indexable();
};

template <class T>
class indexable_dict
{
public:
  using type =
      typename std::remove_cv<typename std::remove_reference<T>::type>::type;

private:
  indexable_dict();
};

template <class K0, class V0, class K1, class V1>
struct __combined<indexable_container<K0, V0>, indexable_container<K1, V1>> {
  using type = indexable_container<typename __combined<K0, K1>::type,
                                   typename __combined<V0, V1>::type>;
};

template <class K, class V>
struct __combined<indexable<K>, indexable<V>> {
  using type = indexable<typename __combined<K, V>::type>;
};

template <class K, class V>
struct __combined<indexable<K>, container<V>> {
  using type = indexable_container<K, V>;
};

template <class V, class K>
struct __combined<container<V>, indexable<K>> {
  using type = indexable_container<K, V>;
};

template <class K, class V, class W>
struct __combined<indexable_container<K, V>, container<W>> {
  using type = indexable_container<K, typename __combined<V, W>::type>;
};

template <class V, class K, class W>
struct __combined<container<W>, indexable_container<K, V>> {
  using type = indexable_container<K, typename __combined<V, W>::type>;
};

template <class K1, class V1, class K2>
struct __combined<indexable_container<K1, V1>, indexable<K2>> {
  using type = indexable_container<typename __combined<K1, K2>::type, V1>;
};

template <class K1, class V1, class K2>
struct __combined<indexable<K2>, indexable_container<K1, V1>> {
  using type = indexable_container<typename __combined<K1, K2>::type, V1>;
};

template <class A, class B>
struct __combined<container<A>, container<B>> {
  using type = container<typename __combined<A, B>::type>;
};

#endif
