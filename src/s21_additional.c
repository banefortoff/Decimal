#include "s21_decimal.h"

/*---------------------------------arithmetic---------------------------------*/
void clear_long_decimal(s21_long_decimal *n) {
  for (int i = 0; i < 7; i++) {
    n->bits[i] = 0;
  }
}

void bit_offset_long_decimal(s21_long_decimal *n1) {
  int tmp = n1->bits[0];
  for (int i = 6; i > 0; i--) {
    n1->bits[i] = n1->bits[i] << 1;
    // offset bits from junior to high bits (bits[1]|x 0 0 1 ...| to bits[2]|...
    // 0 1 0 x|)
    n1->bits[i] += value_bit_index(n1->bits[i - 1]);
  }
  n1->bits[0] = (tmp << 1);
}

int add_bit(int x, int y, int *z) {
  int bit_carry = 0;
  if (x == 1 && y == 1) {
    bit_carry = 1;
    *z = 0;
  } else {
    *z = x | y;
  }
  return bit_carry;
}

// return 3 if value=0
// return 0 if value not NULL
int check_for_zero(s21_long_decimal value) {
  int zero = 3;
  for (int i = 0; i < 6; i++) {
    if (value.bits[i] != 0) {
      zero = 0;
    }
  }
  return zero;
}

void copy_long_decimal(s21_long_decimal src, s21_long_decimal *dst) {
  clear_long_decimal(dst);
  for (int i = 0; i < 7; i++) {
    dst->bits[i] = src.bits[i];
  }
}

void from_decimal_for_long_decimal(s21_decimal src, s21_long_decimal *dst) {
  clear_long_decimal(dst);
  for (int i = 0; i < 3; i++) {
    dst->bits[i] = src.bits[i];
  }
}

int from_long_decimal_for_decimal(s21_long_decimal src, s21_decimal *dst,
                                  int scale, int sign) {
  int result = 0, flag_overflow = 0;
  s21_long_decimal summand, mod, res;
  clear_long_decimal(&summand);
  clear_long_decimal(&mod);
  clear_decimal(dst);
  summand.bits[0] = 10;
  copy_long_decimal(src, &res);
  while (res.bits[3] != 0 && scale >= 0) {
    s21_div_mantisa(res, summand, &res, &mod);
    scale--;
    flag_overflow = 1;
  }
  int c = res.bits[0] & 1;
  // around result
  if (flag_overflow == 1 && c == 1) {
    clear_long_decimal(&summand);
    summand.bits[0] = 1;
    s21_add_mantisa(res, summand, &res);
  }
  if (scale < 0 && src.bits[3] != 0) {
    if (sign == 0) {
      result = 1;
    } else {
      result = 2;
    }
  }
  if (scale > 28) {
    result = 2;
  }
  if (result == 0) {
    for (int i = 0; i < 3; i++) {
      dst->bits[i] = res.bits[i];
    }
    set_value_scale(dst, scale);
  }
  set_sign_decimal(dst, sign);
  return result;
}

int s21_add_mantisa(s21_long_decimal value_1, s21_long_decimal value_2,
                    s21_long_decimal *result) {
  clear_long_decimal(result);
  int n_error = 0, f_carry = 0, tmp_f_carry = 0, a, b, c, tmp, carry1 = 0,
      carry2 = 0;
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 32; j++) {
      tmp_f_carry = f_carry;
      a = (value_1.bits[i] >> j) & 1;
      b = (value_2.bits[i] >> j) & 1;
      carry1 = add_bit(a, b, &tmp);
      carry2 = add_bit(tmp, tmp_f_carry, &c);
      f_carry = carry1 | carry2;
      if (c == 1) {
        result->bits[i] = result->bits[i] | (1 << j);
      }
    }
  }
  n_error = f_carry;
  return n_error;
}

void invert_revers_code(s21_long_decimal *n) {
  s21_long_decimal one_bit;
  clear_long_decimal(&one_bit);
  one_bit.bits[0] = 1;
  for (int i = 0; i < 7; i++) {
    n->bits[i] = ~n->bits[i];
  }
  s21_add_mantisa(*n, one_bit, n);
}

// return 1 - negative result
// return 0 - positive result
int s21_sub_mantisa(s21_long_decimal value_1, s21_long_decimal value_2,
                    s21_long_decimal *result) {
  int sign = 0;
  invert_revers_code(&value_2);
  s21_add_mantisa(value_1, value_2, result);
  if (result->bits[6] != 0) {
    sign = 1;
    invert_revers_code(result);
  }
  return sign;
}

void s21_mul_mantisa(s21_long_decimal value_1, s21_long_decimal value_2,
                     s21_long_decimal *result) {
  int y;
  // s21_long_decimal tmp;
  clear_long_decimal(result);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 32; j++) {
      y = (value_2.bits[i] >> j) & 1;
      if (y == 1) {
        s21_add_mantisa(value_1, *result, result);
      }
      bit_offset_long_decimal(&value_1);
    }
  }
}

void s21_div_mantisa(s21_long_decimal value_1, s21_long_decimal value_2,
                     s21_long_decimal *result, s21_long_decimal *mod) {
  s21_long_decimal dividend, tmp_decimal;
  int j = 5, tmp = 0, count = 0, flag_add_dividor = 0;
  clear_long_decimal(&dividend);
  clear_long_decimal(result);
  clear_long_decimal(mod);
  clear_long_decimal(&tmp_decimal);
  while (value_1.bits[j] == 0 && j > 0) {
    j--;
  }
  while (tmp == 0 && count < 32) {
    tmp = (value_1.bits[j] << count) & 0x80000000;
    count++;
  }
  count--;
  while (count >= 0 && j >= 0) {
    bit_offset_long_decimal(&dividend);
    if ((tmp = (value_1.bits[j] << count) & 0x80000000) != 0) {
      dividend.bits[0] = dividend.bits[0] | 1;
    }
    if (flag_add_dividor == 0) {
      if (s21_sub_mantisa(dividend, value_2, &tmp_decimal) == 0) {
        flag_add_dividor = 1;
        result->bits[0] = 1;
        copy_long_decimal(tmp_decimal, &dividend);
      }
    } else {
      bit_offset_long_decimal(result);
      if (s21_sub_mantisa(dividend, value_2, &tmp_decimal) == 0) {
        result->bits[0] = result->bits[0] | 1;
        copy_long_decimal(tmp_decimal, &dividend);
      }
    }
    count++;
    if (count > 31) {
      j--;
      count = 0;
    }
  }
  for (int i = 0; i <= 3; i++) {
    mod->bits[i] = dividend.bits[i];
  }
}

void s21_mul_mantisa_by_ten(s21_long_decimal *value, int n) {
  s21_long_decimal ten;
  clear_long_decimal(&ten);
  ten.bits[0] = 10;
  for (int i = 0; i < n; i++) {
    s21_mul_mantisa(*value, ten, value);
  }
}

void get_sign_and_scale_both_value(int *sign_value1, int *sign_value2,
                                   int *exp_value1, int *exp_value2,
                                   s21_decimal value_1, s21_decimal value_2) {
  *sign_value1 = get_sign_decimal(value_1);
  *sign_value2 = get_sign_decimal(value_2);
  *exp_value1 = get_value_scale(value_1);
  *exp_value2 = get_value_scale(value_2);
}

/*---------------------------------comparison---------------------------------*/
int if_zero(s21_decimal value_1, s21_decimal value_2) {
  int flag = 1;
  if (value_1.bits[0] == 0 && value_1.bits[1] == 0 && value_1.bits[2] == 0 &&
      value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0 &&
      (get_sign_decimal(value_1) != get_sign_decimal(value_2))) {
    flag = 0;
  }
  return flag;
}

/*---------------------------------convertors---------------------------------*/
void set_sign_decimal(s21_decimal *n, int sign) {
  scale_union u_n;
  u_n.number = n->bits[3];
  u_n.sign = sign;
  n->bits[3] = u_n.number;
}

int get_sign_decimal(s21_decimal n) {
  scale_union u_n;
  u_n.number = n.bits[3];
  return u_n.sign;
}

int value_bit_index(int n) { return (n >> 31) & 1; }

void bit_offset(BCD *n1) {
  int tmp = n1->bits[0];
  for (int i = 3; i > 0; i--) {
    n1->bits[i] = n1->bits[i] << 1;
    // offset bit from junior to high bit (bit[1]|x 0 0 1 ...| to bit[2]|... 0 1
    // 0 x|)
    n1->bits[i] += value_bit_index(n1->bits[i - 1]);
  }
  n1->bits[0] = (tmp << 1);
}

int get_value_nybble(int n, int index_nybble) {
  int i = 4 * (index_nybble);
  int k = (n >> i) & 0x0000000F;
  return k;
}

void add_three_bit_to_nybble(int *n, int index_nybble) {
  int copy_n = *n;
  int i = 4 * (index_nybble);
  *n = (*n >> i) + 3;
  for (int j = i; j > 0; j--) {
    int bit = (copy_n >> (j - 1)) & 0x00000001;
    *n = (*n << 1) | bit;
  }
}

void clear_decimal(s21_decimal *n) {
  for (int i = 0; i < 4; i++) {
    n->bits[i] = 0;
  }
}

void clear_bcd(BCD *n) {
  for (int i = 0; i < 5; i++) {
    n->bits[i] = 0;
  }
}

int s21_get_bit(s21_decimal n, int index) {
  int mask = 1u << (index % 32);
  return (n.bits[index / 32] & mask) != 0;
}

void set_value_scale(s21_decimal *n, int k) {
  scale_union scale;
  int sign = get_sign_decimal(*n);
  if (sign != 0) {
    scale.sign = 1;
  }
  scale.number = n->bits[3];
  scale.scale_bit = k;
  n->bits[3] = scale.number;
}

int get_value_scale(s21_decimal n) {
  scale_union scale;
  scale.number = n.bits[3];
  return scale.scale_bit;
}

int s21_from_decimal_to_double(s21_decimal src, long double *n) {
  long double temp = (double)*n;
  for (int i = 0; i < 96; i++) {
    temp += s21_get_bit(src, i) * pow(2, i);
  }
  temp = temp * pow(10, -s21_get_scale(src));
  if (s21_get_bit(src, 127)) temp = temp * (-1);
  *n = temp;
  return 0;
}

void s21_decimal_to_BCD(BCD *bcd_n, s21_decimal n) {
  if (n.bits[0] == 0 && n.bits[1] == 0 && n.bits[2] == 0) {
    clear_bcd(bcd_n);
  } else {
    int i = 2, count = 96;
    // scip index if element array null
    while (n.bits[i] == 0 && i > 0) {
      i--;
    }
    count -= 32 * (2 - i);
    // scip not value null bit
    while ((n.bits[i] & 0x80000000) == 0) {
      n.bits[i] = n.bits[i] << 1;
      count--;
    }
    do {
      bit_offset(bcd_n);

      // here base code
      int tmp_bit = value_bit_index(n.bits[i]);
      bcd_n->bits[0] += tmp_bit;
      if (count != 1) {
        for (int j = 0; j < 4; j++) {
          for (int k = 0; k < 8; k++) {
            if (get_value_nybble(bcd_n->bits[j], k) > 4) {
              add_three_bit_to_nybble(&(bcd_n->bits[j]), k);
            }
          }
        }
      }
      n.bits[i] = n.bits[i] << 1;
      count--;
      if (count % 32 == 0) {
        i--;
      }
    } while (count != 0);
  }
}

/*---------------------------------other---------------------------------*/
int s21_get_scale(s21_decimal dst) {
  int mask = 127 << 16;
  int scale = (mask & dst.bits[3]) >> 16;
  return scale;
}

void s21_set_scale(s21_decimal *dst, int scale) {
  int sign = 0;
  sign = s21_get_sign(*dst);
  dst->bits[3] = 0;
  scale <<= 16;
  dst->bits[3] = scale | dst->bits[3];
  if (sign) s21_set_sign(dst);
}

void s21_set_bit(s21_decimal *dst, int index, int bits) {
  int mask = 1u << (index % 32);
  if (bits == 0)
    dst->bits[index / 32] = dst->bits[index / 32] & ~mask;
  else
    dst->bits[index / 32] = dst->bits[index / 32] | mask;
}

int s21_get_sign(s21_decimal dst) { return (dst.bits[3] & 1u << 31) != 0; }

void s21_set_sign(s21_decimal *dst) { dst->bits[3] = dst->bits[3] | 1u << 31; }

/*---------------------------------get_decimal---------------------------------*/
void get_decimal(s21_decimal value) {
  for (int i = 127; i >= 0; i--) {
    int bit = s21_get_bit(value, i);
    if (i == 31 || i == 63 || i == 95) {
      printf("\n");
    }
    printf("%d", bit);
  }
}
