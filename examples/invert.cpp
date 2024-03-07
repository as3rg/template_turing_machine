#include <iostream>
#include "macros.h"
#include "../lib/turing-machine.h"
#include "debug.h"

using namespace ttm::turing_machine;
using namespace ttm::tape;

DECLARE_STATE(start, "start")
DECLARE_STATE(working, "working")
DECLARE_STATE(stop, "stop")

DECLARE_TAPE_VALUE(1, "1")
DECLARE_TAPE_VALUE(0, "0")

using start_state = STATE(start);

using rules_ = rules<
  rule<any, STATE(start), same, STATE(working), moves::STAY>,
  rule<zero, STATE(working), zero, STATE(stop), moves::LEFT>,
  rule<TAPE_VALUE(1), STATE(working), TAPE_VALUE(0), STATE(working), moves::RIGHT>,
  rule<TAPE_VALUE(0), STATE(working), TAPE_VALUE(1), STATE(working), moves::RIGHT>>;

using final_states = state_list<STATE(stop)>;

using tape = make_tape_t<TAPE_VALUE(1),
                                TAPE_VALUE(0),
                                TAPE_VALUE(1),
                                TAPE_VALUE(1),
                                TAPE_VALUE(1),
                                TAPE_VALUE(0)>;

using machine = turing_machine<tape, start_state>;
using worked_machine = apply_until_t<machine, rules_, final_states>;

int main() {
  std::cout << ">>>>>>>>>>>start<<<<<<<<<<<<\n";
  ttm::debug::print_tm<machine>(std::cout);
  std::cout << ">>>>>>>>>>>finish<<<<<<<<<<<<\n";
  ttm::debug::print_tm<worked_machine>(std::cout);
  std::cout << ">>>>>>>>>>>progress<<<<<<<<<<\n";
  ttm::debug::apply_until<machine, rules_, final_states>(std::cout);
  return 0;
}
