#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *n) {
  clear_decimal(n);
  int error = 0;
  if (src < 0) {
    s21_set_sign(n);
    src = (-1) * src;
  }
  if (src > INT_MAX)
    error = 1;
  else
    n->bits[0] = src;
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int *n) {
  int error = 0;
  int scale = s21_get_scale(src);
  if (src.bits[1] || src.bits[2]) {
    error = 1;
  } else {
    *n = src.bits[0];
    if (scale > 0 && scale <= 28) {
      *n /= pow(10, scale);
    }
  }
  if (s21_get_bit(src, 127)) *n = *n * (-1);
  return error;
}

int s21_from_float_to_decimal(float src, s21_decimal *n) {
  clear_decimal(n);
  int return_value = 0;
  if (isinf(src) || isnan(src)) {
    return_value = 1;
  } else {
    if (src != 0) {
      int sign = *(int *)&src >> 31;
      int exp = ((*(int *)&src & ~0x80000000) >> 23) - 127;
      double temp = (double)fabs(src);
      int off = 0;
      for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++) {
      }
      temp = round(temp);
      if (off <= 28 && (exp > -94 && exp < 96)) {
        floatbits mant = {0};
        temp = (float)temp;
        for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10) {
        }
        mant.fl = temp;
        exp = ((*(int *)&mant.fl & ~0x80000000) >> 23) - 127;
        n->bits[exp / 32] |= 1 << exp % 32;
        for (int i = exp - 1, j = 22; j >= 0; i--, j--)
          if ((mant.ui & (1 << j)) != 0) n->bits[i / 32] |= 1 << i % 32;
        n->bits[3] = (sign << 31) | (off << 16);
      }
    }
  }
  return return_value;
}

int s21_from_decimal_to_float(s21_decimal src, float *n) {
  double temp = (double)*n;
  for (int i = 0; i < 96; i++) {
    temp += s21_get_bit(src, i) * pow(2, i);
  }
  temp = temp * pow(10, -s21_get_scale(src));
  if (s21_get_bit(src, 127)) temp = temp * (-1);
  *n = temp;
  return 0;
}
