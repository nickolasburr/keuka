/**
 * argv.h
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#ifndef KEUKA_ARGV_H
#define KEUKA_ARGV_H

#include "common.h"
#include "error.h"
#include "utils.h"

#define MAX_HOSTNAME_LENGTH 256
#define MAX_PORT_LENGTH     6
#define MAX_SCHEME_LENGTH   6
#define MAX_URL_LENGTH      268

#define NUM_OPTS 5
#define OPT_INVALID -1
#define OPT_SSLV2 1
#define OPT_SSLV3 2
#define OPT_TLSV1 4
#define OPT_TLSV1_1 8
#define OPT_TLSV1_2 16

typedef struct {
	char *key;
	int value;
} option_t;

static option_t options[NUM_OPTS];

#define NUM_OPT_KEYS (sizeof(options) / sizeof(option_t))

int get_bitmask_from_key(char *);

#endif /* KEUKA_ARGV_H */
