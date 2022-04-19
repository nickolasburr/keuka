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
	{
		"TLSv1_3",
		OPT_TLSV1_3
	},
};

static option_t options[] = {
	{
		"--bits",
		"-b",
		"Show public key length, in bits.",
	},
	{
		"--chain",
		"-c",
		"Show peer certificate chain.",
	},
	{
		"--cipher",
		"-C",
		"Show cipher negotiated during handshake.",
	},
	{
		"--issuer",
		"-i",
		"Show certificate issuer."
	},
	{
		"--method",
		"-m",
		"Show method negotiated during handshake.",
	},
	{
		"--no-sni",
		"-N",
		"Disable SNI support.",
	},
	{
		"--quiet",
		"-q",
		"Suppress timing and progress output.",
	},
	{
		"--raw",
		"-r",
		"Show raw certificate contents.",
	},
	{
		"--serial",
		"-S",
		"Show certificate serial number.",
	},
	{
		"--signature-algorithm",
		"-A",
		"Show certificate signature algorithm.",
	},
	{
		"--subject",
		"-s",
		"Show certificate subject.",
	},
	{
		"--validity",
		"-V",
		"Show certificate Not Before/Not After validity range."
	},
	{
		"--help",
		"-h",
		"Show help information.",
	},
	{
		"--version",
		"-v",
		"Show version number.",
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

	fprintf(
		stdout,
		"Usage: keuka [OPTIONS] [--] hostname\n\nOPTIONS:\n"
	);

	for (index = 0; index < NUM_OPTIONS; index += 1) {
		fprintf(
			stdout,
			"%4s%s, %-22s%-4s%-24s\n",
			"",
			options[index].alias,
			options[index].value,
			"",
			options[index].desc
		);
	}
}
