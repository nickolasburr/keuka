/**
 * sock.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "sock.h"

/**
 * Create TCP socket.
 */
int mksock (char *url, BIO *bp) {
	int sockfd, port;
	char hostname[256] = "";
	char port_num[6] = "443";
	char protocol[6] = "";
	char *tmp_ptr = NULL;
	struct hostent *host;
	struct sockaddr_in dest_addr;

	/**
	 * Remove trailing slash, if applicable.
	 */
	if (url[strlen(url)] == '/') {
		url[strlen(url)] = '\0';
	}

	/**
	 * Protocol type (e.g. https).
	 */
	strncpy(protocol, url, (strchr(url, ':') - url));

	/**
	 * Hostname (e.g. www.example.com)
	 */
	strncpy(hostname, strstr(url, "://") + 3, sizeof(hostname));

	/**
	 * Port (if applicable).
	 */
	if (strchr(hostname, ':')) {
		tmp_ptr = strchr(hostname, ':');
		strncpy(port_num, tmp_ptr + 1,  sizeof(port_num));
		*tmp_ptr = '\0';
	}

	port = atoi(port_num);

	/**
	 * Verify the hostname is resolvable.
	 */
	if (is_null(gethostbyname(hostname))) {
		BIO_printf(bp, "Error: Cannot resolve hostname %s.\n", hostname);

		exit(EXIT_FAILURE);
	}

	host = gethostbyname(hostname);

	/**
	 * Set TCP socket.
	 */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = *(long*)(host->h_addr);

	/**
	 * Initialize the rest of the struct.
	 */
	memset(&(dest_addr.sin_zero), '\0', 8);
	tmp_ptr = inet_ntoa(dest_addr.sin_addr);

	/**
	 * Return error if we're not able to connect.
	 */
	if (is_error(connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr)), -1)) {
		BIO_printf(bp, "Error: Cannot connect to host %s [%s] on port %d.\n", hostname, tmp_ptr, port);

		return -1;
	}

	return sockfd;
}
