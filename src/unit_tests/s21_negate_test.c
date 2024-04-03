#include "s21_decimal_test.h"

START_TEST(negate_0) {
  s21_decimal val = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};
  int sign_before = s21_get_sign(val);
  s21_negate(val, &res);
  int sign_after = s21_get_sign(res);

  ck_assert_int_ne(sign_before, sign_after);
}
END_TEST

START_TEST(negate_1) {
  s21_decimal val = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res;
  int sign_before = s21_get_sign(val);
  s21_negate(val, &res);
  int sign_after = s21_get_sign(res);
  ck_assert_int_ne(sign_before, sign_after);
}
END_TEST

START_TEST(negate_2) {
  s21_decimal val = {{0, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res = {{0}};
  int sign_before = s21_get_sign(val);
  s21_negate(val, &res);
  int sign_after = s21_get_sign(res);
  ck_assert_int_ne(sign_before, sign_after);
}
END_TEST

START_TEST(negate_3) {
  s21_decimal val = {{0}};
  s21_decimal res = {{0}};
  int sign_before = s21_get_sign(val);
  s21_negate(val, &res);
  int sign_after = s21_get_sign(res);
  ck_assert_int_ne(sign_before, sign_after);
}
END_TEST

START_TEST(negate_4) {
  s21_decimal value_1 = {{5, 0xFFFFFFFF, 0, 0}};
  s21_set_sign(&value_1);
  s21_decimal check = {{5, 0xFFFFFFFF, 0, 0}};
  s21_decimal result = {0};
  int return_value = s21_negate(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(negate_5) {
  s21_decimal value_1 = {{1, 1, 1, 0}};
  s21_decimal check = {{1, 1, 1, 0x80000000}};
  s21_decimal result = {0};
  int return_value = s21_negate(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(negate_6) {
  float a = 10.1234e5;
  float res_a = 0;
  s21_decimal src = {0};
  s21_from_float_to_decimal(a, &src);
  s21_decimal res = {0};
  s21_negate(src, &res);
  s21_from_decimal_to_float(res, &res_a);
  ck_assert_float_eq(-10.1234e5, res_a);
}
END_TEST

Suite *suite_negate(void) {
  Suite *s = suite_create("suite_negate");
  TCase *tc = tcase_create("negate_tc");

  tcase_add_test(tc, negate_0);
  tcase_add_test(tc, negate_1);
  tcase_add_test(tc, negate_2);
  tcase_add_test(tc, negate_3);
  tcase_add_test(tc, negate_4);
  tcase_add_test(tc, negate_5);
  tcase_add_test(tc, negate_6);

  suite_add_tcase(s, tc);
  return s;
}