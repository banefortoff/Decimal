#include "s21_decimal_test.h"

void print_BSD_decimal(BCD n, s21_decimal dec_n, char *res) {
  // char result[26]={0};
  int sign = get_sign_decimal(dec_n);
  int scale = 33 - get_value_scale(dec_n);
  int count = 0, tmp;
  if (sign == 0) {
    res[count] = '+';
  } else {
    res[count] = '-';
  }
  count++;
  for (int i = 3; i >= 0; i--) {
    for (int j = 7; j >= 0; j--) {
      if (count == scale) {
        res[count] = '.';
        count++;
      }
      tmp = (n.bits[i] >> 4 * j) & 0x0000000F;
      res[count] = tmp + '0';
      count++;
    }
  }
  res[count] = '\0';
}

void print_float_mantis(int n) {
  for (int i = 0; i < 32; i++) {
    printf("%d ", (n >> i) & 0x00000001);
  }
  printf("\n");
}
/*-----------Convert from float to decimal and from decimal to
 * float-----------*/
START_TEST(s21_decimal_test_from_float) {
  s21_decimal dec_y;
  float examples[5] = {1.2332, -1.00032, 0.00023, 100.421, 1233.245};
  float x = 0;
  s21_from_float_to_decimal(examples[_i], &dec_y);
  s21_from_decimal_to_float(dec_y, &x);
  ck_assert_float_eq_tol(examples[_i], x, 0.0001);
}
END_TEST

START_TEST(s21_decimal_test_from_float8) {
  s21_decimal dec_y;
  float examples[5] = {123344, -132423423, 123123, 2.2, 21133.2};
  float x = 0;
  s21_from_float_to_decimal(examples[_i], &dec_y);
  s21_from_decimal_to_float(dec_y, &x);
  ck_assert_float_eq_tol(examples[_i], x, 0.1);
}
END_TEST

START_TEST(s21_decimal_test_from_float9) {
  s21_decimal dec_y;
  float examples[5] = {0.00000003, -0.000043, 0.0, 0.00000022, 0.0211332};
  float x = 0;
  s21_from_float_to_decimal(examples[_i], &dec_y);
  s21_from_decimal_to_float(dec_y, &x);
  ck_assert_float_eq_tol(examples[_i], x, 0.000000001);
}
END_TEST

START_TEST(s21_decimal_test_from_float1) {
  s21_decimal dec_y;
  float examples[4] = {1.2e-18, -1.2344e-10, 0.00000000000055,
                       -0.0000000000000000000001};
  float x = 0;
  for (int j = 0; j < 4; j++) {
    s21_from_float_to_decimal(examples[j], &dec_y);
    s21_from_decimal_to_float(dec_y, &x);
    ck_assert_float_eq_tol(examples[j], x, 0.0000001);
  }
}
END_TEST

START_TEST(s21_decimal_test_from_float3) {
  s21_decimal dec_y;
  float examples = 0;
  float x = 0;

  s21_from_float_to_decimal(examples, &dec_y);
  s21_from_decimal_to_float(dec_y, &x);
  ck_assert_float_eq_tol(examples, x, 0.000001);
}
END_TEST

// ARITHMETIC FUNCS
START_TEST(s21_add1) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[35] = {0};
  char *check[5] = {
      "+00000000000000000000000009275723", "-00000000000000000000000009275634",
      "-00000000000000000000000000000034", "-00000000000000000000000000000045",
      "-00000000000000000000000111111066"};
  char *std_out = NULL;
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  int n[6] = {123, 9275600, 34, 0, 45, 111111111};
  value1.bits[0] = n[_i];
  value2.bits[0] = n[_i + 1];
  int sign1[5] = {0, 1, 1, 0, 0};
  int sign2[5] = {0, 1, 0, 1, 1};
  set_sign_decimal(&value1, sign1[_i]);
  set_sign_decimal(&value2, sign2[_i]);
  set_value_scale(&value1, 0);
  set_value_scale(&value2, 0);
  s21_add(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  std_out = check[_i];
  int cmp = strcmp(res, std_out);
  ck_assert_int_eq(cmp, 0);
}
END_TEST

START_TEST(s21_add2) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char *check[5] = {"+00000000000000000000000009275723",
                    "-00000000000000000000000009275.634",
                    "-0000000000000000000000000000003.4",
                    "-000000000000000000000000000.00045",
                    "+00000000000000000000000.338888889"};
  char *std_out = NULL;
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  int n[6] = {123, 9275600, 34, 0, 45, 111111111};
  value1.bits[0] = n[_i];
  value2.bits[0] = n[_i + 1];
  int sign1[5] = {0, 1, 1, 0, 0};
  int sign2[5] = {0, 1, 0, 1, 1};
  set_sign_decimal(&value1, sign1[_i]);
  set_sign_decimal(&value2, sign2[_i]);
  int scale1[5] = {0, 3, 1, 0, 2};
  int scale2[5] = {0, 3, 0, 5, 9};
  set_value_scale(&value1, scale1[_i]);
  set_value_scale(&value2, scale2[_i]);
  s21_add(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  std_out = check[_i];
  ck_assert_str_eq(res, std_out);
}
END_TEST

START_TEST(s21_add3) {
  s21_decimal value2, result;
  // clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  s21_decimal value1 = {{4294967295, 4294967295, 4294967295, 0}};
  value2.bits[0] = 1;
  int cmp = s21_add(value1, value2, &result);
  ck_assert_int_eq(cmp, 1);
}
END_TEST

// check sub func.
START_TEST(s21_add7) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char check[34] = "+00010000.000000000000000000000002";
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 10000;
  value2.bits[0] = 11111;
  set_sign_decimal(&value1, 0);
  set_sign_decimal(&value2, 0);
  set_value_scale(&value1, 0);
  set_value_scale(&value2, 28);
  s21_add(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  ck_assert_str_eq(res, check);
}
END_TEST

START_TEST(s21_sub1) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char *check[5] = {"+0000.9999999999999999999999999999",
                    "-0000.9999999999999999999999999999",
                    "-0001.0000000000000000000000000001",
                    "+0001.0000000000000000000000000001",
                    "+0000000000.0100000000000000000001"};
  char *std_out = NULL;
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 1;
  value2.bits[0] = 1;
  int sign1[5] = {0, 1, 1, 0, 0};
  int sign2[5] = {0, 1, 0, 1, 1};
  set_sign_decimal(&value1, sign1[_i]);
  set_sign_decimal(&value2, sign2[_i]);
  int scale1[5] = {0, 0, 0, 0, 2};
  int scale2[5] = {28, 28, 28, 28, 22};
  set_value_scale(&value1, scale1[_i]);
  set_value_scale(&value2, scale2[_i]);
  s21_sub(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  std_out = check[_i];
  ck_assert_str_eq(res, std_out);
}
END_TEST

START_TEST(s21_sub2) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char *check[5] = {"-00000000000000000000000009275477",
                    "-00000000000000000000000009275.566",
                    "-0000000000000000000000000000003.4",
                    "+000000000000000000000000000.00045",
                    "+00000000000000000000000.561111111"};
  char *std_out = NULL;
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  int n[6] = {123, 9275600, 34, 0, 45, 111111111};
  value1.bits[0] = n[_i];
  value2.bits[0] = n[_i + 1];
  int sign1[5] = {0, 1, 1, 0, 0};
  int sign2[5] = {0, 1, 0, 1, 1};
  set_sign_decimal(&value1, sign1[_i]);
  set_sign_decimal(&value2, sign2[_i]);
  int scale1[5] = {0, 3, 1, 0, 2};
  int scale2[5] = {0, 3, 0, 5, 9};
  set_value_scale(&value1, scale1[_i]);
  set_value_scale(&value2, scale2[_i]);
  s21_sub(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  std_out = check[_i];
  ck_assert_str_eq(res, std_out);
}
END_TEST

START_TEST(s21_sub3) {
  s21_decimal value2, result;
  // clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  s21_decimal value1 = {{4294967295, 4294967295, 4294967295, 0}};
  value2.bits[0] = 1;
  set_sign_decimal(&value2, 1);
  int cmp = s21_sub(value1, value2, &result);
  ck_assert_int_eq(cmp, 1);
}
END_TEST

START_TEST(s21_sub4) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char check[34] = "+00000000000100000000000000011111";
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 0b01100011000100000000000000000000;
  value1.bits[1] = 0b01101011110001110101111000101101;
  value1.bits[2] = 0b00000000000000000000000000000101;
  value2.bits[0] = 11111;
  set_sign_decimal(&value2, 1);
  s21_sub(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  ck_assert_str_eq(res, check);
}
END_TEST

START_TEST(s21_sub5) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char check[34] = "+00000000000099999999999999988889";
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 0b01100011000100000000000000000000;
  value1.bits[1] = 0b01101011110001110101111000101101;
  value1.bits[2] = 0b00000000000000000000000000000101;
  value2.bits[0] = 11111;
  s21_sub(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  ck_assert_str_eq(res, check);
}
END_TEST

START_TEST(s21_sub6) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char check[34] = "+00009999.999999999999999999999998";
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 10000;
  value2.bits[0] = 11111;
  set_sign_decimal(&value1, 0);
  set_sign_decimal(&value2, 0);
  set_value_scale(&value1, 0);
  set_value_scale(&value2, 28);
  s21_sub(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  ck_assert_str_eq(res, check);
}
END_TEST

START_TEST(s21_mul1) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char *check[27] = {
      "+00000000000000000000000000000055", "+00000000000000000000000000000550",
      "+00000000000000000000000000005500", "+00000000000000000000000000055000",
      "+00000000000000000000000000550000", "+00000000000000000000000005500000",
      "+00000000000000000000000055000000", "+00000000000000000000000550000000",
      "+00000000000000000000005500000000", "+00000000000000000000055000000000",
      "+00000000000000000000550000000000", "+00000000000000000005500000000000",
      "+00000000000000000055000000000000", "+00000000000000000550000000000000",
      "+00000000000000005500000000000000", "+00000000000000055000000000000000",
      "+00000000000000550000000000000000", "+00000000000005500000000000000000",
      "+00000000000055000000000000000000", "+00000000000550000000000000000000",
      "+00000000005500000000000000000000", "+00000000055000000000000000000000",
      "+00000000550000000000000000000000", "+00000005500000000000000000000000",
      "+00000055000000000000000000000000", "+00000550000000000000000000000000",
      "+00005500000000000000000000000000"};
  char *std_out = NULL;
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  result.bits[0] = 1;
  value1.bits[0] = 55;
  value2.bits[0] = 10;
  for (int i = 0; i < _i; i++) {
    s21_mul(result, value2, &result);
  }
  s21_mul(value1, result, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  std_out = check[_i];
  ck_assert_str_eq(res, std_out);
}
END_TEST

START_TEST(s21_mul2) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char *check[5] = {"+00000000000000000114513570856800",
                    "+00000000000000000000000315.370400",
                    "-0000000000000000000000000000000.0",
                    "-000000000000000000000000000.00000",
                    "-000000000000000000000.04999999995"};
  char *std_out = NULL;
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  int n[6] = {12345678, 9275600, 34, 0, 45, 111111111};
  value1.bits[0] = n[_i];
  value2.bits[0] = n[_i + 1];
  int sign1[5] = {0, 1, 1, 0, 0};
  int sign2[5] = {0, 1, 0, 1, 1};
  set_sign_decimal(&value1, sign1[_i]);
  set_sign_decimal(&value2, sign2[_i]);
  int scale1[5] = {0, 3, 1, 0, 2};
  int scale2[5] = {0, 3, 0, 5, 9};
  set_value_scale(&value1, scale1[_i]);
  set_value_scale(&value2, scale2[_i]);
  s21_mul(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  std_out = check[_i];
  ck_assert_str_eq(res, std_out);
}
END_TEST

START_TEST(s21_mul4) {
  s21_decimal value2, result;
  clear_decimal(&value2);
  clear_decimal(&result);
  s21_decimal value1 = {{4294967295, 4294967295, 4294967295, 0}};
  value2.bits[0] = 2;
  int cmp = s21_mul(value1, value2, &result);
  ck_assert_int_eq(cmp, 1);
}
END_TEST

START_TEST(s21_mul5) {
  s21_decimal value1, value2, result;
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = value2.bits[0] = 1;
  set_value_scale(&value1, 28);
  set_value_scale(&value2, 1);
  int cmp = s21_mul(value1, value2, &result);
  ck_assert_int_eq(cmp, 2);
}
END_TEST

START_TEST(s21_div1) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char *check[27] = {
      "+00000000000000000000000000000055", "+00000000000000000000000000000550",
      "+00000000000000000000000000005500", "+00000000000000000000000000055000",
      "+00000000000000000000000000550000", "+00000000000000000000000005500000",
      "+00000000000000000000000055000000", "+00000000000000000000000550000000",
      "+00000000000000000000005500000000", "+00000000000000000000055000000000",
      "+00000000000000000000550000000000", "+00000000000000000005500000000000",
      "+00000000000000000055000000000000", "+00000000000000000550000000000000",
      "+00000000000000005500000000000000", "+00000000000000055000000000000000",
      "+00000000000000550000000000000000", "+00000000000005500000000000000000",
      "+00000000000055000000000000000000", "+00000000000550000000000000000000",
      "+00000000005500000000000000000000", "+00000000055000000000000000000000",
      "+00000000550000000000000000000000", "+00000005500000000000000000000000",
      "+00000055000000000000000000000000", "+00000550000000000000000000000000",
      "+00005500000000000000000000000000"};
  char *std_out = NULL;
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  result.bits[0] = 1;
  value1.bits[0] = 0b11111100000000000000000000000000;
  value1.bits[1] = 0b01101110111110101100000101001110;
  value1.bits[2] = 0b00010001110001010111111001001101;
  value2.bits[0] = 10;
  for (int i = 0; i < _i; i++) {
    s21_mul(result, value2, &result);
  }
  s21_div(value1, result, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  int index = 26 - _i;
  std_out = check[index];
  ck_assert_str_eq(res, std_out);
}
END_TEST

START_TEST(s21_div2) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char *check[5] = {"+0000000000000000000001.3309843029",
                    "+0000000000000000272811.7647058823",
                    "-000000000000000000000000000000.85",
                    "-000000000000000000000008888.88888",
                    "-00000000000000000000000000004.050"};
  char *std_out = NULL;
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  int n[6] = {12345678, 9275600, 34, 4, 45, 111111111};
  value1.bits[0] = n[_i];
  value2.bits[0] = n[_i + 1];
  int sign1[5] = {0, 1, 1, 0, 0};
  int sign2[5] = {0, 1, 0, 1, 1};
  set_sign_decimal(&value1, sign1[_i]);
  set_sign_decimal(&value2, sign2[_i]);
  int scale1[5] = {0, 3, 1, 0, 2};
  int scale2[5] = {0, 3, 0, 5, 9};
  set_value_scale(&value1, scale1[_i]);
  set_value_scale(&value2, scale2[_i]);
  s21_div(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  std_out = check[_i];
  ck_assert_str_eq(res, std_out);
}
END_TEST

START_TEST(s21_div3) {
  s21_decimal value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char check[34] = "+00039614081257132168796771975168";
  clear_bcd(&bcd_n);

  clear_decimal(&value2);
  clear_decimal(&result);
  s21_decimal value1 = {{4294967295, 4294967295, 4294967295, 0}};
  value2.bits[0] = 2;
  s21_div(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  ck_assert_str_eq(res, check);
}
END_TEST

START_TEST(s21_div4) {
  s21_decimal value2, result;
  clear_decimal(&value2);
  clear_decimal(&result);
  s21_decimal value1 = {{4294967295, 4294967295, 4294967295, 0}};
  value2.bits[0] = 5;
  set_value_scale(&value2, 1);
  int cmp = s21_div(value1, value2, &result);
  ck_assert_int_eq(cmp, 1);
}
END_TEST

START_TEST(s21_div5) {
  s21_decimal value1, value2, result;
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 1;
  value2.bits[0] = 100;
  set_value_scale(&value1, 28);
  set_value_scale(&value2, 1);
  int cmp = s21_div(value1, value2, &result);
  ck_assert_int_eq(cmp, 2);
}
END_TEST

START_TEST(s21_div6) {
  s21_decimal value1, value2, result;
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 1;
  value2.bits[0] = 0;
  int cmp = s21_div(value1, value2, &result);
  ck_assert_int_eq(cmp, 3);
}
END_TEST

START_TEST(s21_div7) {
  s21_decimal value1, value2, result;
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 0;
  value2.bits[0] = 0;
  set_value_scale(&value1, 28);
  set_value_scale(&value1, 5);
  int cmp = s21_div(value1, value2, &result);
  ck_assert_int_eq(cmp, 3);
}
END_TEST

START_TEST(s21_mod1) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char *check[5] = {"+00000000000000000000000003070078",
                    "-00000000000000000000000000000.026",
                    "-0000000000000000000000000000003.4",
                    "+000000000000000000000000000.00040",
                    "+00000000000000000000000.005555556"};
  char *std_out = NULL;
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  int n[6] = {12345678, 9275600, 34, 4, 45, 111111111};
  value1.bits[0] = n[_i];
  value2.bits[0] = n[_i + 1];
  int sign1[5] = {0, 1, 1, 0, 0};
  int sign2[5] = {0, 1, 0, 1, 1};
  set_sign_decimal(&value1, sign1[_i]);
  set_sign_decimal(&value2, sign2[_i]);
  int scale1[5] = {0, 3, 1, 0, 2};
  int scale2[5] = {0, 3, 0, 5, 9};
  set_value_scale(&value1, scale1[_i]);
  set_value_scale(&value2, scale2[_i]);
  s21_mod(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  std_out = check[_i];
  ck_assert_str_eq(res, std_out);
}
END_TEST

START_TEST(s21_mod2) {
  s21_decimal value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char check[34] = "+00000000000000000000000000000001";
  clear_bcd(&bcd_n);

  clear_decimal(&value2);
  clear_decimal(&result);
  s21_decimal value1 = {{4294967295, 4294967295, 4294967295, 0}};
  value2.bits[0] = 2;
  s21_mod(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  ck_assert_str_eq(res, check);
}
END_TEST

START_TEST(s21_mod3) {
  s21_decimal value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char check[34] = "+00000000000000000000000000000000";
  clear_bcd(&bcd_n);
  clear_decimal(&value2);
  clear_decimal(&result);
  s21_decimal value1 = {{4294967295, 4294967295, 4294967295, 0}};
  value2.bits[0] = 1;
  set_value_scale(&value2, 1);
  s21_mod(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  ck_assert_str_eq(res, check);
}
END_TEST

START_TEST(s21_mod4) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char check[34] = "+0000.0000000000000000000000000001";
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 1;
  value2.bits[0] = 100;
  set_value_scale(&value1, 28);
  set_value_scale(&value2, 1);
  s21_mod(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  ck_assert_str_eq(res, check);
}
END_TEST

START_TEST(s21_mod5) {
  s21_decimal value1, value2, result;
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 1;
  value2.bits[0] = 0;
  int cmp = s21_mod(value1, value2, &result);
  ck_assert_int_eq(cmp, 3);
}
END_TEST

START_TEST(s21_mod6) {
  s21_decimal value1, value2, result;
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 0;
  value2.bits[0] = 0;
  set_value_scale(&value1, 28);
  set_value_scale(&value1, 5);
  int cmp = s21_mod(value1, value2, &result);
  ck_assert_int_eq(cmp, 3);
}
END_TEST

START_TEST(s21_mod7) {
  s21_decimal value1, value2, result;
  BCD bcd_n;
  char res[34] = {0};
  char check[34] = "+00000000000000000000000000000000";
  clear_bcd(&bcd_n);
  clear_decimal(&value1);
  clear_decimal(&value2);
  clear_decimal(&result);
  value1.bits[0] = 0b01110000000000000000000000000000;
  value1.bits[1] = 0b10110011000000110001000010100111;
  value1.bits[2] = 0b11100010001011101010010010010011;
  value2.bits[0] = 1;
  set_value_scale(&value2, 3);
  s21_mod(value1, value2, &result);
  s21_decimal_to_BCD(&bcd_n, result);
  print_BSD_decimal(bcd_n, result, res);
  ck_assert_str_eq(res, check);
}
END_TEST

Suite *test_arifmetic(void) {
  int start_tests = 0, end_tests = 5;
  Suite *s1 = suite_create("Convertors and parsers");
  TCase *s21_decimal_tests = tcase_create("Tests");

  suite_add_tcase(s1, s21_decimal_tests);
  tcase_add_loop_test(s21_decimal_tests, s21_decimal_test_from_float,
                      start_tests, end_tests);
  tcase_add_loop_test(s21_decimal_tests, s21_decimal_test_from_float8,
                      start_tests, end_tests);
  tcase_add_loop_test(s21_decimal_tests, s21_decimal_test_from_float9,
                      start_tests, end_tests);
  tcase_add_test(s21_decimal_tests, s21_decimal_test_from_float1);
  tcase_add_test(s21_decimal_tests, s21_decimal_test_from_float3);
  tcase_add_loop_test(s21_decimal_tests, s21_add1, start_tests, end_tests);
  tcase_add_loop_test(s21_decimal_tests, s21_add2, start_tests, end_tests);
  tcase_add_test(s21_decimal_tests, s21_add3);
  tcase_add_test(s21_decimal_tests, s21_add7);
  tcase_add_loop_test(s21_decimal_tests, s21_sub1, start_tests, end_tests);
  tcase_add_loop_test(s21_decimal_tests, s21_sub2, start_tests, end_tests);
  tcase_add_test(s21_decimal_tests, s21_sub3);
  tcase_add_test(s21_decimal_tests, s21_sub4);
  tcase_add_test(s21_decimal_tests, s21_sub5);
  tcase_add_test(s21_decimal_tests, s21_sub6);
  tcase_add_loop_test(s21_decimal_tests, s21_mul1, start_tests, 27);
  tcase_add_loop_test(s21_decimal_tests, s21_mul2, start_tests, end_tests);
  tcase_add_test(s21_decimal_tests, s21_mul4);
  tcase_add_test(s21_decimal_tests, s21_mul5);
  tcase_add_loop_test(s21_decimal_tests, s21_div1, start_tests, 27);
  tcase_add_loop_test(s21_decimal_tests, s21_div2, start_tests, end_tests);
  tcase_add_test(s21_decimal_tests, s21_div3);
  tcase_add_test(s21_decimal_tests, s21_div4);
  tcase_add_test(s21_decimal_tests, s21_div5);
  tcase_add_test(s21_decimal_tests, s21_div6);
  tcase_add_test(s21_decimal_tests, s21_div7);

  tcase_add_loop_test(s21_decimal_tests, s21_mod1, start_tests, end_tests);
  tcase_add_test(s21_decimal_tests, s21_mod2);
  tcase_add_test(s21_decimal_tests, s21_mod3);
  tcase_add_test(s21_decimal_tests, s21_mod4);
  tcase_add_test(s21_decimal_tests, s21_mod5);
  tcase_add_test(s21_decimal_tests, s21_mod6);
  tcase_add_test(s21_decimal_tests, s21_mod7);
  return s1;
}
