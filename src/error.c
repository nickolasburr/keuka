/**
 * error.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "error.h"

/**
 * A more expressive way of determining if an
 * error was encountered from a return value.
 *
 * @note: This relies on the C convention of
 *        returning -1 on error. If a function
 *        returns a different value on error,
 *        manually check the value instead.
 */
int is_error (int value) {
	return (value == -1);
}
