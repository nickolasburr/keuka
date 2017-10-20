/**
 * sock.c
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#include "sock.h"

/**
 * Create a Unix socket and connect via TCP to server.
 */
int mksock (char url_str[], BIO *out) {
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
	if (url_str[strlen(url_str)] == '/') {
		url_str[strlen(url_str)] = '\0';
	}

	/* ---------------------------------------------------------- *
	 * the first : ends the protocol string, i.e. http            *
	 * ---------------------------------------------------------- */
	strncpy(proto, url_str, (strchr(url_str, ':') - url_str));

	/* ---------------------------------------------------------- *
	 * the hostname starts after the "://" part                   *
	 * ---------------------------------------------------------- */
	strncpy(hostname, strstr(url_str, "://") + 3, sizeof(hostname));

	/* ---------------------------------------------------------- *
	 * if the hostname contains a colon :, we got a port number   *
	 * ---------------------------------------------------------- */
	if (strchr(hostname, ':')) {
		tmp_ptr = strchr(hostname, ':');
		/* the last : starts the port number, if avail, i.e. 8443 */
		strncpy(portnum, tmp_ptr + 1,  sizeof(portnum));
		*tmp_ptr = '\0';
	}

	port = atoi(portnum);

	if ((host = gethostbyname(hostname)) == NULL) {
		BIO_printf(out, "Error: Cannot resolve hostname %s.\n",  hostname);
		abort();
	}

	/* ---------------------------------------------------------- *
	 * create the basic TCP socket                                *
	 * ---------------------------------------------------------- */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	dest_addr.sin_family=AF_INET;
	dest_addr.sin_port=htons(port);
	dest_addr.sin_addr.s_addr = *(long*)(host->h_addr);

	/* ---------------------------------------------------------- *
	 * Zeroing the rest of the struct                             *
	 * ---------------------------------------------------------- */
	memset(&(dest_addr.sin_zero), '\0', 8);

	tmp_ptr = inet_ntoa(dest_addr.sin_addr);

	/* ---------------------------------------------------------- *
	 * Try to make the host connect here                          *
	 * ---------------------------------------------------------- */
	if (connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr)) == -1) {
		BIO_printf(out, "Error: Cannot connect to host %s [%s] on port %d.\n", hostname, tmp_ptr, port);
	}

	return sockfd;
}
