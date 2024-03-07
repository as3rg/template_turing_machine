#pragma once

#include "tape.h"

namespace turing_machine {
  namespace moves {
    enum class moves {
      LEFT,
      RIGHT,
      STAY
    };

    template <typename Tape, moves move>
    struct make_move : helpers::type_constant<Tape> {};

    template <typename Tape>
    struct make_move<Tape, moves::LEFT> : helpers::type_constant<tape::prev_t<Tape>> {};

    template <typename Tape>
    struct make_move<Tape, moves::RIGHT> : helpers::type_constant<tape::next_t<Tape>> {};

    template <typename Tape, moves move>
    using make_move_t = typename make_move<Tape, move>::type;
  }

  namespace rules {
    template <typename TapeType, typename StateType, TapeType from_tape, StateType from_state, TapeType to_tape,
              StateType to_state,
              moves::moves move>
    struct rule {};

    template <typename Rule>
    struct is_rule : std::bool_constant<false> {};

    template <typename TapeType, typename StateType, TapeType from_tape, StateType from_state, TapeType to_tape,
              StateType to_state, moves::moves move>
    struct is_rule<rule<TapeType, StateType, from_tape, from_state, to_tape,
                        to_state, move>> : std::bool_constant<true> {};

    template <typename Rule>
    constexpr bool is_rule_t = is_rule<Rule>::value;


    template <typename... Rules>
      requires (true && ... && is_rule_t<Rules>)
    struct rules {};
  }

  namespace turing_machine {
    template <typename TapeType, typename StateType, typename Tape, StateType state>
      requires tape::is_tape_v<Tape, TapeType>
    struct turing_machine {};

    // get_state

    template <typename TuringMachine>
    struct get_state;

    template <typename TapeType, typename StateType, typename Tape, StateType state>
    struct get_state<turing_machine<TapeType, StateType, Tape, state>> : std::integral_constant<StateType, state> {};

    template <typename TuringMachine>
    constexpr auto get_state_v = get_state<TuringMachine>::value;

    // apply

    template <typename TuringMachine, typename Rules>
    struct apply;

    template <typename TapeType, typename StateType, TapeType from_tape, StateType from_state, TapeType to_tape,
              StateType to_state, typename Tape, typename... Rules, moves::moves move>
      requires (tape::get_v<Tape, TapeType> == from_tape)
    struct apply<turing_machine<TapeType, StateType, Tape, from_state>,
                 rules::rules<rules::rule<TapeType, StateType, from_tape, from_state, to_tape, to_state, move>, Rules
                              ...>> :
        helpers::type_constant<
          turing_machine<TapeType, StateType, moves::make_move_t<tape::set_t<Tape, TapeType, to_tape>, move>,
                         to_state>> {};

    template <typename TuringMachine, typename Rule, typename... Rules>
    struct apply<TuringMachine, rules::rules<
                   Rule, Rules...>> : helpers::type_constant<typename apply<
          TuringMachine, rules::rules<Rules...>>::type> {};

    template <typename TuringMachine, typename Rules>
    using apply_t = typename apply<TuringMachine, Rules>::type;

    //apply_until

    template <typename StateType, StateType... states>
    using state_list = helpers::pack::pack<StateType, states...>;

    template <typename TuringMachine, typename Rules, typename StateType, typename FinalStates>
    struct apply_until : helpers::type_constant<typename apply_until<
          apply_t<TuringMachine, Rules>, Rules, StateType, FinalStates>::type> {};

    template <typename TuringMachine, typename Rules, typename StateType, typename FinalStates>
      requires helpers::pack::pack_contains_v<StateType, FinalStates, get_state_v<TuringMachine>>
    struct apply_until<TuringMachine, Rules, StateType, FinalStates> : helpers::type_constant<TuringMachine> {};

    template <typename TuringMachine, typename Rules, typename StateType, typename FinalStates>
    using apply_until_t = typename apply_until<TuringMachine, Rules, StateType, FinalStates>::type;

    //print

    template <typename TapeType, typename StateType, typename Tape, StateType state>
    constexpr void print(turing_machine<TapeType, StateType, Tape, state>, std::ostream& out) {
      out << "state: " << state << "\n";
      out << "tape: ";
      print(Tape{}, out);
    }
  }
}