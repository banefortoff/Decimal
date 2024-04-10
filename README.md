![DECIMAL](./images/decimal.jpg)

**The project is a native implementation of the '__int128' or 'Decimal' data type from the c# language.**

The Decimal value type represents decimal numbers ranging from positive 79,228,162,514,264,337,593,543,950,335 to negative 79,228,162,514,264,337,593,543,950,335. The default value of a Decimal is 0. The Decimal value type is appropriate for financial calculations that require large numbers of significant integral and fractional digits and no round-off errors. The Decimal type does not eliminate the need for rounding. Rather, it minimizes errors due to rounding.

* The program developed according to the principles of structured programming
* Implemented as a static library
* The defined type support numbers from -79,228,162,514,264,337,593,543,950,335 to +79,228,162,514,264,337,593,543,950,335
* The library includes arithmetic operators such as:
  + Addition - `int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)`
  + Subtraction - `int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)`
  + Multiplication - `int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)`
  + Division - `int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)`
  + Modulo - `int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)`
* The library includes comparison operators such as:
  + Less than - `int s21_is_less(s21_decimal value_1, s21_decimal value_2)`
  + Less than or equal to - `int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2)`
  + Greater than - `int s21_is_greater(s21_decimal value_1, s21_decimal value_2)`
  + Greater than or equal to - `int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2)`
  + Equal to - `int s21_is_equal(s21_decimal value_1, s21_decimal value_2)`
  + Not equal to - `int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2)`
* The library includes conversion functions such as:
  + From int - `int s21_from_int_to_decimal(int src, s21_decimal *dst)`
  + From float - `int s21_from_float_to_decimal(float src, s21_decimal *dst)`
  + To int - `int s21_from_decimal_to_int(s21_decimal src, int *dst)`
  + To float - `int s21_from_decimal_to_float(s21_decimal src, float *dst)`
* And another functions:
  + Rounds a specified Decimal number to the closest integer toward negative infinity - `int s21_floor(s21_decimal value, s21_decimal *result)`
  + Rounds a decimal value to the nearest integer - `int s21_round(s21_decimal value, s21_decimal *result)`
  + Returns the integral digits of the specified Decimal; any fractional digits are discarded, including trailing zeroes - `int s21_truncate(s21_decimal value, s21_decimal *result)`
  + Returns the result of multiplying the specified Decimal value by negative one - `int s21_negate(s21_decimal value, s21_decimal *result)`
  
***

This project was developed by a students of School 21: banefort, pipebomb, hallietw