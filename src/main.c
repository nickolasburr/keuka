/**
 * main.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "main.h"

/**
 * Usage:
 *
 * keuka --chain --cipher --raw --serial google.com
 * keuka --issuer --method --signature-algorithm -- amazon.com
 * keuka -ACim github.com
 * keuka -qCA www.ieee.org
 */

int main (int argc, char **argv) {
	clock_t start;
	size_t crt_index, sig_bytes;
	int last_index, penult_index, sig_type_err,
	    opt_value, short_opt_index, long_opt_index;
	int bits, chain, cipher, issuer, method,
	    no_sni, pad_fmt, pad_tfmt, quiet;
	int serial, server, raw, sig_algo,
	    subject, validity;
	int attach, status;
	const char *hostname;
	const char *protocol = "https";
	char port[MAX_PORT_LENGTH],
	     url[MAX_URL_LENGTH];
	const SSL_METHOD *ssl_method;
	ASN1_INTEGER *asn1_serial;
	const ASN1_BIT_STRING *asn1_sig;
	const X509_ALGOR *sig_type;
	STACK_OF(X509) *fullchain;
	BIO *bp;
	X509 *crt = NULL,
	     *tcrt = NULL;
	X509_NAME *crtname, *tcrtname;
	EVP_PKEY *pubkey = NULL,
	         *tpubkey = NULL;
	SSL_CTX *ctx;
	SSL *ssl;
	const SSL_CIPHER *ssl_cipher = NULL;

	server = 0;
	last_index = (argc - 1);
	penult_index = (last_index - 1);

	/**
	 * Add padding after progress output, if applicable.
	 */
	pad_fmt = (argc > 2 && argv[penult_index] != OPT_LSEP);

	/**
	 * OPTIONS
	 *
	 * -b, --bits                   Show public key length, in bits.
	 * -c, --chain                  Show entire peer certificate chain.
	 * -C, --cipher                 Show cipher used for exchange.
	 * -i, --issuer                 Show certificate issuer information.
	 * -m, --method                 Show method version for exchange.
	 * -N, --no-sni                 Disable SNI support.
	 * -q, --quiet                  Suppress timing and progress output.
	 * -r, --raw                    Show raw certificate contents.
	 * -S, --serial                 Show certificate serial number.
	 * -A, --signature-algorithm    Show signature algorithm.
	 * -s, --subject                Show certificate subject.
	 * -V, --validity               Show certificate Not Before/Not After validity range.
	 * -h, --help                   Show help information and usage examples.
	 * -v, --version                Show version information.
	 */

	/**
	 * Initialize defaults.
	 */
	bits = 0;
	chain = 0;
	cipher = 0;
	issuer = 0;
	method = 0;
	no_sni = 0;
	short_opt_index = 0;
	long_opt_index = 0;
	quiet = 0;
	raw = 0;
	serial = 0;
	sig_algo = 0;
	subject = 0;
	validity = 0;

	static struct option long_options[] = {
		{ "bits", no_argument, 0, 'b' },
		{ "chain", no_argument, 0, 'c' },
		{ "cipher", no_argument, 0, 'C' },
		{ "issuer", no_argument, 0, 'i' },
		{ "method", no_argument, 0, 'm' },
		{ "no-sni", no_argument, 0, 'N' },
		{ "quiet", no_argument, 0, 'q' },
		{ "raw", no_argument, 0, 'r' },
		{ "serial", no_argument, 0, 'S' },
		{ "signature-algorithm", no_argument, 0, 'A' },
		{ "subject", no_argument, 0, 's' },
		{ "validity", no_argument, 0, 'V' },
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'v' },
	};

	while ((opt_value = getopt_long(argc, argv, "bcCimNqrSAsVhv", long_options, &long_opt_index)) != -1) {
		switch (opt_value) {
			/**
			 * If --bits option was given, output
			 * public key length, in bits.
			 */
			case 'b':
				bits = 1;
				continue;
			/**
			 * If --chain option was given, output
			 * the entire peer certificate chain.
			 */
			case 'c':
				chain = 1;
				continue;
			/**
			 * If --cipher option was given, output
			 * the cipher used for the exchange.
			 */
			case 'C':
				cipher = 1;
				continue;
			/**
			 * If --issuer option was given, output
			 * issuer information for certificate.
			 */
			case 'i':
				issuer = 1;
				continue;
			/**
			 * If --method option was given, output
			 * version of method used for handshake.
			 */
			case 'm':
				method = 1;
				continue;
			/**
			 * If --no-sni option was given, disable
			 * establishing connection, handshake.
			 */
			case 'N':
				no_sni = 1;
				continue;
			/**
			 * If --quiet option was given, suppress
			 * timing and progress-related output.
			 */
			case 'q':
				quiet = 1;
				pad_fmt = 0;
				continue;
			/**
			 * If --raw option was given, output
			 * raw certificate contents to stdout.
			 */
			case 'r':
				raw = 1;
				continue;
			/**
			 * If --serial option was given, output
			 * serial number for the certificate(s).
			 */
			case 'S':
				serial = 1;
				continue;
			/**
			 * If --signature-algorithm option was given,
			 * output signature algorithm used for certificate(s).
			 */
			case 'A':
				sig_algo = 1;
				continue;
			/**
			 * If --subject option was given, output
			 * the certificate(s) subject information.
			 */
			case 's':
				subject = 1;
				continue;
			/**
			 * If --validity option was given, output
			 * Not Before/Not After validity time range.
			 */
			case 'V':
				validity = 1;
				continue;
			/**
			 * If --help option was given, output
			 * usage information and exit.
			 */
			case 'h':
				usage();
				exit(EXIT_SUCCESS);
			/**
			 * If --version option was given, output
			 * the current release version and exit.
			 */
			case 'v':
				fprintf(stdout, "%s\n", KEUKA_VERSION);
				exit(EXIT_SUCCESS);
			case '?':
				usage();
				exit(EXIT_FAILURE);
			default:
				break;
		}
	}

	/**
	 * If no arguments were given,
	 * complain to stderr and exit.
	 */
	if (last_index < 1) {
		fprintf(stderr, "Error: Hostname not specified.\n");
		exit(EXIT_FAILURE);
	}

	/**
	 * Limit length of hostname given as an argument.
	 */
	if (length(argv[last_index]) > MAX_HOSTNAME_LENGTH) {
		fprintf(stderr, "Error: Hostname exceeds maximum length of 256 characters.\n");
		exit(EXIT_FAILURE);
	}

	/**
	 * The last element in argv should be the peer hostname.
	 */
	hostname = argv[last_index];

	/**
	 * Assemble URL for request.
	 */
	copy(url, protocol);
	concat(url, "://");
	concat(url, hostname);

	/**
	 * Run OpenSSL initialization tasks.
	 */
	SSL_load_error_strings();
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OpenSSL_add_all_digests();
	SSL_library_init();

	/**
	 * Start execution clock.
	 */
	start = clock();

	/**
	 * Initialize new BIO.
	 */
	bp = BIO_new_fp(stdout, BIO_NOCLOSE);

	/**
	 * Method for peer connection.
	 */
	ssl_method = SSLv23_client_method();

	if (!quiet) {
		BIO_printf(
			bp,
			"%s [%fs] Establishing SSL context.\n",
			KEUKA_NEUTRAL_INDICATOR,
			get_elapsed_ticks(start)
		);
	}

	/**
	 * Establish new SSL context.
	 */
	if (is_null(ctx = SSL_CTX_new(ssl_method))) {
		/**
		 * Display error in progress format, unless given --quiet.
		 */
		if (!quiet) {
			BIO_printf(
				bp,
				"%s [%fs] Error: Unable to establish SSL context.\n",
				KEUKA_NEUTRAL_INDICATOR,
				get_elapsed_ticks(start)
			);
		} else {
			BIO_printf(bp, "Error: Unable to establish SSL context.\n");
		}

		goto on_error;
	}

	if (!quiet) {
		BIO_printf(
			bp,
			"%s [%fs] SSL context established.\n",
			KEUKA_NEUTRAL_INDICATOR,
			get_elapsed_ticks(start)
		);
	}

	/**
	 * Make TCP socket connection.
	 */
	server = mksock(url, bp);

	if (!quiet) {
		BIO_printf(
			bp,
			"%s [%fs] Establishing connection to %s.\n",
			KEUKA_OUTBOUND_INDICATOR,
			get_elapsed_ticks(start),
			hostname
		);
	}

	/**
	 * Exit if there was an issue establishing a connection.
	 */
	if (is_error(server, -1)) {
		/**
		 * Display error in progress format, unless given --quiet.
		 */
		if (!quiet) {
			BIO_printf(
				bp,
				"%s [%fs] Error: Unable to resolve hostname %s.\n",
				KEUKA_INBOUND_INDICATOR,
				get_elapsed_ticks(start),
				hostname
			);
		} else {
			BIO_printf(bp, "Error: Unable to resolve hostname %s.\n", hostname);
		}

		exit(EXIT_FAILURE);
	}

	if (!quiet) {
		BIO_printf(
			bp,
			"%s [%fs] Connection established.\n",
			KEUKA_INBOUND_INDICATOR,
			get_elapsed_ticks(start)
		);
	}

	/**
	 * Establish connection, set state in client mode.
	 */
	ssl = SSL_new(ctx);
	SSL_set_connect_state(ssl);

	/**
	 * Disable SNI support if --no-sni was given.
	 */
	if (!no_sni) {
		SSL_set_tlsext_host_name(ssl, hostname);
	}

	if (!quiet) {
		BIO_printf(
			bp,
			"%s [%fs] Attaching SSL session to socket.\n",
			KEUKA_NEUTRAL_INDICATOR,
			get_elapsed_ticks(start)
		);
	}

	attach = SSL_set_fd(ssl, server);

	/**
	 * Attach the SSL session to the TCP socket.
	 */
	if (is_error(attach, -1)) {
		/**
		 * Display error in progress format, unless given --quiet.
		 */
		if (!quiet) {
			BIO_printf(
				bp,
				"%s [%fs] Error: Unable to attach SSL session to socket.\n",
				KEUKA_NEUTRAL_INDICATOR,
				get_elapsed_ticks(start)
			);
		} else {
			BIO_printf(bp, "Error: Unable to attach SSL session to socket.\n");
		}

		goto on_error;
	}

	if (!quiet) {
		BIO_printf(
			bp,
			"%s [%fs] SSL session attached, initiating handshake.\n",
			KEUKA_OUTBOUND_INDICATOR,
			get_elapsed_ticks(start)
		);
	}

	status = SSL_connect(ssl);

	/**
	 * Bridge the connection.
	 */
	if (status != 1) {
		/**
		 * Display error in progress format, unless given --quiet.
		 */
		if (!quiet) {
			BIO_printf(
				bp,
				"%s [%fs] Error: Could not build SSL session with %s. Handshake aborted.\n",
				KEUKA_NEUTRAL_INDICATOR,
				get_elapsed_ticks(start),
				url
			);
		} else {
			BIO_printf(
				bp,
				"Error: Could not build SSL session with %s. Handshake aborted.\n",
				url
			);
		}

		goto on_error;
	}

	ssl_cipher = SSL_get_current_cipher(ssl);

	if (!quiet) {
		BIO_printf(
			bp,
			"%s [%fs] %s negotiated, handshake complete.\n",
			KEUKA_INBOUND_INDICATOR,
			get_elapsed_ticks(start),
			SSL_CIPHER_get_version(ssl_cipher)
		);

		if (pad_fmt) {
			BIO_printf(bp, "\n");
		}
	}

	/**
	 * Print cipher used if --cipher was given.
	 */
	if (cipher) {
		BIO_printf(
			bp,
			"--- Cipher: %s\n",
			SSL_CIPHER_get_name(ssl_cipher)
		);
	}

	/**
	 * If --method option was given, output
	 * version of method used for handshake.
	 */
	if (method) {
		BIO_printf(
			bp,
			"--- Method: %s\n",
			SSL_get_version(ssl)
		);
	}

	/**
	 * Print full chain if --chain was given.
	 */
	if (chain) {
		/**
		 * Get peer certificate chain.
		 */
		if (is_null(fullchain = SSL_get_peer_cert_chain(ssl))) {
			BIO_printf(bp, "Error: Could not get certificate chain from %s.\n", url);
			goto on_error;
		}

		BIO_printf(bp, "--- Certificate Chain:\n");

		/**
		 * Output certificate chain.
		 */
		for (crt_index = 0; crt_index < sk_X509_num(fullchain); crt_index += 1) {
			pad_tfmt = 0;
			tcrt = sk_X509_value(fullchain, crt_index);
			tcrtname = X509_get_subject_name(tcrt);
			tpubkey = X509_get_pubkey(tcrt);

			BIO_printf(bp, "%5d: ", (int) crt_index);

			/**
			 * If --subject option was given, output certificate subject information.
			 */
			if (subject) {
				BIO_printf(bp, "--- Subject: ");
				X509_NAME_print_ex(
					bp,
					tcrtname,
					0,
					XN_FLAG_SEP_CPLUS_SPC
				);
				pad_tfmt = 1;
			}

			/**
			 * If --issuer option was given, output certificate issuer information.
			 */
			if (issuer) {
				if (pad_tfmt) {
					BIO_printf(bp, "%s%7s", "\n", "");
				} else {
					pad_tfmt = 1;
				}

				BIO_printf(bp, "--- Issuer: ");
				X509_NAME_print_ex(
					bp,
					X509_get_issuer_name(tcrt),
					0,
					XN_FLAG_SEP_CPLUS_SPC
				);
			}

			if (bits) {
				if (pad_tfmt) {
					BIO_printf(bp, "%s%7s", "\n", "");
				} else {
					pad_tfmt = 1;
				}

				BIO_printf(
					bp,
					"--- Bits: %d",
					EVP_PKEY_bits(tpubkey)
				);
			}

			/**
			 * If --serial option was given, output ASN1 serial.
			 */
			if (serial) {
				if (pad_tfmt) {
					BIO_printf(bp, "%s%7s", "\n", "");
				} else {
					pad_tfmt = 1;
				}

				BIO_printf(bp, "--- Serial: ");
				i2a_ASN1_INTEGER(
					bp,
					X509_get_serialNumber(tcrt)
				);
			}

			/**
			 * If --signature-algorithm option was given,
			 * output signature algorithm for certificate(s).
			 */
			if (sig_algo) {
				if (pad_tfmt) {
					BIO_printf(bp, "%s%7s", "\n", "");
				} else {
					pad_tfmt = 1;
				}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
				X509_get0_signature(&asn1_sig, &sig_type, tcrt);
#else
				sig_type = tcrt->sig_alg;
				asn1_sig = tcrt->signature;
#endif

				BIO_printf(bp, "--- Signature Algorithm: ");
				sig_type_err = i2a_ASN1_OBJECT(bp, sig_type->algorithm);

				if (is_error(sig_type_err, -1) || is_error(sig_type_err, 0)) {
					BIO_printf(bp, "Could not get signature algorithm.");
				}
			}

			/**
			 * If --validity option was given, output the
			 * range of Not Before/Not After timestamps.
			 */
			if (validity) {
				if (pad_tfmt) {
					BIO_printf(bp, "%s%7s", "\n", "");
				} else {
					pad_tfmt = 1;
				}

				BIO_printf(bp, "--- Validity:\n");
				BIO_printf(bp, "%11s%s", "", "--- Not Before: ");
				ASN1_TIME_print(bp, X509_get_notBefore(tcrt));
				BIO_printf(bp, "\n");
				BIO_printf(bp, "%11s%s", "", "--- Not After: ");
				ASN1_TIME_print(bp, X509_get_notAfter(tcrt));
			}

			if (!pad_tfmt) {
				BIO_printf(bp, "[redacted]");
			}

			BIO_printf(bp, "\n");
		}

		/**
		 * Output raw certificate contents if --raw option was specified.
		 */
		if (raw) {
			BIO_printf(bp, "\n");
			PEM_write_bio_PUBKEY(bp, tpubkey);
			BIO_printf(bp, "\n");

			for (crt_index = 0; crt_index < sk_X509_num(fullchain); crt_index += 1) {
				PEM_write_bio_X509(
					bp,
					sk_X509_value(fullchain, crt_index)
				);
				BIO_printf(bp, "\n");
			}
		}
	} else {
		/**
		 * Get peer certificate.
		 */
		if (is_null(crt = SSL_get_peer_certificate(ssl))) {
			BIO_printf(bp, "Error: Could not get certificate from %s.\n", url);
			goto on_error;
		}

		crtname = X509_get_subject_name(crt);
		pubkey = X509_get_pubkey(crt);

		/**
		 * If --subject option was given, output certificate subject information.
		 */
		if (subject) {
			BIO_printf(bp, "--- Subject: ");
			X509_NAME_print_ex(bp, crtname, 0, XN_FLAG_SEP_COMMA_PLUS);
			BIO_printf(bp, "\n");
		}

		/**
		 * If --issuer option was given, output certificate issuer information.
		 */
		if (issuer) {
			BIO_printf(bp, "--- Issuer: ");
			X509_NAME_print_ex(bp, X509_get_issuer_name(crt), 0, XN_FLAG_SEP_CPLUS_SPC);
			BIO_printf(bp, "\n");
		}

		if (bits) {
			BIO_printf(bp, "%s%d\n", "--- Bits: ", EVP_PKEY_bits(pubkey));
		}

		/**
		 * If --serial option was given, output ASN1 serial.
		 */
		if (serial) {
			BIO_printf(bp, "--- Serial: ");
			i2a_ASN1_INTEGER(
				bp,
				X509_get_serialNumber(crt)
			);
			BIO_printf(bp, "\n");
		}

		/**
		 * If --signature-algorithm option was given,
		 * output signature algorithm for certificate(s).
		 */
		if (sig_algo) {
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
			X509_get0_signature(&asn1_sig, &sig_type, crt);
#else
			sig_type = crt->sig_alg;
			asn1_sig = crt->signature;
#endif

			BIO_printf(bp, "--- Signature Algorithm: ");
			sig_type_err = i2a_ASN1_OBJECT(bp, sig_type->algorithm);
			BIO_printf(bp, "\n");

			if (is_error(sig_type_err, -1) || is_error(sig_type_err, 0)) {
				BIO_printf(bp, "Error: Could not get signature algorithm.\n");
			}
		}

		/**
		 * If --validity option was given, output the
		 * range of Not Before/Not After timestamps.
		 */
		if (validity) {
			BIO_printf(bp, "%s", "--- Validity:\n");
			BIO_printf(bp, "%4s%s", "", "--- Not Before: ");
			ASN1_TIME_print(bp, X509_get_notBefore(crt));
			BIO_printf(bp, "\n");
			BIO_printf(bp, "%4s%s", "", "--- Not After: ");
			ASN1_TIME_print(bp, X509_get_notAfter(crt));
			BIO_printf(bp, "\n");
		}

		/**
		 * Output raw certificate contents if --raw option was specified.
		 */
		if (raw) {
			BIO_printf(bp, "\n");
			PEM_write_bio_PUBKEY(bp, pubkey);
			BIO_printf(bp, "\n");
			PEM_write_bio_X509(bp, crt);
			BIO_printf(bp, "\n");
		}
	}

	EVP_PKEY_free(pubkey);
	BIO_free(bp);
	SSL_free(ssl);
	close(server);
	X509_free(crt);
	X509_free(tcrt);
	SSL_CTX_free(ctx);
	ERR_free_strings();

	return EXIT_SUCCESS;

on_error:
	ERR_print_errors(bp);
	EVP_PKEY_free(pubkey);
	BIO_free(bp);
	SSL_free(ssl);
	close(server);
	X509_free(crt);
	X509_free(tcrt);
	SSL_CTX_free(ctx);
	ERR_free_strings();

	return EXIT_FAILURE;
}
