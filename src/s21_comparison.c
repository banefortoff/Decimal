#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  s21_decimal tmp;
  clear_decimal(&tmp);
  s21_sub(value_1, value_2, &tmp);
  if (get_sign_decimal(tmp) != 0) {
    result = 1;
  } else if ((tmp.bits[0] == 0 && tmp.bits[1] == 0 && tmp.bits[2] == 0 &&
              get_sign_decimal(tmp) == 0)) {
    result = 0;
  }

  if (if_zero(value_1, value_2) == 0) result = 0;

  return result;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  s21_decimal tmp;
  if (get_sign_decimal(value_1) == 0 && get_sign_decimal(value_2) != 0) {
    result = 1;
  } else {
    clear_decimal(&tmp);
    s21_sub(value_1, value_2, &tmp);

    if (get_sign_decimal(tmp) == 0) {
      result = 1;
    }
  }
  if ((tmp.bits[0] == 0 && tmp.bits[1] == 0 && tmp.bits[2] == 0) ||
      (if_zero(value_1, value_2) == 0)) {
    result = 0;
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  s21_decimal tmp;

  clear_decimal(&tmp);
  s21_sub(value_1, value_2, &tmp);
  if ((get_sign_decimal(tmp) != 0) ||
      (tmp.bits[0] == 0 && tmp.bits[1] == 0 && tmp.bits[2] == 0)) {
    result = 1;
  }

  if (if_zero(value_1, value_2) == 0) result = 1;

  return result;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  s21_decimal tmp;
  clear_decimal(&tmp);
  s21_sub(value_1, value_2, &tmp);
  if ((get_sign_decimal(tmp) == 0) ||
      (tmp.bits[0] == 0 && tmp.bits[1] == 0 && tmp.bits[2] == 0)) {
    result = 1;
  }
  return result;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  s21_decimal tmp;
  clear_decimal(&tmp);
  s21_sub(value_1, value_2, &tmp);
  if (if_zero(value_1, value_2) == 0) result = 1;

  if ((tmp.bits[0] == 0 && tmp.bits[1] == 0 && tmp.bits[2] == 0) &&
      (get_sign_decimal(value_1) == get_sign_decimal(value_2))) {
    result = 1;
  }

  return result;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  s21_decimal tmp;
  clear_decimal(&tmp);
  s21_sub(value_1, value_2, &tmp);
  if ((tmp.bits[0] != 0 || tmp.bits[1] != 0 || tmp.bits[2] != 0) ||
      (get_sign_decimal(value_1) != get_sign_decimal(value_2))) {
    result = 1;
  }
  if (if_zero(value_1, value_2) == 0) result = 0;
  return result;
}
