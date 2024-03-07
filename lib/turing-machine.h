#pragma once

#include "tape.h"

namespace ttm {
  namespace details::rules {
    struct same;
    struct any;
  }

  namespace details::moves {
    enum class moves {
      LEFT,
      RIGHT,
      STAY
    };

    template <typename Tape, moves move>
    struct make_move : type_constant<Tape> {};

    template <typename Tape>
    struct make_move<Tape, moves::LEFT> : type_constant<ttm::tape::prev_t<Tape>> {};

    template <typename Tape>
    struct make_move<Tape, moves::RIGHT> : type_constant<ttm::tape::next_t<Tape>> {};

    template <typename Tape, moves move>
    using make_move_t = typename make_move<Tape, move>::type;
  }

  namespace details::rules {
    struct any;
    struct same;

    template <typename... States>
    using state_list = pack::pack<States...>;

    template <typename T>
    constexpr bool is_state_v = !std::is_same_v<any, T> && !std::is_same_v<same, T>;

    template <typename T>
    constexpr bool is_tape_value_v = !std::is_same_v<any, T> && !std::is_same_v<same, T>;

    template <typename FromTape, typename FromState, typename ToTape, typename ToState, moves::moves move>
      requires (is_tape_value_v<FromTape> || std::is_same_v<FromTape, any>) &&
               (is_tape_value_v<ToTape> || std::is_same_v<ToTape, same>) &&
               (is_state_v<FromState> || std::is_same_v<FromState, any>) &&
               (is_state_v<ToState> || std::is_same_v<ToState, same>)
    struct rule {};

    template <typename Rule>
    struct is_rule : std::bool_constant<false> {};

    template <typename FromTape, typename FromState, typename ToTape, typename ToState, moves::moves move>
    struct is_rule<rule<FromTape, FromState, ToTape, ToState, move>> : std::bool_constant<true> {};

    template <typename Rule>
    constexpr bool is_rule_t = is_rule<Rule>::value;

    template <typename... Rules>
      requires (true && ... && is_rule_t<Rules>)
    struct rules {};

    //apply_same
    template <typename Value, typename New>
    struct apply_same : type_constant<New> {};

    template <typename Value>
    struct apply_same<Value, same> : type_constant<Value> {};

    template <typename Tape, typename New>
    using apply_same_t = typename apply_same<Tape, New>::type;
  }

  namespace turing_machine {
    using details::moves::moves;
    using details::rules::rule;
    using details::rules::rules;
    using details::rules::any;
    using details::rules::same;
    using details::rules::state_list;

    template <typename Tape, typename State>
      requires details::tape::is_tape_v<Tape> && details::rules::is_state_v<State>
    struct turing_machine {};

    // get_state
    template <typename TuringMachine>
    struct get_state;

    template <typename Tape, typename State>
    struct get_state<turing_machine<Tape, State>> : details::type_constant<State> {};

    template <typename TuringMachine>
    using get_state_t = typename get_state<TuringMachine>::type;

    // get_tape
    template <typename TuringMachine>
    struct get_tape;

    template <typename Tape, typename State>
    struct get_tape<turing_machine<Tape, State>> : details::type_constant<Tape> {};

    template <typename TuringMachine>
    using get_tape_t = typename get_tape<TuringMachine>::type;

    // apply
    template <typename TuringMachine, typename Rules>
    struct apply;

    template <typename Tape, typename State, typename FromTape, typename FromState, typename ToTape, typename ToState,
              moves move, typename... Rules>
      requires (std::is_same_v<tape::get_t<Tape>, FromTape> || std::is_same_v<FromTape, any>) &&
               (std::is_same_v<State, FromState> || std::is_same_v<FromState, any>)
    struct apply<turing_machine<Tape, State>, rules<rule<FromTape, FromState, ToTape, ToState, move>, Rules...>>
        : details::type_constant<turing_machine<
          details::moves::make_move_t<tape::set_t<Tape, details::rules::apply_same_t<tape::get_t<Tape>, ToTape>>, move>,
          details::rules::apply_same_t<State, ToState>>> {};

    template <typename TuringMachine, typename Rule, typename... Rules>
    struct apply<TuringMachine, rules<Rule, Rules...>>
        : details::type_constant<typename apply<TuringMachine, rules<Rules...>>::type> {};

    template <typename TuringMachine, typename Rules>
    using apply_t = typename apply<TuringMachine, Rules>::type;

    //apply_until

    template <typename TuringMachine, typename Rules, typename FinalStates>
    struct apply_until
        : details::type_constant<typename apply_until<apply_t<TuringMachine, Rules>, Rules, FinalStates>::type> {};

    template <typename TuringMachine, typename Rules, typename FinalStates>
      requires details::pack::pack_contains_v<FinalStates, get_state_t<TuringMachine>>
    struct apply_until<TuringMachine, Rules, FinalStates> : details::type_constant<TuringMachine> {};

    template <typename TuringMachine, typename Rules, typename FinalStates>
    using apply_until_t = typename apply_until<TuringMachine, Rules, FinalStates>::type;
  }
}