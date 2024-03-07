#pragma once

#include "../lib/turing-machine.h"
#include <ostream>

namespace ttm::debug {

  template <typename T>
  struct to_str {
    static constexpr std::string value = T::to_str;
  };

  template <>
  struct to_str<tape::zero> {
    static constexpr std::string value = "*";
  };

  template <typename T>
  constexpr std::string to_str_v = to_str<T>::value;

  template <typename Semitape>
  constexpr void print_semitape_reversed(std::ostream& out) {
    if constexpr (!std::is_same_v<details::semitape::empty, Semitape>) {
      print_semitape_reversed<details::semitape::get_next_t<Semitape>>(out);
      out << to_str_v<details::semitape::get_t<Semitape>> << " ";
    }
  }

  template <typename Semitape>
  constexpr void print_semitape_direct(std::ostream& out) {
    if constexpr (!std::is_same_v<details::semitape::empty, Semitape>) {
      out << to_str_v<details::semitape::get_t<Semitape>> << " ";
      print_semitape_direct<details::semitape::get_next_t<Semitape>>(out);
    }
  }

  template <typename Tape>
  constexpr void print_tape(std::ostream& out) {
    print_semitape_reversed<details::tape::get_prev_t<Tape>>(out);
    out << "(" << to_str_v<tape::get_t<Tape>> << ") ";
    print_semitape_direct<details::tape::get_next_t<Tape>>(out);
    out << "\n";
  }

  template <typename TuringMachine>
  constexpr void print_tm(std::ostream& out) {
    out << "state: " << turing_machine::get_state_t<TuringMachine>::to_str << "\n";
    out << "tape: ";
    print_tape<turing_machine::get_tape_t<TuringMachine>>(out);
  }

  template <typename TuringMachine, typename Rules, typename FinalStates>
  constexpr void apply_until(std::ostream& out) {
    print_tm<TuringMachine>(out);
    out << "--------------------------\n";
    if constexpr (!details::pack::pack_contains_v<FinalStates, turing_machine::get_state_t<TuringMachine>>) {
      apply_until<turing_machine::apply_t<TuringMachine, Rules>, Rules, FinalStates>(out);
    }
  }
}