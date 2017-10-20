/**
 * error.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "error.h"

/**
 * A more expressive way of determining if an
 * error was encountered from a return value.
 */
int is_error (int value) {
	return (value == -1);
}
