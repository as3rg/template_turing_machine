#pragma once

namespace turing_machine::helpers {
  template <typename T>
  struct type_constant {
    using type = T;
  };

  namespace pack {
    template <typename Type, Type... values>
    struct pack : type_constant<Type> {};

    template <typename Type, typename Pack, Type target>
    struct pack_contains : std::bool_constant<false> {};

    template <typename Type, Type value, Type... values, Type target>
    struct pack_contains<Type, pack<Type, value, values...>, target> : std::bool_constant<false> {};

    template <typename Type, Type value, Type... values>
    struct pack_contains<Type, pack<Type, value, values...>, value> : std::bool_constant<true> {};

    template <typename Type, typename Pack, Type target>
    constexpr bool pack_contains_v = pack_contains<Type, Pack, target>::value;
  }
}