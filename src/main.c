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
	     url[MAX_URL_LENGTH],
	     method_name[6];
	const SSL_CIPHER *cipher;
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
	} else {
		fprintf(stderr, "Error: Missing --hostname\n");

		exit(EXIT_FAILURE);
	}

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
	 * Check if --method option was given.
	 * If not, method will be negotiated.
	 */
	if (in_array("--method", argv, argc) ||
	    in_array("-M", argv, argc)) {

		opt_index = (index_of("--method", argv, argc) != NOT_FOUND)
		          ? index_of("--method", argv, argc)
		          : index_of("-M", argv, argc);

		if ((arg_index = (opt_index + 1)) > last_index) {
			fprintf(stderr, "--method: Missing argument\n");

			exit(EXIT_FAILURE);
		}

		copy(method_name, argv[arg_index]);

		/**
		 * Force specific method for handshake.
		 */
		switch (get_const_from_key(method_name)) {
			case OPT_SSLV2:
				method = SSLv2_client_method();

				/**
				 * Manually clear SSL_OP_NO_SSLv2 so we
				 * can downgrade the handshake properly.
				 */
				SSL_CTX_clear_options(ctx, SSL_OP_NO_SSLv2);
				SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1 | SSL_OP_NO_TLSv1_2);

				break;
			case OPT_SSLV3:
				method = SSLv3_client_method();
				SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1 | SSL_OP_NO_TLSv1_2);

				break;
			case OPT_TLSV1:
				method = TLSv1_client_method();
				SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1_1 | SSL_OP_NO_TLSv1_2);

				break;
			case OPT_TLSV1_1:
				method = TLSv1_1_client_method();
				SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_2);

				break;
			case OPT_TLSV1_2:
				method = TLSv1_2_client_method();
				SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1);

				break;
			default:
				fprintf(stderr, "%s is not a valid method type\n", method_name);

				exit(EXIT_FAILURE);
		}
	} else {
		method = SSLv23_client_method();
	}

	/**
	 * Initialize BIO stream.
	 */
	outbio = BIO_new_fp(stdout, BIO_NOCLOSE);

	/**
	 * Establish new SSL context.
	 */
	if (is_null(ctx = SSL_CTX_new(method))) {
		BIO_printf(outbio, "Unable to establish new SSL context structure.\n");

		goto on_error;
	}

	/**
	 * Establish connection state.
	 */
	ssl = SSL_new(ctx);

	/**
	 * Make TCP socket connection.
	 */
	server = mksock(url, outbio);

	if (server != 0) {
		BIO_printf(outbio, "Successfully made TCP connection to %s.\n\n", url);
	}

	/**
	 * Attach the SSL session to the socket.
	 */
	SSL_set_fd(ssl, server);

	/**
	 * Bridge the connection.
	 */
	if (SSL_connect(ssl) != 1) {
		BIO_printf(outbio, "Error: Could not build an SSL session to %s.\n", url);

		goto on_error;
	}

	cipher = SSL_get_current_cipher(ssl);

	BIO_printf(outbio, "Using cipher %s\n", SSL_CIPHER_get_name(cipher));
	BIO_printf(outbio, "\n");

	/**
	 * Load remote certificate into X509 structure.
	 */
	crt = SSL_get_peer_certificate(ssl);

	if (is_null(crt)) {
		BIO_printf(outbio, "Error: Could not get certificate from %s.\n", url);

		goto on_error;
	}

	/**
	 * Get certificate subject.
	 */
	crtname = X509_get_subject_name(crt);

	/**
	 * Output certificate subject.
	 */
	BIO_printf(outbio, "Certificate subject:\n\n");
	X509_NAME_print_ex(outbio, crtname, 2, XN_FLAG_SEP_MULTILINE);
	BIO_printf(outbio, "\n\n");

	/**
	 * Get certificate chain.
	 */
	chain = SSL_get_peer_cert_chain(ssl);

	if (is_null(chain)) {
		BIO_printf(outbio, "Error: Could not get certificate chain from %s.\n", url);
	}

	BIO_printf(outbio, "Certificate chain:\n\n");

	for (crt_index = 0; crt_index < sk_X509_num(chain); crt_index += 1) {
		tcrt = sk_X509_value(chain, crt_index);
		tcrtname = X509_get_subject_name(tcrt);

		/**
		 * Output certificate chain.
		 */
		X509_NAME_print_ex(outbio, tcrtname, 2, XN_FLAG_SEP_MULTILINE);
		BIO_printf(outbio, "\n\n");
		PEM_write_bio_X509(outbio, tcrt);
		BIO_printf(outbio, "\n");
	}

	/**
	 * Run cleanup tasks.
	 */
	SSL_free(ssl);
	close(server);
	X509_free(crt);
	SSL_CTX_free(ctx);

	return EXIT_SUCCESS;

on_error:
	SSL_free(ssl);
	close(server);
	X509_free(crt);
	SSL_CTX_free(ctx);

	return EXIT_FAILURE;
}
