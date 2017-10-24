/**
 * sock.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "sock.h"

/**
 * Create TCP socket.
 */
int mksock (char *url, BIO *out) {
	int sockfd, port;
	char hostname[256] = "";
	char portnum[6] = "443";
	char proto[6] = "";
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
	strncpy(proto, url, (strchr(url, ':') - url));

	/**
	 * Hostname (e.g. www.example.com)
	 */
	strncpy(hostname, strstr(url, "://") + 3, sizeof(hostname));

	/**
	 * Port (if applicable).
	 */
	if (strchr(hostname, ':')) {
		tmp_ptr = strchr(hostname, ':');
		strncpy(portnum, tmp_ptr + 1,  sizeof(portnum));
		*tmp_ptr = '\0';
	}

	port = atoi(portnum);

	if (is_null(host = gethostbyname(hostname))) {
		BIO_printf(out, "Error: Cannot resolve hostname %s.\n",  hostname);
		abort();
	}

	/**
	 * Create TCP socket.
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

	if (is_error(connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr)))) {
		BIO_printf(out, "Error: Cannot connect to host %s [%s] on port %d.\n", hostname, tmp_ptr, port);
	}

	return sockfd;
}
