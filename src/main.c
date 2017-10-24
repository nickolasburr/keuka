/**
 * main.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "main.h"

int main (int argc, char **argv) {
	size_t crt_index;
	int opt_index, arg_index, last_index, server;
	char hostname[MAX_HOSTNAME_LENGTH],
	     port[MAX_PORT_LENGTH],
	     scheme[MAX_SCHEME_LENGTH],
	     url[MAX_URL_LENGTH];
	const SSL_METHOD *method;
	STACK_OF(X509) *chain;
	BIO *outbio;
	X509 *crt, *tcrt;
	X509_NAME *crtname, *tcrtname;
	SSL_CTX *ctx;
	SSL *ssl;

	last_index = (argc - 1);
	server = 0;

	/**
	 * Check if --hostname option was given.
	 * If not, throw an exception and exit.
	 *
	 * @todo: This should be --hostname instead.
	 */
	if (in_array("--hostname", argv, argc) ||
	    in_array("-H", argv, argc)) {

		opt_index = (index_of("--hostname", argv, argc) != NOT_FOUND)
		          ? index_of("--hostname", argv, argc)
		          : index_of("-H", argv, argc);

		if ((arg_index = (opt_index + 1)) > last_index) {
			fprintf(stderr, "--hostname: Missing argument\n");

			exit(EXIT_FAILURE);
		}

		copy(hostname, argv[arg_index]);
	}

	fprintf(stdout, "Hostname: %s\n", hostname);

	/**
	 * Check if --scheme option was given.
	 * If not, set default scheme to https.
	 *
	 * @todo: Whitelist schemes and throw an
	 *        exception on unsupported schemes.
	 */
	if (in_array("--scheme", argv, argc) ||
	    in_array("-S", argv, argc)) {

		opt_index = (index_of("--scheme", argv, argc) != NOT_FOUND)
		          ? index_of("--scheme", argv, argc)
		          : index_of("-S", argv, argc);

		if ((arg_index = (opt_index + 1)) > last_index) {
			fprintf(stderr, "--scheme: Missing argument\n");

			exit(EXIT_FAILURE);
		}

		copy(scheme, argv[arg_index]);
	} else {
		copy(scheme, "https");
	}

	fprintf(stdout, "Scheme: %s\n", scheme);

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
	 * Initialize BIO stream, set handshake method.
	 */
	outbio = BIO_new_fp(stdout, BIO_NOCLOSE);
	method = SSLv23_client_method();

	/**
	 * Establish new SSL context.
	 */
	if (is_null(ctx = SSL_CTX_new(method))) {
		BIO_printf(outbio, "Unable to create a new SSL context structure.\n");
	}

	/**
	 * Disabling SSLv2 will leave SSLv3 and TLSv1 available for negotiation.
	 */
	SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

	/**
	 * Establish connection state.
	 */
	ssl = SSL_new(ctx);

	/**
	 * Make TCP socket connection.
	 */
	server = mksock(url, outbio);

	if (server != 0) {
		BIO_printf(outbio, "Successfully made TCP connection to: %s.\n\n", url);
	}

	/**
	 * Attach the SSL session to the socket.
	 */
	SSL_set_fd(ssl, server);

	/**
	 * Bridge the connection.
	 *
	 * @todo: Throw an exception if an error was encountered.
	 */
	if (SSL_connect(ssl) != 1) {
		BIO_printf(outbio, "Error: Could not build a SSL session to: %s.\n", url);
	}

	/**
	 * Load remote certificate into X509 structure.
	 */
	crt = SSL_get_peer_certificate(ssl);

	if (is_null(crt)) {
		BIO_printf(outbio, "Error: Could not get certificate from: %s.\n", url);
	}

	/**
	 * Get certificate subject.
	 */
	crtname = X509_get_subject_name(crt);

	/**
	 * Output certificate subject.
	 */
	BIO_printf(outbio, "Displaying certificate subject data:\n\n");
	X509_NAME_print_ex(outbio, crtname, 2, XN_FLAG_SEP_MULTILINE);
	BIO_printf(outbio, "\n\n");

	/**
	 * Get certificate chain.
	 */
	chain = SSL_get_peer_cert_chain(ssl);

	if (is_null(chain)) {
		BIO_printf(outbio, "Error: Could not get certificate chain from: %s.\n", url);
	}

	BIO_printf(outbio, "Displaying certificate chain data:\n\n");

	for (crt_index = 0; crt_index < sk_X509_num(chain); crt_index += 1) {
		tcrt = sk_X509_value(chain, crt_index);
		tcrtname = X509_get_subject_name(tcrt);

		/**
		 * Output certificate chain.
		 */
		X509_NAME_print_ex(outbio, tcrtname, 2, XN_FLAG_SEP_MULTILINE);
		BIO_printf(outbio, "\n\n");
	}

	/**
	 * Run cleanup tasks.
	 */
	SSL_free(ssl);
	close(server);
	X509_free(crt);
	SSL_CTX_free(ctx);

	return EXIT_SUCCESS;
}
