# Keuka

Keuka is a utility for analyzing the properties of SSL/TLS handshakes.

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

+ `--chain`, `-c`: Show peer certificate chain.
+ `--cipher`, `-C`: Show cipher negotiated during handshake.
+ `--issuer`, `-i`: Show certificate issuer information.
+ `--method`, `-m`: Show method negotiated during handshake.
+ `--raw`, `-r`: Output raw certificate contents.
+ `--serial`, `-S`: Show certificate serial number.
+ `--signature-algorithm`, `-A`: Show certificate signature algorithm.
+ `--validity`, `-V`: Show Not Before/Not After validity time range.

## Examples

`keuka www.openssl.org`

```
--- Subject: OU=Domain Control Validated,CN=*.openssl.org
```

`keuka --chain --cipher -- amazon.com`

```
--- Cipher: ECDHE-RSA-AES128-GCM-SHA256
--- Certificate Chain:
    0: --- Subject: C=US, ST=Washington, L=Seattle, O=Amazon.com, Inc., CN=www.amazon.com
    1: --- Subject: C=US, O=Symantec Corporation, OU=Symantec Trust Network, CN=Symantec Class 3 Secure Server CA - G4
```

`keuka --chain --issuer --method -- google.com`

```
--- Method: TLSv1.2
--- Certificate Chain:
    0: --- Subject: C=US, ST=California, L=Mountain View, O=Google Inc, CN=*.google.com
       --- Issuer: C=US, O=Google Inc, CN=Google Internet Authority G2
    1: --- Subject: C=US, O=Google Inc, CN=Google Internet Authority G2
       --- Issuer: C=US, O=GeoTrust Inc., CN=GeoTrust Global CA
    2: --- Subject: C=US, O=GeoTrust Inc., CN=GeoTrust Global CA
       --- Issuer: C=US, O=Equifax, OU=Equifax Secure Certificate Authority
```

`keuka --cipher --issuer --method --raw -- www.openssl.org`

```
--- Method: TLSv1.2
--- Cipher: ECDHE-RSA-AES256-GCM-SHA384
--- Subject: CN=www.openssl.org
--- Issuer: C=US, O=Let's Encrypt, CN=Let's Encrypt Authority X3

-----BEGIN CERTIFICATE-----
MIIFATCCA+mgAwIBAgISA8BGaubNIyZD/Mci4GG+hCCJMA0GCSqGSIb3DQEBCwUA
MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD
ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0xNzEwMDYyMDQ5MzBaFw0x
ODAxMDQyMDQ5MzBaMBoxGDAWBgNVBAMTD3d3dy5vcGVuc3NsLm9yZzCCASIwDQYJ
KoZIhvcNAQEBBQADggEPADCCAQoCggEBAKdjPj6qGa11M4rnPNJlPiXlRAtpWc7o
hIxx/Jxd6MYo2PGYVIjg4+vSQ0KdKve4joG+1Xm0ZtVEFd7aLVsJVPIEW7yHpHos
O9GLDxNMtFgdP3jfHDx4IO54IyfuoWUL6iBw5Yd0PTht5Araz3HXQwSjfHItkoCj
sTAyJb5njdQb+9iVDj0Ri8qu6R7Aircd76oyWf0VdL+y+9pjGhOR+nkMLCSQ4e0f
UY3LnYpooyS6a+utmBytaqAGG4vYvnac1S+HV1xgWq6Z1jdCIKmELJ1Oz3/rwNZg
pMVwHrSLxAjiive0EsQW8EW5BKBGr7hxEDQQrqD8wVLMrNAu5CWfH8sCAwEAAaOC
Ag8wggILMA4GA1UdDwEB/wQEAwIFoDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYB
BQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQUwXoJGX10eae5sehLWnnnCwtf
lkowHwYDVR0jBBgwFoAUqEpqYwR93brm0Tm3pkVl7/Oo7KEwbwYIKwYBBQUHAQEE
YzBhMC4GCCsGAQUFBzABhiJodHRwOi8vb2NzcC5pbnQteDMubGV0c2VuY3J5cHQu
b3JnMC8GCCsGAQUFBzAChiNodHRwOi8vY2VydC5pbnQteDMubGV0c2VuY3J5cHQu
b3JnLzAaBgNVHREEEzARgg93d3cub3BlbnNzbC5vcmcwgf4GA1UdIASB9jCB8zAI
BgZngQwBAgEwgeYGCysGAQQBgt8TAQEBMIHWMCYGCCsGAQUFBwIBFhpodHRwOi8v
Y3BzLmxldHNlbmNyeXB0Lm9yZzCBqwYIKwYBBQUHAgIwgZ4MgZtUaGlzIENlcnRp
ZmljYXRlIG1heSBvbmx5IGJlIHJlbGllZCB1cG9uIGJ5IFJlbHlpbmcgUGFydGll
cyBhbmQgb25seSBpbiBhY2NvcmRhbmNlIHdpdGggdGhlIENlcnRpZmljYXRlIFBv
bGljeSBmb3VuZCBhdCBodHRwczovL2xldHNlbmNyeXB0Lm9yZy9yZXBvc2l0b3J5
LzANBgkqhkiG9w0BAQsFAAOCAQEAkpQEEoZtC7am78t5yu8w20QpZ/JTTciHRFTT
6h64phLQ5Z2epI9MIBXrUvteYS4MJ2rKDyDhxfJNWxbfss8QhRkImX+315Y6Trbd
hcHEy27g8ozZ4o7Yl8NMEzmuFSDTi+Ur+lhc+HNbl8WeT+bQQKwRyaT1KEA6MJF2
RgM1MKdcygsBjEWaZVQ7BosNPcY0bxO8ozOcPEKKs4FdK8eduWJ8MHI5+VoVyi8V
YWBGbFUU5x/8Wr3GuwYXXdXRAwlwMNXRESTWbAg83dwyV2zizd87v8+HDzaBvg7n
JaGz+SjzenXX2WbfQiyNv8ycU0jjDOPDc8N1hDcxw9XlKkSmtA==
-----END CERTIFICATE-----
```

`keuka --chain --method --signature-algorithm -- www.ietf.org`

```
--- Method: TLSv1.2
--- Certificate Chain:
    0: --- Subject: OU=Domain Control Validated, CN=*.ietf.org
       --- Signature Algorithm: sha256WithRSAEncryption
    1: --- Subject: C=US, ST=Arizona, L=Scottsdale, O=Starfield Technologies, Inc., OU=http://certs.starfieldtech.com/repository/, CN=Starfield Secure Certificate Authority - G2
       --- Signature Algorithm: sha256WithRSAEncryption
    2: --- Subject: C=US, ST=Arizona, L=Scottsdale, O=Starfield Technologies, Inc., CN=Starfield Root Certificate Authority - G2
       --- Signature Algorithm: sha256WithRSAEncryption
```

`keuka --issuer --validity -- www.ieee.org`

```
--- Subject: C=US,ST=New Jersey,L=Piscataway,O=Institute of Electrical and Electronics Engineers, Inc.,OU=IT,CN=www.ieee.org
--- Issuer: C=US, O=Symantec Corporation, OU=Symantec Trust Network, CN=Symantec Class 3 Secure Server CA - G4
--- Validity:
    --- Not Before: Jul 17 00:00:00 2017 GMT
    --- Not After: Oct 16 23:59:59 2018 GMT
```
