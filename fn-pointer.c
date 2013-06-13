#include <stdio.h>

int cube(int number) {
  return number * number * number;
}

int square(int number) {
  return number * number;
}

int calculate(int number,  int (*fn) (int)) {
  return fn(number);
}

int main(void) {
  int (*fn) (int);
  fn = square;
  printf("Result: %i\n", calculate(2, fn));
  fn = cube;
  printf("Result: %i\n", calculate(2, fn));
  return 0;
}