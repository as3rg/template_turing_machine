#pragma once

#include <type_traits>

namespace ttm::details {
  template <typename T>
  struct type_constant {
    using type = T;
  };

  namespace pack {
    template <typename... Types>
    struct pack;

    template <typename Pack, typename Target>
    struct pack_contains : std::bool_constant<false> {};

    template <typename Type, typename... Types, typename Target>
    struct pack_contains<pack<Type, Types...>, Target> : std::bool_constant<false> {};

    template <typename Type, typename... Types>
    struct pack_contains<pack<Type, Types...>, Type> : std::bool_constant<true> {};

    template <typename Pack, typename Target>
    constexpr bool pack_contains_v = pack_contains<Pack, Target>::value;
  }
}