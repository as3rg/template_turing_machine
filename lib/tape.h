#pragma once
#include "helpers.h"

namespace ttm {
  namespace details::semitape {
    // values
    struct zero;

    // semitapes
    struct empty {};

    template <typename Tape>
    struct is_semitape : std::bool_constant<false> {};

    template <typename T, typename Next = empty>
      requires is_semitape<Next>::value
    struct semitape : type_constant<T> {};

    template <>
    struct is_semitape<empty> : std::bool_constant<true> {};

    template <typename T, typename Next>
    struct is_semitape<semitape<T, Next>> : std::bool_constant<is_semitape<Next>::value> {};

    template <typename Semitape>
    constexpr bool is_semitape_v = is_semitape<Semitape>::value;

    //get_next
    template <typename Semitape>
    struct get_next;

    template <typename T, typename Next>
    struct get_next<semitape<T, Next>> : type_constant<Next> {};

    template <typename Semitape>
    using get_next_t = typename get_next<Semitape>::type;

    //get
    template <typename Semitape>
    struct get;

    template <typename T, typename Next>
    struct get<semitape<T, Next>> : type_constant<T> {};

    template <typename Semitape>
    using get_t = typename get<Semitape>::type;

    //make_semitape
    template <typename... T>
    struct make_semitape : type_constant<empty> {};

    template <typename T, typename... Ts>
    struct make_semitape<T, Ts...> : type_constant<semitape<T, typename make_semitape<Ts...>::type>> {};

    template <typename... Ts>
    using make_semitape_t = typename make_semitape<Ts...>::type;
  }


  namespace details::tape {
    //tape
    template <typename T, typename Prev = semitape::empty, typename Next = semitape::empty>
      requires details::semitape::is_semitape_v<Prev> && details::semitape::is_semitape_v<Next>
    struct tape : type_constant<T> {};

    using empty = tape<semitape::zero>;

    //is_tape
    template <typename Tape>
    struct is_tape : std::bool_constant<false> {};

    template <typename T, typename Prev, typename Next>
    struct is_tape<tape<T, Prev, Next>> : std::bool_constant<true> {};

    template <typename Tape>
    constexpr bool is_tape_v = is_tape<Tape>::value;

    //get_prev
    template <typename Tape>
    struct get_prev;

    template <typename T, typename Prev, typename Next>
    struct get_prev<tape<T, Prev, Next>> : type_constant<Prev> {};

    template <typename Tape>
    using get_prev_t = typename get_prev<Tape>::type;

    //get_next
    template <typename Tape>
    struct get_next;

    template <typename T, typename Prev, typename Next>
    struct get_next<tape<T, Prev, Next>> : type_constant<Next> {};

    template <typename Tape>
    using get_next_t = typename get_next<Tape>::type;

  }

  namespace tape {
    using details::semitape::zero;

    //next
    template <typename Tape>
    struct next;

    template <>
    struct next<details::tape::empty>
        : details::type_constant<details::tape::empty> {};

    template <typename NextT, typename Next>
    struct next<details::tape::tape<zero, details::semitape::empty, details::semitape::semitape<NextT, Next>>>
        : details::type_constant<details::tape::tape<NextT, details::semitape::empty, Next>> {};

    template <typename T, typename NextT, typename Prev, typename Next>
    struct next<details::tape::tape<T, Prev, details::semitape::semitape<NextT, Next>>>
        : details::type_constant<details::tape::tape<NextT, details::semitape::semitape<T, Prev>, Next>> {};

    template <typename T, typename Prev>
    struct next<details::tape::tape<T, Prev>>
        : details::type_constant<details::tape::tape<zero, details::semitape::semitape<T, Prev>>> {};

    template <typename T>
    using next_t = typename next<T>::type;

    //prev
    template <typename Tape>
    struct prev;

    template <typename T, typename PrevT, typename Prev, typename Next>
    struct prev<details::tape::tape<T, details::semitape::semitape<PrevT, Prev>, Next>>
        : details::type_constant<details::tape::tape<PrevT, Prev, details::semitape::semitape<T, Next>>> {};

    template <typename T, typename Next>
    struct prev<details::tape::tape<T, details::semitape::empty, Next>>
        : details::type_constant<details::tape::tape<
          zero, details::semitape::empty, details::semitape::semitape<T, Next>>> {};

    template <>
    struct prev<details::tape::empty>
        : details::type_constant<details::tape::empty> {};

    template <typename PrevT, typename Prev>
    struct prev<details::tape::tape<zero, details::semitape::semitape<PrevT, Prev>>>
        : details::type_constant<details::tape::tape<PrevT, Prev>> {};

    template <typename T>
    using prev_t = typename prev<T>::type;

    //get
    template <typename Tape>
    struct get;

    template <typename T, typename Prev, typename Next>
    struct get<details::tape::tape<T, Prev, Next>> : details::type_constant<T> {};

    template <typename Tape>
    using get_t = typename get<Tape>::type;

    //set
    template <typename Tape, typename T>
    struct set;

    template <typename T, typename NewT, typename Prev, typename Next>
    struct set<details::tape::tape<T, Prev, Next>, NewT>
        : details::type_constant<details::tape::tape<NewT, Prev, Next>> {};

    template <typename Tape, typename T>
    using set_t = typename set<Tape, T>::type;

    //make_tape
    template <typename... T>
    struct make_tape : details::type_constant<details::semitape::empty> {};

    template <typename T, typename... Ts>
    struct make_tape<T, Ts...>
        : details::type_constant<details::tape::tape<
          T, details::semitape::empty, details::semitape::make_semitape_t<Ts...>>> {};

    template <typename... Ts>
    using make_tape_t = typename make_tape<Ts...>::type;
  }
}