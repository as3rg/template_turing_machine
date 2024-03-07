#include <iostream>

#include "turing-machine.h"

using inverse_rules = turing_machine::rules::rules<
  turing_machine::rules::rule<int, int, 2, 0, 1, 0, turing_machine::moves::moves::RIGHT>,
  turing_machine::rules::rule<int, int, 1, 0, 2, 0, turing_machine::moves::moves::RIGHT>,
  turing_machine::rules::rule<int, int, 0, 0, 0, 1, turing_machine::moves::moves::LEFT>>;

int main() {
  using t = turing_machine::tape::make_tape_t<int, 2, 1, 1, 2, 1, 2, 1, 1, 2, 2>;
  using tm = turing_machine::turing_machine::turing_machine<int, int, t, 0>;

  turing_machine::turing_machine::print(tm{}, std::cout);

  using tm2 = turing_machine::turing_machine::apply_until_t<
    tm, inverse_rules, int, turing_machine::turing_machine::state_list<int, 1>>;

  turing_machine::turing_machine::print(tm2{}, std::cout);

  return 0;
}