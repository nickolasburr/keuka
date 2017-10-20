/**
 * main.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "main.h"

int main (int argc, char **argv) {
	size_t index;
	int opt_index, arg_index, last_index, server = 0;
	char *dest_url = "https://www.example.com";
	char host[256];
	const SSL_METHOD *method;
	STACK_OF(X509) *chain;
	BIO *outbio;
	X509 *crt;
	X509_NAME *crtname;
	SSL_CTX *ctx;
	SSL *ssl;

	last_index = (argc - 1);

	/**
	 * Check if --domain option was given.
	 * If not, throw an exception and exit.
	 */
	if (in_array("--domain", argv, argc) ||
	    in_array("-D", argv, argc)) {

		opt_index = (index_of("--domain", argv, argc) != NOT_FOUND)
		          ? index_of("--domain", argv, argc)
		          : index_of("-D", argv, argc);

		if ((arg_index = (opt_index + 1)) > last_index) {
			fprintf(stderr, "--domain: Missing argument\n");

			exit(EXIT_FAILURE);
		}

		copy(host, argv[arg_index]);
	}

	fprintf(stdout, "Hostname: %s\n", host);

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
	server = mksock(dest_url, outbio);

	if (server != 0) {
		BIO_printf(outbio, "Successfully made the TCP connection to: %s.\n\n", dest_url);
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
		BIO_printf(outbio, "Error: Could not build a SSL session to: %s.\n", dest_url);
	}

	/**
	 * Load remote certificate into X509 structure.
	 */
	crt = SSL_get_peer_certificate(ssl);

	if (is_null(crt)) {
		BIO_printf(outbio, "Error: Could not get certificate from: %s.\n", dest_url);
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
		BIO_printf(outbio, "Error: Could not get certificate chain from: %s.\n", dest_url);
	}

	BIO_printf(outbio, "Displaying certificate chain data:\n\n");

	for (index = 0; index < sk_X509_num(chain); index += 1) {
		X509 *ccrt = sk_X509_value(chain, index);
		X509_NAME *ccrtname = X509_get_subject_name(ccrt);

		/**
		 * Output certificate chain.
		 */
		X509_NAME_print_ex(outbio, ccrtname, 2, XN_FLAG_SEP_MULTILINE);
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
