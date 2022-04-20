/**
 * clock.c
 *
 * Copyright (C) 2022 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "clock.h"

double get_elapsed_ticks(clock_t start) {
	double diff;

	diff = (double)(clock() - start);
	return (diff / CLOCKS_PER_SEC);
}
