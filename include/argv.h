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
#define MAX_PORT_LENGTH 6
#define MAX_SCHEME_LENGTH 6
#define MAX_URL_LENGTH 268

#define NUM_METHODS 5
#define NUM_OPTIONS 11

#define OPT_SSLV2 1
#define OPT_SSLV3 2
#define OPT_TLSV1 4
#define OPT_TLSV1_1 8
#define OPT_TLSV1_2 16

typedef struct {
	char *key;
	int value;
} method_t;

typedef struct {
	char *name;
	char *value;
	char *alias;
	char *desc;
} option_t;

static method_t methods[NUM_METHODS];
static option_t options[NUM_OPTIONS];

int get_bitmask_from_key(char *);
void usage(void);

#endif /* KEUKA_ARGV_H */
