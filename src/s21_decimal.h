#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FRACTION_FLOAT 10000000
#define MAX_COUNT_NUMBER_BCD 29
#define S21_INFINITY 1.0 / 0.0
#define s21_NEG_INF -1.0 / 0.0
#define MAX_DECIMAL 79228162514264337593543950335.0F
#define MAX_DEC \
  powf(2.0f, 96.0f) - 1.0  ////////////////////////////////////////////////////
#define MIN_DEC \
  -powf(2.0f, 96.0f) + 1.0  ////////////////////////////////////////////////////
#define s21_NULL ((void *)0)
#define START_INFO 96

typedef union {
  float n;
  struct {
    unsigned int mantisa : 23;
    unsigned int exponent : 8;
    int sign : 1;
  };
} float_union;

typedef struct BCD {
  int bits[5];
} BCD;

typedef struct decimal {
  unsigned int bits[4];
} s21_decimal;

typedef union {
  int number;
  struct {
    unsigned int null_bit : 16;
    unsigned int scale_bit : 8;
    unsigned int null_bit2 : 7;
    int sign : 1;
  };
} scale_union;

typedef union {
  int ui;
  float fl;
} floatbits;

typedef struct long_decimal {
  unsigned int bits[7];
} s21_long_decimal;

typedef enum { FALSE, TRUE } bool_t;

// arithmetic function
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// comparison function
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

// converters function
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// othet function
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// additional function
void get_sign_and_scale_both_value(int *sign_value1, int *sign_value2,
                                   int *exp_value1, int *exp_value2,
                                   s21_decimal value1, s21_decimal value2);
int from_long_decimal_for_decimal(s21_long_decimal src, s21_decimal *dst,
                                  int scale, int sign);
void from_decimal_for_long_decimal(s21_decimal src, s21_long_decimal *dst);
int add_bit(int x, int y, int *z);
void copy_long_decimal(s21_long_decimal src, s21_long_decimal *dst);
int s21_add_mantisa(s21_long_decimal value_1, s21_long_decimal value_2,
                    s21_long_decimal *result);
void bit_offset_long_decimal(s21_long_decimal *n1);
void invert_revers_code(s21_long_decimal *n);
int s21_sub_mantisa(s21_long_decimal value_1, s21_long_decimal value_2,
                    s21_long_decimal *result);
void s21_mul_mantisa(s21_long_decimal value_1, s21_long_decimal value_2,
                     s21_long_decimal *result);
void s21_div_mantisa(s21_long_decimal value_1, s21_long_decimal value_2,
                     s21_long_decimal *result, s21_long_decimal *mod);
void s21_mul_mantisa_by_ten(s21_long_decimal *value, int n);
int check_for_zero(s21_long_decimal value);

void set_sign_decimal(s21_decimal *n, int sign);
int get_sign_decimal(s21_decimal n);
int value_bit_index(int n);
void clear_decimal(s21_decimal *n);
void clear_long_decimal(s21_long_decimal *n);
void clear_bcd(BCD *n);
void set_value_scale(s21_decimal *n, int k);
int get_value_scale(s21_decimal n);
void set_bit_by_index(s21_decimal *decimal, int index);

void bit_offset(BCD *n1);
int get_value_nybble(int n, int index_nybble);
void add_three_bit_to_nybble(int *n, int index_nybble);
void s21_decimal_to_BCD(BCD *bcd_n, s21_decimal n);

int s21_get_bit(s21_decimal dst, int index);
int s21_get_scale(s21_decimal dst);
void s21_set_scale(s21_decimal *dst, int scale);
int s21_get_sign(s21_decimal dst);
void s21_set_sign(s21_decimal *dst);
void s21_set_bit(s21_decimal *dst, int index, int bit);
int s21_from_decimal_to_double(s21_decimal src, long double *dst);
int if_zero(s21_decimal value_1, s21_decimal value_2);

void get_decimal(s21_decimal value);