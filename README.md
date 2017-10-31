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

+ `--bits`, `-b`: Show public key length, in bits.
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

`keuka --bits --chain --cipher --issuer --method --serial --signature-algorithm --validity --raw -- www.gnu.org`

```
--- Method: TLSv1.2
--- Cipher: ECDHE-RSA-AES128-GCM-SHA256
--- Certificate Chain:
    0: --- Subject: CN=gnu.org
       --- Issuer: C=US, O=Let's Encrypt, CN=Let's Encrypt Authority X3
       --- Bits: 2048
       --- Serial: 04D5DB3D40FAACC68EB0B07188517C0858C1
       --- Signature Algorithm: sha256WithRSAEncryption
       --- Validity:
           --- Not Before: Sep 27 09:01:00 2017 GMT
           --- Not After: Dec 26 09:01:00 2017 GMT
    1: --- Subject: C=US, O=Let's Encrypt, CN=Let's Encrypt Authority X3
       --- Issuer: O=Digital Signature Trust Co., CN=DST Root CA X3
       --- Bits: 2048
       --- Serial: 0A0141420000015385736A0B85ECA708
       --- Signature Algorithm: sha256WithRSAEncryption
       --- Validity:
           --- Not Before: Mar 17 16:40:46 2016 GMT
           --- Not After: Mar 17 16:40:46 2021 GMT

-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7No
YzDq1zUmGSXhvb418XCSL7e4S0EFq6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf
15Gan/PQeGdxyGkOlZHP/uaZ6WA8SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K
671B5TaYucv9bTyWaN8jKkKQDIZ0Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+X
hq+w3Brvaw2VFn3EK6BlspkENnWAa6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiV
FrqoM7hD8bEfwzB/onkxEz0tNvjj/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3D
kwIDAQAB
-----END PUBLIC KEY-----

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

-----BEGIN CERTIFICATE-----
MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow
SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT
GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC
AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF
q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8
SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0
Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA
a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj
/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T
AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG
CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv
bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k
c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw
VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC
ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz
MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu
Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF
AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo
uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/
wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu
X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG
PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6
KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==
-----END CERTIFICATE-----
```
