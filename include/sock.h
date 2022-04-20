/**
 * sock.h
 *
 * Copyright (C) 2017 Nickolas Burr <nickolasburr@gmail.com>
 */

#ifndef KEUKA_SOCK_H
#define KEUKA_SOCK_H

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"
#include "error.h"
#include "ssl.h"
#include "utils.h"

int mksock(char *, BIO *);

#endif /* KEUKA_SOCK_H */
