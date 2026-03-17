# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**keuka** is a CLI tool written in C that analyzes SSL/TLS handshakes — it connects to a remote host, performs a TLS handshake, and extracts certificate and cryptographic information. Current version: `1.0.6` (defined in `include/common.h`).

## Build Commands

```sh
make              # Compile the keuka binary
make clean        # Remove compiled binary and debug symbols
make install      # Install to /usr/local/bin and man page to /usr/local/share/man/man1
make uninstall    # Remove installed files
make prefix=/path # Install to a custom prefix
```

**Dependencies**: OpenSSL (expected at `/usr/local/opt/openssl/`), POSIX threads, zlib. On macOS, CoreFoundation and Security frameworks are linked automatically.

There is no automated test suite or linter configured.

## Architecture

All source is in `src/` with corresponding headers in `include/`. The build produces a single statically-linked binary.

### Module Responsibilities

- **main.c** — Entry point. Parses arguments with `getopt_long`, initializes OpenSSL, creates the TCP socket, performs the TLS handshake, and extracts/displays certificate fields based on selected options. Error cleanup uses a `goto on_error` pattern.
- **sock.c** — `mksock()`: resolves hostname with `gethostbyname()`, creates and connects a TCP socket, returns the fd or -1 on failure.
- **argv.c** — Defines the 14 `long_options[]` entries and 6 TLS protocol version entries. `usage()` prints help; `get_bitmask_from_key()` maps protocol name strings to bitmasks.
- **mem.c** — Custom allocators (`Mem_alloc`, `Mem_calloc`, `Mem_resize`, `Mem_free`) that raise a `Mem_Failed` exception on failure. Use the macros `ALLOC`, `CALLOC`, `NEW`, `NEW0`, `FREE`, `RESIZE` defined in `include/mem.h`.
- **except.c** — Exception framework using `setjmp`/`longjmp`. Provides `TRY / EXCEPT / ELSE / FINALLY / END_TRY` macros and `Except_raise()`. Pattern is from *C Interfaces and Implementations*.
- **assert.c** — Overrides `assert()` to raise `Assert_Failed` instead of calling `abort()`.
- **clock.c** — `get_elapsed_ticks()` returns elapsed CPU time in seconds as a `double`.
- **utils.c** — String helpers (`copy`, `concat`, `compare`, `length`, `base_name`, `dir_name`, `index_of`, `in_array`) and filesystem type checks (`is_file`, `is_dir`, `is_fifo`, etc.).
- **error.c** — `is_error()` helper for checking error codes.

### Headers-only modules

- **include/ssl.h** — Aggregates all OpenSSL includes (BIO, SSL, X509, PEM, EVP, etc.).
- **include/common.h** — Standard library includes, `KEUKA_VERSION`, and `__APPLE__` platform detection.
- **include/format.h** — Output indicator strings: `-->` (outbound), `<--` (inbound), `---` (neutral).

### OpenSSL compatibility

`main.c` uses `#if OPENSSL_VERSION_NUMBER` guards to handle API differences between OpenSSL 1.0.x and 1.1.x+.

### Output timing

`main.c` wraps each major operation (socket connect, SSL handshake, certificate extraction) with `get_elapsed_ticks()` calls and prints elapsed time via OpenSSL `BIO` unless `--quiet` is passed.
