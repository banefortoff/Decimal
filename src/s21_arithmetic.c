#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, sign_value1 = 0, sign_value2 = 0, exp_value1 = 0,
      exp_value2 = 0, exp_value = 0, count = 0, overflow_f = 0, sign = 0;
  s21_long_decimal mod, result_tmp, ten, tmp, res;
  clear_decimal(result);
  clear_long_decimal(&mod);
  clear_long_decimal(&result_tmp);
  clear_long_decimal(&res);
  clear_long_decimal(&ten);
  clear_long_decimal(&tmp);
  ten.bits[0] = 10;
  sign_value1 = get_sign_decimal(value_1);
  sign_value2 = get_sign_decimal(value_2);
  if (sign_value1 != sign_value2) {
    sign = 1;
  }
  exp_value1 = get_value_scale(value_1);
  exp_value2 = get_value_scale(value_2);
  exp_value = exp_value1 - exp_value2;

  s21_long_decimal long_value_1, long_value_2;
  from_decimal_for_long_decimal(value_1, &long_value_1);
  from_decimal_for_long_decimal(value_2, &long_value_2);
  if ((error = check_for_zero(long_value_2)) != 3) {
    s21_div_mantisa(long_value_1, long_value_2, &res, &mod);
    //
    if (check_for_zero(mod) != 3) {
      while (check_for_zero(mod) != 3 && exp_value < 28 && count < 10 &&
             overflow_f == 0) {
        s21_mul_mantisa(mod, ten, &result_tmp);
        s21_div_mantisa(result_tmp, long_value_2, &result_tmp, &mod);
        s21_mul_mantisa(res, ten, &tmp);
        s21_add_mantisa(tmp, result_tmp, &res);
        exp_value++;
        count++;
        if (res.bits[3] != 0) {
          overflow_f = 1;
        }
      }
    }

    while (exp_value < 0) {
      s21_mul_mantisa(res, ten, &res);
      exp_value++;
    }
    if (check_for_zero(res) == 3 && check_for_zero(long_value_1) != 3 &&
        exp_value >= 28) {
      error = 2;
    } else {
      error = from_long_decimal_for_decimal(res, result, exp_value, sign);
    }
  }
  return error;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, sign_value1 = 0, sign_value2 = 0, exp_value1 = 0,
      exp_value2 = 0, exp_value = 0, sign = 0;
  s21_long_decimal value_1_long, value_2_long, res_long;
  clear_long_decimal(&res_long);
  clear_decimal(result);
  get_sign_and_scale_both_value(&sign_value1, &sign_value2, &exp_value1,
                                &exp_value2, value_1, value_2);
  if (sign_value1 != sign_value2) {
    sign = 1;
  }
  exp_value = exp_value1 + exp_value2;
  from_decimal_for_long_decimal(value_1, &value_1_long);
  from_decimal_for_long_decimal(value_2, &value_2_long);
  s21_mul_mantisa(value_1_long, value_2_long, &res_long);
  error = from_long_decimal_for_decimal(res_long, result, exp_value, sign);
  return error;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, sign_value1 = 0, sign_value2 = 0, exp_value1 = 0,
      exp_value2 = 0, exp_value = 0, sign = 0, i;
  s21_long_decimal value_1_long, value_2_long, res_long;
  clear_long_decimal(&res_long);
  clear_decimal(result);
  get_sign_and_scale_both_value(&sign_value1, &sign_value2, &exp_value1,
                                &exp_value2, value_1, value_2);
  from_decimal_for_long_decimal(value_1, &value_1_long);
  from_decimal_for_long_decimal(value_2, &value_2_long);
  if ((i = exp_value1 - exp_value2) != 0) {
    if (i > 0) {
      exp_value = exp_value1;
      s21_mul_mantisa_by_ten(&value_2_long, i);
    } else {
      exp_value = exp_value2;
      s21_mul_mantisa_by_ten(&value_1_long, i * (-1));
    }
  } else {
    exp_value = exp_value1;
  }
  if (sign_value1 == sign_value2) {
    if (sign_value1 != 0) {
      sign = 1;
    }
    s21_add_mantisa(value_1_long, value_2_long, &res_long);
  } else {
    if (sign_value1 != 0) {
      sign = s21_sub_mantisa(value_2_long, value_1_long, &res_long);
    } else {
      sign = s21_sub_mantisa(value_1_long, value_2_long, &res_long);
    }
  }
  error = from_long_decimal_for_decimal(res_long, result, exp_value, sign);
  return error;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, sign_value1 = 0, sign_value2 = 0, exp_value1 = 0,
      exp_value2 = 0, exp_value = 0, sign = 0, i;
  s21_long_decimal value_1_long, value_2_long, res_long;
  clear_long_decimal(&res_long);
  clear_decimal(result);
  get_sign_and_scale_both_value(&sign_value1, &sign_value2, &exp_value1,
                                &exp_value2, value_1, value_2);
  from_decimal_for_long_decimal(value_1, &value_1_long);
  from_decimal_for_long_decimal(value_2, &value_2_long);
  if ((i = exp_value1 - exp_value2) != 0) {
    if (i > 0) {
      exp_value = exp_value1;
      s21_mul_mantisa_by_ten(&value_2_long, i);
    } else {
      exp_value = exp_value2;
      s21_mul_mantisa_by_ten(&value_1_long, i * (-1));
    }
  } else {
    exp_value = exp_value1;
  }
  if (sign_value1 == sign_value2) {
    sign = s21_sub_mantisa(value_1_long, value_2_long, &res_long);
    // printf("\nsign=%d\n", sign);
    // printf("sign_value1=%d\n", sign_value1);

    if (sign_value2 != 0) {
      if (sign == 1) {
        sign = 0;
      } else {
        sign = 1;
      }
    }
  } else {
    if (sign_value1 != 0) {
      sign = 1;
    }
    s21_add_mantisa(value_1_long, value_2_long, &res_long);
  }

  error = from_long_decimal_for_decimal(res_long, result, exp_value, sign);

  if (res_long.bits[0] + res_long.bits[1] + res_long.bits[2] +
          res_long.bits[3] + res_long.bits[4] + res_long.bits[5] +
          res_long.bits[6] ==
      0) {
    set_sign_decimal(result, 0);
  }
  return error;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0, sign_value1 = 0, sign_value2 = 0, exp_value1 = 0,
      exp_value2 = 0, sign = 0;
  s21_long_decimal mod, res;
  clear_decimal(result);
  clear_long_decimal(&mod);
  get_sign_and_scale_both_value(&sign_value1, &sign_value2, &exp_value1,
                                &exp_value2, value_1, value_2);
  if (sign_value1 != 0 || sign_value2 != 0) {
    set_sign_decimal(&value_1, 0);
    set_sign_decimal(&value_2, 0);
  }
  s21_long_decimal long_value_1, long_value_2;
  from_decimal_for_long_decimal(value_1, &long_value_1);
  from_decimal_for_long_decimal(value_2, &long_value_2);
  if ((error = check_for_zero(long_value_2)) != 3) {
    //  (value1/exp1)/(value2/exp2)=(value1*exp2)/(value2*exp1)

    s21_mul_mantisa_by_ten(&long_value_1, exp_value2);
    s21_mul_mantisa_by_ten(&long_value_2, exp_value1);
    s21_div_mantisa(long_value_1, long_value_2, &res, &mod);

    error = from_long_decimal_for_decimal(res, result, 0, sign);
    if (error == 0) {
      s21_mul(value_2, *result, result);
      s21_sub(value_1, *result, result);
      if ((sign_value1 != 0)) {
        set_sign_decimal(result, 1);
      }
    } else {
      clear_decimal(result);
    }
  }
  return error;
}
