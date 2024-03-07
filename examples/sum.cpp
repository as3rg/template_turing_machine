#include <iostream>
#include "macros.h"
#include "../lib/turing-machine.h"
#include "debug.h"

using namespace ttm::turing_machine;
using namespace ttm::tape;

DECLARE_STATE(go_begin, "going to the beginning")
DECLARE_STATE(error, "error")
DECLARE_STATE(go1, "going to end of the first operand")
DECLARE_STATE(go2, "going to end of the second operand")
DECLARE_STATE(got1, "reached the end of the first operand")
DECLARE_STATE(got2, "reached the end of the second operand")
DECLARE_STATE(cur1_0, "carring bit 0 from the first operand to the end")
DECLARE_STATE(cur1_1, "carring bit 1 from the first operand to the end")
DECLARE_STATE(cur2_0, "carring bit 0 from the second operand to the end")
DECLARE_STATE(cur2_1, "carring bit 1 from the second operand to the end")
DECLARE_STATE(carry, "carring bit to next position")
DECLARE_STATE(check, "checking any bits left")
DECLARE_STATE(cont, "continue after the check")
DECLARE_STATE(pre_check, "going to = before the check")
DECLARE_STATE(clean, "cleaning up")
DECLARE_STATE(accepted, "accepted")

DECLARE_TAPE_VALUE(1, "1")
DECLARE_TAPE_VALUE(0, "0")
DECLARE_TAPE_VALUE(plus, "+")
DECLARE_TAPE_VALUE(eq, "=")
DECLARE_TAPE_VALUE(acc1, "[*]")  //cell contains one
DECLARE_TAPE_VALUE(empty_, "_")  //custom empty cell
DECLARE_TAPE_VALUE(acc2, "[**]") //cell contains two

using go_begin_state = STATE(go_begin);

using rules_ = rules<
  rule<TAPE_VALUE(0), STATE(go_begin), TAPE_VALUE(0), STATE(go_begin), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(go_begin), TAPE_VALUE(1), STATE(go_begin), moves::RIGHT>,
  rule<TAPE_VALUE(plus), STATE(go_begin), TAPE_VALUE(plus), STATE(go_begin), moves::RIGHT>,
  rule<TAPE_VALUE(eq), STATE(go_begin), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc1), STATE(go_begin), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc2), STATE(go_begin), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<zero, STATE(go_begin), TAPE_VALUE(eq), STATE(go1), moves::STAY>,
  rule<TAPE_VALUE(empty_), STATE(go_begin), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(0), STATE(go1), TAPE_VALUE(0), STATE(go1), moves::LEFT>,
  rule<TAPE_VALUE(1), STATE(go1), TAPE_VALUE(1), STATE(go1), moves::LEFT>,
  rule<TAPE_VALUE(plus), STATE(go1), TAPE_VALUE(plus), STATE(go1), moves::LEFT>,
  rule<TAPE_VALUE(eq), STATE(go1), TAPE_VALUE(eq), STATE(go1), moves::LEFT>,
  rule<TAPE_VALUE(acc1), STATE(go1), TAPE_VALUE(acc1), STATE(go1), moves::LEFT>,
  rule<TAPE_VALUE(acc2), STATE(go1), TAPE_VALUE(acc2), STATE(go1), moves::LEFT>,
  rule<zero, STATE(go1), zero, STATE(got1), moves::RIGHT>,
  rule<TAPE_VALUE(empty_), STATE(go1), TAPE_VALUE(empty_), STATE(go1), moves::LEFT>,
  rule<TAPE_VALUE(0), STATE(got1), TAPE_VALUE(empty_), STATE(cur1_0), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(got1), TAPE_VALUE(empty_), STATE(cur1_1), moves::RIGHT>,
  rule<TAPE_VALUE(plus), STATE(got1), TAPE_VALUE(plus), STATE(cur1_0), moves::RIGHT>,
  rule<TAPE_VALUE(eq), STATE(got1), zero, STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc1), STATE(got1), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc2), STATE(got1), TAPE_VALUE(acc2), STATE(error), moves::STAY>,
  rule<zero, STATE(got1), zero, STATE(error), moves::STAY>,
  rule<TAPE_VALUE(empty_), STATE(got1), TAPE_VALUE(empty_), STATE(got1), moves::RIGHT>,
  rule<TAPE_VALUE(0), STATE(cur1_0), TAPE_VALUE(0), STATE(cur1_0), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(cur1_0), TAPE_VALUE(1), STATE(cur1_0), moves::RIGHT>,
  rule<TAPE_VALUE(plus), STATE(cur1_0), TAPE_VALUE(plus), STATE(cur1_0), moves::RIGHT>,
  rule<TAPE_VALUE(eq), STATE(cur1_0), TAPE_VALUE(eq), STATE(cur1_0), moves::RIGHT>,
  rule<TAPE_VALUE(acc1), STATE(cur1_0), TAPE_VALUE(acc1), STATE(go2), moves::LEFT>,
  rule<TAPE_VALUE(acc2), STATE(cur1_0), TAPE_VALUE(acc2), STATE(error), moves::STAY>,
  rule<zero, STATE(cur1_0), zero, STATE(go2), moves::LEFT>,
  rule<TAPE_VALUE(empty_), STATE(cur1_0), TAPE_VALUE(empty_), STATE(cur1_0), moves::RIGHT>,
  rule<TAPE_VALUE(0), STATE(cur1_1), TAPE_VALUE(0), STATE(cur1_1), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(cur1_1), TAPE_VALUE(1), STATE(cur1_1), moves::RIGHT>,
  rule<TAPE_VALUE(plus), STATE(cur1_1), TAPE_VALUE(plus), STATE(cur1_1), moves::RIGHT>,
  rule<TAPE_VALUE(eq), STATE(cur1_1), TAPE_VALUE(eq), STATE(cur1_1), moves::RIGHT>,
  rule<TAPE_VALUE(acc1), STATE(cur1_1), TAPE_VALUE(acc2), STATE(go2), moves::LEFT>,
  rule<TAPE_VALUE(acc2), STATE(cur1_1), TAPE_VALUE(acc2), STATE(error), moves::STAY>,
  rule<zero, STATE(cur1_1), TAPE_VALUE(acc1), STATE(go2), moves::LEFT>,
  rule<TAPE_VALUE(empty_), STATE(cur1_1), TAPE_VALUE(empty_), STATE(cur1_1), moves::RIGHT>,
  rule<TAPE_VALUE(0), STATE(go2), TAPE_VALUE(0), STATE(go2), moves::LEFT>,
  rule<TAPE_VALUE(1), STATE(go2), TAPE_VALUE(1), STATE(go2), moves::LEFT>,
  rule<TAPE_VALUE(empty_), STATE(go2), TAPE_VALUE(empty_), STATE(go2), moves::LEFT>,
  rule<TAPE_VALUE(plus), STATE(go2), TAPE_VALUE(plus), STATE(got2), moves::RIGHT>,
  rule<TAPE_VALUE(eq), STATE(go2), TAPE_VALUE(eq), STATE(go2), moves::LEFT>,
  rule<TAPE_VALUE(acc1), STATE(go2), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc2), STATE(go2), TAPE_VALUE(acc2), STATE(error), moves::STAY>,
  rule<zero, STATE(go2), zero, STATE(error), moves::STAY>,
  rule<TAPE_VALUE(0), STATE(got2), TAPE_VALUE(empty_), STATE(cur2_0), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(got2), TAPE_VALUE(empty_), STATE(cur2_1), moves::RIGHT>,
  rule<TAPE_VALUE(empty_), STATE(got2), TAPE_VALUE(empty_), STATE(got2), moves::RIGHT>,
  rule<TAPE_VALUE(plus), STATE(got2), TAPE_VALUE(plus), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(eq), STATE(got2), TAPE_VALUE(eq), STATE(cur2_0), moves::RIGHT>,
  rule<TAPE_VALUE(acc1), STATE(got2), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc2), STATE(got2), TAPE_VALUE(acc2), STATE(error), moves::STAY>,
  rule<zero, STATE(got2), zero, STATE(error), moves::STAY>,
  rule<TAPE_VALUE(0), STATE(cur2_0), TAPE_VALUE(0), STATE(cur2_0), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(cur2_0), TAPE_VALUE(1), STATE(cur2_0), moves::RIGHT>,
  rule<TAPE_VALUE(empty_), STATE(cur2_0), TAPE_VALUE(empty_), STATE(cur2_0), moves::RIGHT>,
  rule<TAPE_VALUE(plus), STATE(cur2_0), TAPE_VALUE(plus), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(eq), STATE(cur2_0), TAPE_VALUE(eq), STATE(cur2_0), moves::RIGHT>,
  rule<TAPE_VALUE(acc1), STATE(cur2_0), TAPE_VALUE(1), STATE(go1), moves::STAY>,
  rule<TAPE_VALUE(acc2), STATE(cur2_0), TAPE_VALUE(0), STATE(carry), moves::RIGHT>,
  rule<zero, STATE(cur2_0), zero, STATE(pre_check), moves::LEFT>,
  rule<TAPE_VALUE(0), STATE(cur2_1), TAPE_VALUE(0), STATE(cur2_1), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(cur2_1), TAPE_VALUE(1), STATE(cur2_1), moves::RIGHT>,
  rule<TAPE_VALUE(empty_), STATE(cur2_1), TAPE_VALUE(empty_), STATE(cur2_1), moves::RIGHT>,
  rule<TAPE_VALUE(plus), STATE(cur2_1), TAPE_VALUE(plus), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(eq), STATE(cur2_1), TAPE_VALUE(eq), STATE(cur2_1), moves::RIGHT>,
  rule<TAPE_VALUE(acc1), STATE(cur2_1), TAPE_VALUE(0), STATE(carry), moves::RIGHT>,
  rule<TAPE_VALUE(acc2), STATE(cur2_1), TAPE_VALUE(1), STATE(carry), moves::RIGHT>,
  rule<zero, STATE(cur2_1), TAPE_VALUE(1), STATE(go1), moves::STAY>,
  rule<TAPE_VALUE(0), STATE(carry), TAPE_VALUE(0), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(1), STATE(carry), TAPE_VALUE(1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(empty_), STATE(carry), TAPE_VALUE(empty_), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(plus), STATE(carry), TAPE_VALUE(plus), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(eq), STATE(carry), TAPE_VALUE(eq), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc1), STATE(carry), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc2), STATE(carry), TAPE_VALUE(acc2), STATE(error), moves::STAY>,
  rule<zero, STATE(carry), TAPE_VALUE(acc1), STATE(go1), moves::LEFT>,
  rule<TAPE_VALUE(0), STATE(check), TAPE_VALUE(0), STATE(cont), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(check), TAPE_VALUE(1), STATE(cont), moves::RIGHT>,
  rule<TAPE_VALUE(empty_), STATE(check), TAPE_VALUE(empty_), STATE(check), moves::LEFT>,
  rule<TAPE_VALUE(plus), STATE(check), TAPE_VALUE(plus), STATE(check), moves::LEFT>,
  rule<TAPE_VALUE(eq), STATE(check), TAPE_VALUE(eq), STATE(check), moves::LEFT>,
  rule<TAPE_VALUE(acc1), STATE(check), TAPE_VALUE(acc1), STATE(check), moves::LEFT>,
  rule<TAPE_VALUE(acc2), STATE(check), TAPE_VALUE(acc2), STATE(check), moves::LEFT>,
  rule<zero, STATE(check), zero, STATE(clean), moves::RIGHT>,
  rule<TAPE_VALUE(0), STATE(cont), TAPE_VALUE(0), STATE(cont), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(cont), TAPE_VALUE(1), STATE(cont), moves::RIGHT>,
  rule<TAPE_VALUE(empty_), STATE(cont), TAPE_VALUE(empty_), STATE(cont), moves::RIGHT>,
  rule<TAPE_VALUE(plus), STATE(cont), TAPE_VALUE(plus), STATE(cont), moves::RIGHT>,
  rule<TAPE_VALUE(eq), STATE(cont), TAPE_VALUE(eq), STATE(cont), moves::RIGHT>,
  rule<TAPE_VALUE(acc1), STATE(cont), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc2), STATE(cont), TAPE_VALUE(acc2), STATE(error), moves::STAY>,
  rule<zero, STATE(cont), TAPE_VALUE(0), STATE(go1), moves::STAY>,
  rule<TAPE_VALUE(0), STATE(pre_check), TAPE_VALUE(0), STATE(pre_check), moves::LEFT>,
  rule<TAPE_VALUE(1), STATE(pre_check), TAPE_VALUE(1), STATE(pre_check), moves::LEFT>,
  rule<TAPE_VALUE(empty_), STATE(pre_check), TAPE_VALUE(empty_), STATE(pre_check), moves::LEFT>,
  rule<TAPE_VALUE(plus), STATE(pre_check), TAPE_VALUE(plus), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(eq), STATE(pre_check), TAPE_VALUE(eq), STATE(check), moves::LEFT>,
  rule<TAPE_VALUE(acc1), STATE(pre_check), TAPE_VALUE(acc1), STATE(error), moves::STAY>,
  rule<TAPE_VALUE(acc2), STATE(pre_check), TAPE_VALUE(acc2), STATE(error), moves::STAY>,
  rule<zero, STATE(pre_check), zero, STATE(error), moves::STAY>,
  rule<TAPE_VALUE(0), STATE(clean), zero, STATE(clean), moves::RIGHT>,
  rule<TAPE_VALUE(1), STATE(clean), zero, STATE(clean), moves::RIGHT>,
  rule<TAPE_VALUE(empty_), STATE(clean), zero, STATE(clean), moves::RIGHT>,
  rule<TAPE_VALUE(plus), STATE(clean), zero, STATE(clean), moves::RIGHT>,
  rule<TAPE_VALUE(eq), STATE(clean), zero, STATE(accepted), moves::RIGHT>,
  rule<TAPE_VALUE(acc1), STATE(clean), TAPE_VALUE(acc1), STATE(clean), moves::STAY>,
  rule<TAPE_VALUE(acc2), STATE(clean), TAPE_VALUE(acc2), STATE(clean), moves::STAY>,
  rule<zero, STATE(clean), zero, STATE(error), moves::STAY>>;

using final_states = state_list<STATE(accepted), STATE(error)>;

using tape = make_tape_t<TAPE_VALUE(1),
                         TAPE_VALUE(0),
                         TAPE_VALUE(1),
                         TAPE_VALUE(plus),
                         TAPE_VALUE(0),
                         TAPE_VALUE(0),
                         TAPE_VALUE(0),
                         TAPE_VALUE(0),
                         TAPE_VALUE(0),
                         TAPE_VALUE(1)>;

using machine = turing_machine<tape, go_begin_state>;
using worked_machine = apply_until_t<machine, rules_, final_states>;

//sum two numbers split by + in big-endian
int main() {
  std::cout << ">>>>>>>>>>>go_begin<<<<<<<<<<<<\n";
  ttm::debug::print_tm<machine>(std::cout);
  std::cout << ">>>>>>>>>>>finish<<<<<<<<<<<<\n";
  ttm::debug::print_tm<worked_machine>(std::cout);
  std::cout << ">>>>>>>>>>>progress<<<<<<<<<<\n";
  ttm::debug::apply_until<machine, rules_, final_states>(std::cout);
  return 0;
}