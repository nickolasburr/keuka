/**
 * main.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "main.h"

int main (int argc, char **argv) {
	int server = 0;
	char *dest_url = "https://www.example.com";
	const SSL_METHOD *method;
	BIO *certbio, *outbio;
	X509 *cert = NULL;
	X509_NAME *certname = NULL;
	SSL_CTX *ctx;
	SSL *ssl;

	/**
	 * Run OpenSSL initialization tasks.
	 */
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();

	/**
	 * Initialize BIO streams.
	 */
	certbio = BIO_new(BIO_s_file());
	outbio  = BIO_new_fp(stdout, BIO_NOCLOSE);

	/**
	 * Initialize OpenSSL library.
	 */
	if (SSL_library_init() < 0) {
		BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
	}

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
	 * Establish new connection state.
	 */
	ssl = SSL_new(ctx);

	/**
	 * Make TCP socket connection.
	 */
	server = mksock(dest_url, outbio);

	if (server != 0) {
		BIO_printf(outbio, "Successfully made the TCP connection to: %s.\n", dest_url);
	}

	/**
	 * Attach the SSL session to the socket.
	 */
	SSL_set_fd(ssl, server);

	/**
	 * Bridge the connection. Throw an exception if an error was encountered.
	 */
	if (SSL_connect(ssl) != 1) {
		BIO_printf(outbio, "Error: Could not build a SSL session to: %s.\n", dest_url);
	} else {
		BIO_printf(outbio, "Successfully enabled SSL/TLS session to: %s.\n", dest_url);
	}

	/**
	 * Load remote certificate into X509 structure.
	 */
	cert = SSL_get_peer_certificate(ssl);

	if (is_null(cert)) {
		BIO_printf(outbio, "Error: Could not get a certificate from: %s.\n", dest_url);
	} else {
		BIO_printf(outbio, "Retrieved the server's certificate from: %s.\n", dest_url);
	}

	/**
	 * Get certificate information.
	 */
	certname = X509_NAME_new();
	certname = X509_get_subject_name(cert);

	/**
	 * Output certificate subject below.
	 */
	BIO_printf(outbio, "Displaying the certificate subject data:\n");
	X509_NAME_print_ex(outbio, certname, 0, 0);
	BIO_printf(outbio, "\n");

	/**
	 * Run cleanup tasks.
	 */
	SSL_free(ssl);
	close(server);
	X509_free(cert);
	SSL_CTX_free(ctx);
	BIO_printf(outbio, "Finished SSL/TLS connection with server: %s.\n", dest_url);

	return EXIT_SUCCESS;
}
