#pragma once

#define STATE(name) ttm_##name##_state

#define DECLARE_STATE(class_name, symbol) \
struct STATE(class_name) {\
  static const std::string to_str;\
};\
const std::string STATE(class_name)::to_str = symbol;

#define TAPE_VALUE(name) ttm_##name##_tape_value

#define DECLARE_TAPE_VALUE(class_name, symbol) \
struct TAPE_VALUE(class_name) {\
  static const std::string to_str;\
};\
const std::string TAPE_VALUE(class_name)::to_str = symbol;