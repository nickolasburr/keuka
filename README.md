# Keuka

Keuka is a utility for analyzing the properties of SSL/TLS handshakes.

## Installation

There are currently two installation methods: Homebrew and manual.

### Homebrew

```
brew tap nickolasburr/pfa
brew install keuka
```

### Manual

By default, `keuka` is installed to `/usr/local/bin`. You can set the `prefix` variable when running `make` to specify an alternate path.

```
make
make install
```

## Options

+ `--bits`, `-b`: Show public key length, in bits.
+ `--chain`, `-c`: Show peer certificate chain.
+ `--cipher`, `-C`: Show cipher negotiated during handshake.
+ `--issuer`, `-i`: Show certificate issuer information.
+ `--method`, `-m`: Show method negotiated during handshake.
+ `--no-sni`, `-N`: Disable SNI support.
+ `--quiet`, `-q`: Suppress progress-related output.
+ `--raw`, `-r`: Output raw key and certificate contents.
+ `--serial`, `-S`: Show certificate serial number.
+ `--signature-algorithm`, `-A`: Show certificate signature algorithm.
+ `--subject`, `-s`: Show certificate subject.
+ `--validity`, `-V`: Show Not Before/Not After validity time range.
+ `--help`, `-h`: Show help information and usage examples.
+ `--version`, `-v`: Show version information.

## Examples

### Basic Usage

`keuka` provides basic progress and timing information, including context
establishment, connection establishment, and handshake initiation and completion.

#### Get progress and timing information.

`keuka www.openssl.org`

```
--- [0.000052s] Establishing SSL context.
--- [0.000721s] SSL context established.
--> [0.002033s] Establishing connection to www.openssl.org.
<-- [0.002056s] Connection established.
--- [0.002161s] Attaching SSL session to socket.
--- [0.002176s] SSL session attached to socket.
--> [0.002176s] Initiating handshake with www.openssl.org.
<-- [0.004188s] Handshake complete.
```

#### Get cipher and certificate chain.<sup>[1](#chain)</sup>

`keuka --chain --cipher -- amazon.com`

```
--- [0.000018s] Establishing SSL context.
--- [0.000494s] SSL context established.
--> [0.001827s] Establishing connection to amazon.com.
<-- [0.001866s] Connection established.
--- [0.001985s] Attaching SSL session to socket.
--- [0.002024s] SSL session attached to socket.
--> [0.002024s] Initiating handshake with amazon.com.
<-- [0.004389s] Handshake complete.

--- Cipher: ECDHE-RSA-AES128-GCM-SHA256
--- Certificate Chain:
    0: [redacted]
    1: [redacted]
```

#### Get handshake method and signature algorithm.

`keuka -mA google.com`

```
--- [0.000013s] Establishing SSL context.
--- [0.000330s] SSL context established.
--> [0.001227s] Establishing connection to google.com.
<-- [0.001262s] Connection established.
--- [0.001360s] Attaching SSL session to socket.
--- [0.001373s] SSL session attached to socket.
--> [0.001373s] Initiating handshake with google.com.
<-- [0.003505s] Handshake complete.

--- Method: TLSv1.2
--- Signature Algorithm: sha256WithRSAEncryption
```

### Advanced Usage

`keuka` was built to make extracting key and certificate information simple.
Likewise, it can be used with tools such as `sed`, `grep`, and `cut` to fulfill
a variety of tasks. Below are a few examples of advanced usage.

#### Get cipher.

`keuka -qC amazon.com | cut -d' ' -f3`

```
ECDHE-RSA-AES128-GCM-SHA256
```

#### Get certificate expiration date.

`keuka -qV www.ietf.org | tail -n +3 | sed 's/   --- Not After: //g' | awk '{$1=$1};1'`

```
Aug 11 23:12:50 2018 GMT
```

#### Get Common Name from certificate subject.

`keuka -qs www.github.com | sed 's/--- Subject: //g' | grep --color=never -o CN=*.github.com`

```
CN=github.com
```

#### Get public key and certificate, and split into separate files.<sup>[2](#split)</sup>

`keuka -qr www.gnu.org | split -p '-----BEGIN CERTIFICATE-----' - keuka-`

```
# -rw-r--r--  1 nickolasburr staff   453 Nov 11 14:24 keuka-aa

-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvvqMJ98hjHN7HyNq8oxL
RomE+O2rbIBsdxhUxNAbj+F03WTZUG7xz9hdUHxr+1oTG2eBp0+n9LRIDEw5dkx2
bfKHZioxiwUEPNEQr8QErudFfsmR5+SKBmPMjk1njmnRkZErFGWB9FMGFIo8C9AM
1gOHr1caJUw4auTDZMhchL7gOPZVGTqe21qVCZzT/CjRUN/wzGSVON2depTYXn41
yKSMXN0lJZj6PQwSuhfjv0NsASBOY9bWlTeduFKB49VvFVY8TTajvr7ZZL6Ripxh
pmdtcsU+7uAnE68NoFvq9lgbHSg/Syzf1Vv1oPCx/lrrisEbJN6OgNzryDecz3R9
3QIDAQAB
-----END PUBLIC KEY-----
```

```
# -rw-r--r--  1 nickolasburr staff   453 Nov 11 14:24 keuka-ab

-----BEGIN CERTIFICATE-----
MIIFZDCCBEygAwIBAgISBNXbPUD6rMaOsLBxiFF8CFjBMA0GCSqGSIb3DQEBCwUA
MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD
ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0xNzA5MjcwOTAxMDBaFw0x
NzEyMjYwOTAxMDBaMBIxEDAOBgNVBAMTB2dudS5vcmcwggEiMA0GCSqGSIb3DQEB
AQUAA4IBDwAwggEKAoIBAQC++own3yGMc3sfI2ryjEtGiYT47atsgGx3GFTE0BuP
4XTdZNlQbvHP2F1QfGv7WhMbZ4GnT6f0tEgMTDl2THZt8odmKjGLBQQ80RCvxASu
50V+yZHn5IoGY8yOTWeOadGRkSsUZYH0UwYUijwL0AzWA4evVxolTDhq5MNkyFyE
vuA49lUZOp7bWpUJnNP8KNFQ3/DMZJU43Z16lNhefjXIpIxc3SUlmPo9DBK6F+O/
Q2wBIE5j1taVN524UoHj1W8VVjxNNqO+vtlkvpGKnGGmZ21yxT7u4CcTrw2gW+r2
WBsdKD9LLN/VW/Wg8LH+WuuKwRsk3o6A3OvIN5zPdH3dAgMBAAGjggJ6MIICdjAO
BgNVHQ8BAf8EBAMCBaAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMAwG
A1UdEwEB/wQCMAAwHQYDVR0OBBYEFGFxRfLUavp6y+CssYKH3vnIAcIiMB8GA1Ud
IwQYMBaAFKhKamMEfd265tE5t6ZFZe/zqOyhMG8GCCsGAQUFBwEBBGMwYTAuBggr
BgEFBQcwAYYiaHR0cDovL29jc3AuaW50LXgzLmxldHNlbmNyeXB0Lm9yZzAvBggr
BgEFBQcwAoYjaHR0cDovL2NlcnQuaW50LXgzLmxldHNlbmNyeXB0Lm9yZy8wgYQG
A1UdEQR9MHuCCWVtYWNzLm9yZ4IHZ251Lm9yZ4IMaHVyZC5nbnUub3Jnghh3d3cu
ZnJlZXNvZnR3YXJlLmZzZi5vcmeCC3d3dy5nbnUub3JnghB3d3cuaHVyZC5nbnUu
b3JnghB3d3cuaXB2Ni5nbnUub3Jnggx3d3c2LmdudS5vcmcwgf4GA1UdIASB9jCB
8zAIBgZngQwBAgEwgeYGCysGAQQBgt8TAQEBMIHWMCYGCCsGAQUFBwIBFhpodHRw
Oi8vY3BzLmxldHNlbmNyeXB0Lm9yZzCBqwYIKwYBBQUHAgIwgZ4MgZtUaGlzIENl
cnRpZmljYXRlIG1heSBvbmx5IGJlIHJlbGllZCB1cG9uIGJ5IFJlbHlpbmcgUGFy
dGllcyBhbmQgb25seSBpbiBhY2NvcmRhbmNlIHdpdGggdGhlIENlcnRpZmljYXRl
IFBvbGljeSBmb3VuZCBhdCBodHRwczovL2xldHNlbmNyeXB0Lm9yZy9yZXBvc2l0
b3J5LzANBgkqhkiG9w0BAQsFAAOCAQEANJfoCZuo5apkn6Kncabwq7JoCsMFVrzk
72CpAqs8tVWZfYkbCnezEuESyWZfYA4vjiD+BOD+2jlcfeswamy5oDU8tCykp/+Q
1E4Wa7D14jb6zbgGj7h3z0EXYTx4l58t0PUjbZJbp7d2kf6/H4I7cCnmSf6kF2sN
bBlPN7x81vtmY/mA7bboMLU5ZtuhSW2d82o9/Uo1IY2o0X32ZBCdTKRvABN3urDQ
9e1txwfZyuJefgX2sZrv1QlmjI4jQwVPgMxwBK7asmxP8ZWkqGfnZokCX+iYOI0U
lx1MOd8y3HFI/j1nE7xZZMPLKtGGyBYZ+W1sXbpTx/7WQ+8uQcaYfA==
-----END CERTIFICATE-----
```

## Notes

+ <a name="#chain">1</a>: Specifying `--chain` without complementary options shows `[redacted]` per certificate.</div>
+ <a name="#split">2</a>: Assumes BSD split(1). For GNU split(1), use `--filter` instead.</div>
