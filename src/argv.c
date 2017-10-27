/**
 * argv.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "argv.h"

static option_t options[] = {
	{ "SSLv2", OPT_SSLV2 },
	{ "SSLv3", OPT_SSLV3 },
	{ "TLSv1", OPT_TLSV1 },
	{ "TLSv1_1", OPT_TLSV1_1 },
	{ "TLSv1_2", OPT_TLSV1_2 },
};

/**
 * Get bitmask associated with key.
 */
int get_bitmask_from_key (char *key) {
	int index;

	for (index = 0; index < NUM_OPTS; index += 1) {
		option_t option = options[index];

		if (!compare(option.key, key)) {
			return option.value;
		}
	}

	return OPT_INVALID;
}
