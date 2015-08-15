// Copyright: 2014, 2015, Ableton AG, Berlin. All rights reserved.
/*!
 * @file
 */

#pragma once

#include <atria/xform/transducer_impl.hpp>
#include <atria/xform/maybe_reduced.hpp>
#include <atria/xform/skip.hpp>
#include <atria/estd/functional.hpp>

namespace atria {
namespace xform {

namespace detail {

struct take_while_rf_gen
{
  template <typename ReducingFnT,
            typename PredicateT>
  struct apply
  {
    ReducingFnT step;
    PredicateT predicate;

    template <typename StateT, typename ...InputTs>
    auto operator() (StateT&& s, InputTs&& ...is)
      -> maybe_reduced<estd::decay_t<decltype(
      call(step, state_unwrap(std::forward<StateT>(s)),
           std::forward<InputTs>(is)...))> >
    {
      return estd::invoke(predicate, is...)
          ? not_reduced(call(step, state_unwrap(std::forward<StateT>(s)),
                             std::forward<InputTs>(is)...))
          : reduced(skip(step, state_unwrap(std::forward<StateT>(s)),
                         std::forward<InputTs>(is)...));
    }
  };
};

} // namespace detail

template <typename T>
using take_while_t = transducer_impl<detail::take_while_rf_gen, T>;

/*!
 * Similar to clojure.core/take_while$1
 */
template <typename PredicateT>
auto take_while(PredicateT&& p)
  -> take_while_t<estd::decay_t<PredicateT> >
{
  return take_while_t<estd::decay_t<PredicateT> > {
    std::forward<PredicateT>(p) };
}

} // namespace xform
} // namespace atria
