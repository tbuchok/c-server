#include <stdio.h>

struct date {
  int month;
  int day;
  int year;
};

void printDate(struct date *d) {
  d->month = 4;
  d->day = 23;
  d->year = 1991;
  return;
}

int main(void) {
  struct date d = { .month = 12, .day = 30, .year = 1983 };
  printDate(&d);
  printf("Month: %i, Day: %i, Year: %i\n", d.month, d.day, d.year);
  return 0;
}