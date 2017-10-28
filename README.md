# Keuka

Keuka is a utility for analyzing and modifying certificate and key information during SSL/TLS handshakes.

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

+ `--chain`, `-c`: Show full certificate chain.
+ `--cipher`, `-C`: Show cipher used in handshake.
+ `--hostname`, `-H`: Hostname of the remote server.
+ `--method`, `-M`: Handshake method. See below for a list.
+ `--port`, `-P`: Alternate port number.
+ `--raw`, `-r`: Output raw certificate(s) in addition to information table.
+ `--scheme`, `-S`: Alternate transport protocol to use for the exchange (e.g. sftp).
+ `--serial`, `-s`: Show certificate serial number.
+ `--signature-algorithm`, `-A`: Show signature algorithm.

## Methods

Available methods:

+ `SSLv2`
+ `SSLv3`
+ `TLSv1`
+ `TLSv1_1`
+ `TLSv1_2`

## Examples

TBD
