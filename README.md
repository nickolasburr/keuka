# Keuka

Keuka is a key exchange utility for analyzing and modifying certificate and key information during SSL/TLS handshakes.

## Installation

There are currently two installation methods: Homebrew and manual.

### Homebrew

```shell
brew tap nickolasburr/pfa
brew install keuka
```

### Manual

By default, `keuka` is installed to `/usr/local/bin`. You can set the `prefix` variable when running `make` to specify an alternate path.

```shell
make
make install
```

## Options

+ `--hostname`, `-H`: Hostname of the remote server.
+ `--port`, `-P`: Alternate port number.
+ `--scheme`, `-S`: Alternate transport protocol to use for the exchange (e.g. sftp).

## Examples

TBD
