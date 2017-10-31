/**
 * argv.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "argv.h"

static method_t methods[] = {
	{
		"SSLv2",
		OPT_SSLV2
	},
	{
		"SSLv3",
		OPT_SSLV3
	},
	{
		"TLSv1",
		OPT_TLSV1
	},
	{
		"TLSv1_1",
		OPT_TLSV1_1
	},
	{
		"TLSv1_2",
		OPT_TLSV1_2
	},
};

static option_t options[] = {
	{
		"Chain",
		"--chain",
		"-c",
		"Show peer certificate chain.",
	},
	{
		"Cipher",
		"--cipher",
		"-C",
		"Show cipher negotiated during handshake.",
	},
	{
		"Issuer",
		"--issuer",
		"-i",
		"Show certificate issuer information."
	},
	{
		"Method",
		"--method",
		"-m",
		"Show method negotiated during handshake.",
	},
	{
		"Raw",
		"--raw",
		"-r",
		"Show raw certificate contents.",
	},
	{
		"Serial",
		"--serial",
		"-S",
		"Show certificate serial number.",
	},
	{
		"Signature Algorithm",
		"--signature-algorithm",
		"-A",
		"Show certificate signature algorithm.",
	},
	{
		"Validity",
		"--validity",
		"-V",
		"Show certificate Not Before/Not After validity range."
	},
	{
		"Help",
		"--help",
		"-h",
		"Show help information and usage examples.",
	},
	{
		"Version",
		"--version",
		"-v",
		"Show version information.",
	},
};

/**
 * Get bitmask associated with key.
 *
 * @note: May look to use this again
 *        depending on the direction
 *        taken with setter options.
 */
int get_bitmask_from_key (char *key) {
	int index;

	for (index = 0; index < NUM_METHODS; index += 1) {
		method_t *method = &methods[index];

		if (!compare(method->key, key)) {
			return method->value;
		}
	}

	return NOT_FOUND;
}

/**
 * Print usage information.
 */
void usage (void) {
	int index;

	fprintf(stdout, "Options:\n\n");

	for (index = 0; index < NUM_OPTIONS; index += 1) {
		option_t *option = &options[index];
		fprintf(stdout, "%4s%s, %s: %s\n", "", option->value, option->alias, option->desc);
	}
}
