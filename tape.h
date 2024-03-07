#pragma once
#include "helpers.h"

namespace turing_machine {
  namespace semitape {
    template <typename Tape, typename T>
    struct is_semitape : std::bool_constant<false> {};

    template <typename T>
    struct empty : helpers::type_constant<T> {};

    template <typename T, T val, typename Next = empty<T>>
      requires is_semitape<Next, T>::value && std::is_default_constructible_v<T>
    struct semitape : helpers::type_constant<T> {};

    template <typename T>
    struct is_semitape<empty<T>, T> : std::bool_constant<true> {};

    template <typename T, T val, typename Next>
    struct is_semitape<semitape<T, val, Next>, T> : std::bool_constant<is_semitape<Next, T>::value> {};

    template <typename semitape, typename T>
    constexpr bool is_semitape_v = is_semitape<semitape, T>::value;

    //make_semitape

    template <typename T, T...>
    struct make_semitape : helpers::type_constant<empty<T>> {};

    template <typename T, T arg, T... args>
    struct make_semitape<T, arg, args
                         ...> : helpers::type_constant<semitape<T, arg, typename make_semitape<T, args...>::type>> {};

    template <typename T, T... args>
    using make_semitape_t = typename make_semitape<T, args...>::type;

    //print

    template <typename T, T arg, typename Prev>
    constexpr void print_prev(semitape<T, arg, Prev>, std::ostream& out) {
      print_prev(Prev{}, out);
      out << arg << " ";
    }

    template <typename T>
    constexpr void print_prev(empty<T>, std::ostream&) {}

    template <typename T, T arg, typename Next>
    constexpr void print_next(semitape<T, arg, Next>, std::ostream& out) {
      out << arg << " ";
      print_next(Next{}, out);
    }

    template <typename T>
    constexpr void print_next(empty<T>, std::ostream&) {}
  }

  namespace tape {
    //tape

    template <typename T, T val, typename Prev = semitape::empty<T>, typename Next = semitape::empty<T>>
      requires semitape::is_semitape_v<Prev, T> && semitape::is_semitape_v<Next, T>
    struct tape : helpers::type_constant<T> {};

    template <typename T>
    using empty = tape<T, T{}>;

    //is_tape

    template <typename Tape, typename T>
    struct is_tape : std::bool_constant<false> {};

    template <typename T, T val, typename Prev, typename Next>
    struct is_tape<tape<T, val, Prev, Next>, T> : std::bool_constant<semitape::is_semitape_v<Prev, T> &&
                                                                     semitape::is_semitape_v<Next, T>> {};

    template <typename Tape, typename T>
    constexpr bool is_tape_v = is_tape<Tape, T>::value;

    //next

    template <typename Tape>
    struct next;

    template <typename T, T val, T next_val, typename Prev, typename Next>
    struct next<tape<T, val, Prev, semitape::semitape<T, next_val, Next>>> : helpers::type_constant<tape<
          T, next_val, semitape::semitape<T, val, Prev>, Next>> {};

    template <typename T, T val, typename Prev>
    struct next<tape<T, val, Prev>> : helpers::type_constant<tape<T, T{}, semitape::semitape<T, val, Prev>>> {};

    template <typename T>
    using next_t = typename next<T>::type;

    //prev

    template <typename Tape>
    struct prev;

    template <typename T, T val, T prev_val, typename Prev, typename Next>
    struct prev<tape<T, val, semitape::semitape<T, prev_val, Prev>, Next>> : helpers::type_constant<tape<
          T, prev_val, Prev, semitape::semitape<T, val, Next>>> {};

    template <typename T, T val, typename Next>
    struct prev<tape<T, val, semitape::empty<T>, Next>> : helpers::type_constant<tape<
          T, T{}, semitape::empty<T>, semitape::semitape<T, val, Next>>> {};

    template <typename T>
    using prev_t = typename prev<T>::type;

    //get

    template <typename Tape, typename T>
    struct get;

    template <typename T, T val, typename Prev, typename Next>
    struct get<tape<T, val, Prev, Next>, T> : std::integral_constant<T, val> {};

    template <typename Tape, typename T>
    constexpr T get_v = get<Tape, T>::value;

    //set

    template <typename Tape, typename T, T = {}>
    struct set;

    template <typename T, T val, T new_val, typename Prev, typename Next>
    struct set<tape<T, val, Prev, Next>, T, new_val> : helpers::type_constant<tape<T, new_val, Prev, Next>> {};

    template <typename Tape, typename T, T val = T{}>
    using set_t = typename set<Tape, T, val>::type;

    //make_tape

    template <typename T, T...>
    struct make_tape : helpers::type_constant<semitape::empty<T>> {};

    template <typename T, T arg, T... args>
    struct make_tape<T, arg, args...> : helpers::type_constant<tape<
          T, arg, semitape::empty<T>, semitape::make_semitape_t<T, args...>>> {};

    template <typename T, T... args>
    using make_tape_t = typename make_tape<T, args...>::type;

    //print

    template <typename T, T arg, typename Prev, typename Next>
    constexpr void print(tape<T, arg, Prev, Next>, std::ostream& out) {
      print_prev(Prev{}, out);
      out << "(" << arg << ") ";
      print_next(Next{}, out);
      out << "\n";
    }
  }
}