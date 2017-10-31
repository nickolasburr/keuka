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
 * keuka -A -C -i -m github.com
 */

int main (int argc, char **argv) {
	size_t crt_index, sig_bytes;
	int opt_index, arg_index, last_index, sig_type_err;
	int chain, cipher, issuer, meth, serial, server, raw, sig_algo, validity;
	const char *hostname;
	char port[MAX_PORT_LENGTH],
	     scheme[MAX_SCHEME_LENGTH],
	     url[MAX_URL_LENGTH];
	const SSL_METHOD *method;
	ASN1_INTEGER *asn1_serial;
	ASN1_STRING *asn1_sig;
	X509_ALGOR *sig_type;
	STACK_OF(X509) *fullchain;
	BIO *bp;
	X509 *crt, *tcrt;
	X509_NAME *crtname, *tcrtname;
	SSL_CTX *ctx;
	SSL *ssl;

	last_index = (argc - 1);
	server = 0;

	/**
	 * Options:
	 *
	 * --chain, -c: Show entire peer certificate chain.
	 * --cipher, -C: Show cipher used for exchange.
	 * --issuer, -i: Show certificate issuer information.
	 * --method, -m: Show method version for exchange.
	 * --raw, -r: Show raw certificate contents.
	 * --serial, -S: Show certificate serial number.
	 * --signature-algorithm, -A: Show signature algorithm.
	 * --validity, -V: Show certificate Not Before/Not After validity range.
	 * --help, -h: Show help information and usage examples.
	 * --version, -v: Show version information.
	 */

	/**
	 * Initialize defaults.
	 */
	chain = 0;
	cipher = 0;
	issuer = 0;
	meth = 0;
	raw = 0;
	serial = 0;
	sig_algo = 0;
	validity = 0;

	if (in_array("--help", argv, argc) ||
	    in_array("-h", argv, argc)) {

		usage();
		exit(EXIT_SUCCESS);
	}

	if (in_array("--version", argv, argc) ||
	    in_array("-v", argv, argc)) {

		fprintf(stdout, "%s\n", KEUKA_VERSION);
		exit(EXIT_SUCCESS);
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
		fprintf(stderr, "Error: Hostname exceeds maximum length\n");

		exit(EXIT_FAILURE);
	}

	/**
	 * The last element in argv should be the peer hostname.
	 */
	hostname = argv[last_index];

	/**
	 * If --chain option was given, output
	 * the entire peer certificate chain.
	 */
	if (in_array("--chain", argv, argc) ||
	    in_array("-c", argv, argc)) {

		chain = 1;
	}

	/**
	 * If --cipher option was given, output
	 * the cipher used for the exchange.
	 */
	if (in_array("--cipher", argv, argc) ||
	    in_array("-C", argv, argc)) {

		cipher = 1;
	}

	/**
	 * If --issuer option was given, output
	 * issuer information for certificate.
	 */
	if (in_array("--issuer", argv, argc) ||
	    in_array("-i", argv, argc)) {

		issuer = 1;
	}

	/**
	 * If --method option was given, output
	 * version of method used for handshake.
	 */
	if (in_array("--method", argv, argc) ||
	    in_array("-m", argv, argc)) {

		meth = 1;
	}

	/**
	 * If --raw option was given, output
	 * raw certificate contents to stdout.
	 */
	if (in_array("--raw", argv, argc) ||
	    in_array("-r", argv, argc)) {

		raw = 1;
	}

	/**
	 * If --serial option was given, output
	 * serial number for the certificate(s).
	 */
	if (in_array("--serial", argv, argc) ||
	    in_array("-S", argv, argc)) {

		serial = 1;
	}

	/**
	 * If --signature-algorithm option was given,
	 * output signature algorithm used for certificate(s).
	 */
	if (in_array("--signature-algorithm", argv, argc) ||
	    in_array("-A", argv, argc)) {

		sig_algo = 1;
	}

	/**
	 * If --validity option was given, output
	 * Not Before/Not After validity time range.
	 */
	if (in_array("--validity", argv, argc) ||
	    in_array("-V", argv, argc)) {

		validity = 1;
	}

	/**
	 * Check if --scheme option was given.
	 * If not, set default scheme to https.
	 *
	 * @note: Might get rid of this or implement differently.
	 */
	if (in_array("--scheme", argv, argc) ||
	    in_array("-s", argv, argc)) {

		opt_index = (index_of("--scheme", argv, argc) != NOT_FOUND)
		          ? index_of("--scheme", argv, argc)
		          : index_of("-s", argv, argc);

		if ((arg_index = (opt_index + 1)) > last_index) {
			fprintf(stderr, "--scheme: Missing argument\n");

			exit(EXIT_FAILURE);
		}

		copy(scheme, argv[arg_index]);
	} else {
		copy(scheme, "https");
	}

	/**
	 * Assemble URL for request.
	 */
	copy(url, scheme);
	concat(url, "://");
	concat(url, hostname);

	/**
	 * Run OpenSSL initialization tasks.
	 */
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();

	/**
	 * Initialize OpenSSL library.
	 */
	SSL_library_init();

	/**
	 * Initialize BIO stream pointer.
	 */
	bp = BIO_new_fp(stdout, BIO_NOCLOSE);

	/**
	 * Method for peer connection.
	 */
	method = SSLv23_client_method();

	/**
	 * Establish new SSL context.
	 */
	if (is_null(ctx = SSL_CTX_new(method))) {
		BIO_printf(bp, "Unable to establish new SSL context structure.\n");

		goto on_error;
	}

	/**
	 * Make TCP socket connection.
	 *
	 * @todo: Add check, throw exception on failure.
	 */
	server = mksock(url, bp);

	if (is_error(server, -1)) {
		fprintf(stderr, "Please specify a valid hostname.\n");

		exit(EXIT_FAILURE);
	}

	/**
	 * Establish connection, set state in client mode.
	 */
	ssl = SSL_new(ctx);
	SSL_set_connect_state(ssl);

	/**
	 * If --method option was given, output
	 * version of method used for handshake.
	 */
	if (meth) {
		BIO_printf(bp, "--- Method: %s\n", SSL_get_version(ssl));
	}

	/**
	 * Attach the SSL session to the socket.
	 */
	if (is_error(SSL_set_fd(ssl, server), -1)) {
		BIO_printf(bp, "Error: Unable to attach SSL session to socket.\n");

		goto on_error;
	}

	/**
	 * Bridge the connection.
	 */
	if (SSL_connect(ssl) != 1) {
		BIO_printf(bp, "Error: Could not build an SSL session to %s.\n", url);

		goto on_error;
	}

	/**
	 * Print cipher used if --cipher was given.
	 */
	if (cipher) {
		BIO_printf(bp, "--- Cipher: %s\n", SSL_CIPHER_get_name(SSL_get_current_cipher(ssl)));
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

		for (crt_index = 0; crt_index < sk_X509_num(fullchain); crt_index += 1) {
			tcrt = sk_X509_value(fullchain, crt_index);
			tcrtname = X509_get_subject_name(tcrt);

			/**
			 * Output certificate chain.
			 */
			BIO_printf(bp, "%5d: ", (int) crt_index);
			BIO_printf(bp, "--- Subject: ");
			X509_NAME_print_ex(bp, tcrtname, 0, XN_FLAG_SEP_CPLUS_SPC);

			/**
			 * If --issuer option was given, output certificate issuer information.
			 */
			if (issuer) {
				BIO_printf(bp, "\n");
				BIO_printf(bp, "%7s%s", "", "--- Issuer: ");
				X509_NAME_print_ex(bp, X509_get_issuer_name(tcrt), 0, XN_FLAG_SEP_CPLUS_SPC);
			}

			/**
			 * If --serial option was given, output ASN1 serial.
			 */
			if (serial) {
				BIO_printf(bp, "\n");
				BIO_printf(bp, "%7s%s", "", "--- Serial: ");
				i2a_ASN1_INTEGER(bp, X509_get_serialNumber(tcrt));
			}

			/**
			 * If --signature-algorithm option was given,
			 * output signature algorithm for certificate(s).
			 */
			if (sig_algo) {
				sig_type = tcrt->sig_alg;
				asn1_sig = tcrt->signature;

				BIO_printf(bp, "\n");
				BIO_printf(bp, "%7s%s", "", "--- Signature Algorithm: ");
				sig_type_err = i2a_ASN1_OBJECT(bp, sig_type->algorithm);

				if (is_error(sig_type_err, -1) || is_error(sig_type_err, 0)) {
					BIO_printf(bp, "Error: Could not get signature algorithm.\n");
				}
			}

			/**
			 * If --validity option was given, output the
			 * range of Not Before/Not After timestamps.
			 */
			if (validity) {
				BIO_printf(bp, "\n");
				BIO_printf(bp, "%7s%s", "", "--- Validity:\n");
				BIO_printf(bp, "%11s%s", "", "--- Not Before: ");
				ASN1_TIME_print(bp, X509_get_notBefore(tcrt));
				BIO_printf(bp, "\n");
				BIO_printf(bp, "%11s%s", "", "--- Not After: ");
				ASN1_TIME_print(bp, X509_get_notAfter(tcrt));
			}

			BIO_printf(bp, "\n");
		}

		/**
		 * Output raw certificate contents if --raw option was specified.
		 */
		if (raw) {
			BIO_printf(bp, "\n");

			for (crt_index = 0; crt_index < sk_X509_num(fullchain); crt_index += 1) {
				PEM_write_bio_X509(bp, sk_X509_value(fullchain, crt_index));
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

		/**
		 * Get certificate subject.
		 */
		crtname = X509_get_subject_name(crt);

		/**
		 * Output certificate subject.
		 */
		BIO_printf(bp, "--- Subject: ");
		X509_NAME_print_ex(bp, crtname, 0, XN_FLAG_SEP_COMMA_PLUS);
		BIO_printf(bp, "\n");

		/**
		 * If --issuer option was given, output certificate issuer information.
		 */
		if (issuer) {
			BIO_printf(bp, "--- Issuer: ");
			X509_NAME_print_ex(bp, X509_get_issuer_name(crt), 0, XN_FLAG_SEP_CPLUS_SPC);
			BIO_printf(bp, "\n");
		}

		/**
		 * If --serial option was given, output ASN1 serial.
		 */
		if (serial) {
			BIO_printf(bp, "--- Serial: ");
			i2a_ASN1_INTEGER(bp, X509_get_serialNumber(crt));
			BIO_printf(bp, "\n");
		}

		/**
		 * If --signature-algorithm option was given,
		 * output signature algorithm for certificate(s).
		 */
		if (sig_algo) {
			sig_type = crt->sig_alg;
			asn1_sig = crt->signature;

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
			PEM_write_bio_X509(bp, crt);
		}
	}

	/**
	 * Run cleanup tasks.
	 */
	BIO_free(bp);
	SSL_free(ssl);
	close(server);
	X509_free(crt);
	SSL_CTX_free(ctx);
	ERR_free_strings();

	return EXIT_SUCCESS;

on_error:
	BIO_printf(bp, "\n");
	ERR_print_errors(bp);
	BIO_free(bp);
	SSL_free(ssl);
	close(server);
	X509_free(crt);
	SSL_CTX_free(ctx);
	ERR_free_strings();

	return EXIT_FAILURE;
}
