/**
 * main.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "main.h"

int main () {
	int server = 0;
	char *dest_url = "https://www.example.com";
	const SSL_METHOD *method;
	BIO *certbio, *outbio;
	X509 *cert = NULL;
	X509_NAME *certname = NULL;
	SSL_CTX *ctx;
	SSL *ssl;

	/* ---------------------------------------------------------- *
	 * These function calls initialize openssl for correct work.  *
	 * ---------------------------------------------------------- */
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();

	/* ---------------------------------------------------------- *
	 * Create the Input/Output BIO's.                             *
	 * ---------------------------------------------------------- */
	certbio = BIO_new(BIO_s_file());
	outbio  = BIO_new_fp(stdout, BIO_NOCLOSE);

	/* ---------------------------------------------------------- *
	 * initialize SSL library and register algorithms             *
	 * ---------------------------------------------------------- */
	if (SSL_library_init() < 0) {
		BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
	}

	/* ---------------------------------------------------------- *
	 * Set SSLv2 client hello, also announce SSLv3 and TLSv1      *
	 * ---------------------------------------------------------- */
	method = SSLv23_client_method();

	/* ---------------------------------------------------------- *
	 * Try to create a new SSL context                            *
	 * ---------------------------------------------------------- */
	if (is_null(ctx = SSL_CTX_new(method))) {
		BIO_printf(outbio, "Unable to create a new SSL context structure.\n");
	}

	/* ---------------------------------------------------------- *
	 * Disabling SSLv2 will leave v3 and TSLv1 for negotiation    *
	 * ---------------------------------------------------------- */
	SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

	/* ---------------------------------------------------------- *
	 * Create new SSL connection state object                     *
	 * ---------------------------------------------------------- */
	ssl = SSL_new(ctx);

	/**
	 * Make TCP socket connection.
	 */
	server = mksock(dest_url, outbio);

	if (server != 0) {
		BIO_printf(outbio, "Successfully made the TCP connection to: %s.\n", dest_url);
	}

	/* ---------------------------------------------------------- *
	 * Attach the SSL session to the socket descriptor            *
	 * ---------------------------------------------------------- */
	SSL_set_fd(ssl, server);

	/* ---------------------------------------------------------- *
	 * Try to SSL-connect here, returns 1 for success             *
	 * ---------------------------------------------------------- */
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

	/* ---------------------------------------------------------- *
	 * extract various certificate information                    *
	 * -----------------------------------------------------------*/
	certname = X509_NAME_new();
	certname = X509_get_subject_name(cert);

	/* ---------------------------------------------------------- *
	 * display the cert subject here                              *
	 * -----------------------------------------------------------*/
	BIO_printf(outbio, "Displaying the certificate subject data:\n");
	X509_NAME_print_ex(outbio, certname, 0, 0);
	BIO_printf(outbio, "\n");

	/* ---------------------------------------------------------- *
	 * Free the structures we don't need anymore                  *
	 * -----------------------------------------------------------*/
	SSL_free(ssl);
	close(server);
	X509_free(cert);
	SSL_CTX_free(ctx);
	BIO_printf(outbio, "Finished SSL/TLS connection with server: %s.\n", dest_url);

	return EXIT_SUCCESS;
}
