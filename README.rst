keuka
=====

.. contents:: :local:

Description
-----------

``keuka`` (kyew-kuh) is a **k**\ ey **e**\ xchange **u**\ tility for **k**\ ey **a**\ nalysis.
It's helpful for analyzing SSL/TLS handshakes.

Installation
------------

There are two installation methods, Homebrew and manual.

Homebrew
^^^^^^^^

.. code-block:: sh

    brew tap nickolasburr/pfa
    brew install keuka

Manual
^^^^^^

By default, ``keuka`` is installed to ``/usr/local/bin``. You can set the ``prefix`` variable when running ``make`` to specify an alternate path.

.. code-block:: sh

    make
    make install


Options
-------

.. raw:: html

    <blockquote>
        <table frame="void" rules="none">
            <tbody valign="top">
                <tr>
                    <td>
                        <kbd>
                            <span>-b, --bits</span>
                        </kbd>
                    </td>
                    <td>Show public key length, in bits.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-c, --chain</span>
                        </kbd>
                    </td>
                    <td>Show peer certificate chain.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-C, --cipher</span>
                        </kbd>
                    </td>
                    <td>Show cipher negotiated during handshake.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-i, --issuer</span>
                        </kbd>
                    </td>
                    <td>Show certificate issuer information.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-m, --method</span>
                        </kbd>
                    </td>
                    <td>Show method negotiated during handshake.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-N, --no-sni</span>
                        </kbd>
                    </td>
                    <td>Disable SNI support.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-q, --quiet</span>
                        </kbd>
                    </td>
                    <td>Suppress progress-related output.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-r, --raw</span>
                        </kbd>
                    </td>
                    <td>Output raw key and certificate contents.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-S, --serial</span>
                        </kbd>
                    </td>
                    <td>Show certificate serial number.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-A, --signature-algorithm</span>
                        </kbd>
                    </td>
                    <td>Show certificate signature algorithm.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-s, --subject</span>
                        </kbd>
                    </td>
                    <td>Show certificate subject.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-V, --validity</span>
                        </kbd>
                    </td>
                    <td>Show Not Before/Not After validity time range.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-h, --help</span>
                        </kbd>
                    </td>
                    <td>Show help information and usage examples.</td>
                </tr>
                <tr>
                    <td>
                        <kbd>
                            <span>-v, --version</span>
                        </kbd>
                    </td>
                    <td>Show version information.</td>
                </tr>
            </tbody>
        </table>
    </blockquote>

Examples
--------

Basic Usage
^^^^^^^^^^^

``keuka`` provides basic progress and timing information, including context
establishment, connection establishment, and handshake initiation and completion.

Get progress and timing information
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sh

    keuka www.openssl.org

::

    --- [0.000011s] Establishing SSL context.
    --- [0.002499s] SSL context established.
    --> [0.003232s] Establishing connection to www.openssl.org.
    <-- [0.003261s] Connection established.
    --- [0.003347s] Attaching SSL session to socket.
    --> [0.003362s] SSL session attached, handshake initiated.
    <-- [0.007192s] TLSv1.3 negotiated, handshake complete.

Get cipher and certificate chain
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sh

    keuka --chain --cipher -- www.amazon.com

::

    --- [0.000011s] Establishing SSL context.
    --- [0.002810s] SSL context established.
    --> [0.003543s] Establishing connection to www.amazon.com.
    <-- [0.003567s] Connection established.
    --- [0.003675s] Attaching SSL session to socket.
    --> [0.003693s] SSL session attached, handshake initiated.
    <-- [0.008298s] TLSv1.3 negotiated, handshake complete.

    --- Cipher: TLS_AES_128_GCM_SHA256
    --- Certificate Chain:
        0: [redacted]
        1: [redacted]
        2: [redacted]

Get handshake method and signature algorithm
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sh

    keuka -mA www.google.com

::

    --- [0.000012s] Establishing SSL context.
    --- [0.002750s] SSL context established.
    --> [0.003358s] Establishing connection to www.google.com.
    <-- [0.003396s] Connection established.
    --- [0.003547s] Attaching SSL session to socket.
    --> [0.003564s] SSL session attached, handshake initiated.
    <-- [0.007649s] TLSv1.3 negotiated, handshake complete.

    --- Method: TLSv1.3
    --- Signature Algorithm: sha256WithRSAEncryption

Advanced Usage
^^^^^^^^^^^^^^

``keuka`` was built to make extracting key and certificate information simple.
Likewise, it can be used with tools such as ``sed``, ``grep``, and ``cut`` to
fulfill a variety of tasks. Below are a few examples of advanced usage.

Get negotiated cipher
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sh

    keuka -qC www.amazon.com | cut -d' ' -f3

::

    TLS_AES_128_GCM_SHA256

Get certificate expiration date
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sh

    keuka -qV www.ietf.org | tail -n +3 | sed 's/   --- Not After: //g' | awk '{$1=$1};1'

::

    Jun 27 23:59:59 2022 GMT

Get Common Name from certificate subject
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sh

    keuka -qs www.github.com | sed 's/--- Subject: //g' | tr "," "\n" | tail -1

::

    CN=github.com

Get public key and certificate, and split into separate files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sh

    keuka -qr www.gnu.org | split -p '-----BEGIN CERTIFICATE-----' - keuka-

::

    # -rw-r--r--  1 nickolasburr staff   453 Nov 11 14:24 keuka-aa
    # -rw-r--r--  1 nickolasburr staff   453 Nov 11 14:24 keuka-ab

    -----BEGIN PUBLIC KEY-----
    MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmEsA2Ma0mA2LR/chNzHq
    FUeF8Fw9b8ZSkCbyDfnFFk7imMI9E9GPoRibqWRvbCx9ypYiPu35HwnGeAqzY/fe
    oJuLaQ0IXVmJQ8/c18KzFGR8a5KSgwA/lwIbmkdUhQONHHxvAlaAXrb773v8kX0p
    s7dvG+Fi2aP5dH8x8H5WO753h1hKhW7xvJgYQ4jL6sFa432Iejfh9OeI24FLDtWj
    7AuhcNp8xKfeIBpPYX8+MDYpUEbCUYgAklXi97YBFCBnPyM2wz3tp9EZ223Y2Wls
    ais9ut2sPhVnln0pfzimHMzV4yYohEe+DPSyleyvYnAktGSImm/q7l6lPpkDKBQp
    bQIDAQAB
    -----END PUBLIC KEY-----

    -----BEGIN CERTIFICATE-----
    MIIKRDCCCSygAwIBAgISAwANCRhgGOkL54Wacj8CsppBMA0GCSqGSIb3DQEBCwUA
    MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
    EwJSMzAeFw0yMjA0MTUxMjI0NDJaFw0yMjA3MTQxMjI0NDFaMB8xHTAbBgNVBAMT
    FHdpbGRlYmVlc3QxcC5nbnUub3JnMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB
    CgKCAQEAmEsA2Ma0mA2LR/chNzHqFUeF8Fw9b8ZSkCbyDfnFFk7imMI9E9GPoRib
    qWRvbCx9ypYiPu35HwnGeAqzY/feoJuLaQ0IXVmJQ8/c18KzFGR8a5KSgwA/lwIb
    mkdUhQONHHxvAlaAXrb773v8kX0ps7dvG+Fi2aP5dH8x8H5WO753h1hKhW7xvJgY
    Q4jL6sFa432Iejfh9OeI24FLDtWj7AuhcNp8xKfeIBpPYX8+MDYpUEbCUYgAklXi
    97YBFCBnPyM2wz3tp9EZ223Y2Wlsais9ut2sPhVnln0pfzimHMzV4yYohEe+DPSy
    leyvYnAktGSImm/q7l6lPpkDKBQpbQIDAQABo4IHZTCCB2EwDgYDVR0PAQH/BAQD
    AgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAMBgNVHRMBAf8EAjAA
    MB0GA1UdDgQWBBT5l5VnNWb+f7ooZyhvx/RUakXPcDAfBgNVHSMEGDAWgBQULrMX
    t1hWy65QCUDmH6+dixTCxjBVBggrBgEFBQcBAQRJMEcwIQYIKwYBBQUHMAGGFWh0
    dHA6Ly9yMy5vLmxlbmNyLm9yZzAiBggrBgEFBQcwAoYWaHR0cDovL3IzLmkubGVu
    Y3Iub3JnLzCCBTIGA1UdEQSCBSkwggUlghVhcmNoaXZlLmduZXdzZW5zZS5vcmeC
    EmJldGEuZ25ld3NlbnNlLm9yZ4IWYmxvb2Rub2suZ25ld3NlbnNlLm9yZ4ISYm9m
    aC5nbmV3c2Vuc2Uub3JnghJidWdzLmduZXdzZW5zZS5vcmeCEWJ6ci5nbmV3c2Vu
    c2Uub3JnghVjZGltYWdlLmduZXdzZW5zZS5vcmeCDWNsYXNzcGF0aC5vcmeCFGNv
    bmZpZy5nbmV3c2Vuc2Uub3JnghFkaWdpdGFsc3BlZWNoLm9yZ4IYZG9uYXRlLmRp
    Z2l0YWxzcGVlY2gub3Jnggpkb3RnbnUub3JnghRlY2NsZXMuZ25ld3NlbnNlLm9y
    Z4IJZW1hY3Mub3Jngg1nbGliYy5nbnUub3Jngg1nbmV3c2Vuc2Uub3JnggdnbnUu
    b3JnggtnbnVraWRzLm9yZ4IKZ3BsZmFxLm9yZ4IMaHVyZC5nbnUub3Jngg9pcHY2
    Lm5vbmdudS5vcmeCEWtpbmRsZXN3aW5kbGUub3Jnggpub25nbnUub3JnghtwYXRj
    aC10cmFja2VyLmduZXdzZW5zZS5vcmeCD3BsYXlmcmVlZG9tLm9yZ4ILcGxheW9n
    Zy5jb22CC3BsYXlvZ2cubmV0ggtwbGF5b2dnLm9yZ4ITcnN5bmMuZ25ld3NlbnNl
    Lm9yZ4IVc2VhZ29vbi5nbmV3c2Vuc2Uub3JnghZzZWN1cml0eS5nbmV3c2Vuc2Uu
    b3JnghFzbWFsbHRhbGsuZ251Lm9yZ4IVdG9ycmVudC5nbmV3c2Vuc2Uub3JnghZ1
    cGdyYWRlZnJvbXdpbmRvd3MuY29tghZ1cGdyYWRlZnJvbXdpbmRvd3Mub3Jnghd1
    cGdyYWRlZnJvbXdpbmRvd3M4LmNvbYIXdXBncmFkZWZyb213aW5kb3dzOC5vcmeC
    GHVzLmFyY2hpdmUuZ25ld3NlbnNlLm9yZ4INdmNkaW1hZ2VyLm9yZ4ISd2lraS5n
    bmV3c2Vuc2Uub3Jnghd3aWxkZWJlZXN0LmlwdjYuZ251Lm9yZ4IUd2lsZGViZWVz
    dDFwLmdudS5vcmeCEXd3dy5jbGFzc3BhdGgub3JnghV3d3cuZGlnaXRhbHNwZWVj
    aC5vcmeCDnd3dy5kb3RnbnUub3Jngg13d3cuZW1hY3Mub3JnghF3d3cuZ25ld3Nl
    bnNlLm9yZ4ILd3d3LmdudS5vcmeCD3d3dy5nbnVraWRzLm9yZ4IOd3d3LmdwbGZh
    cS5vcmeCEHd3dy5odXJkLmdudS5vcmeCEHd3dy5pcHY2LmdudS5vcmeCE3d3dy5p
    cHY2Lm5vbmdudS5vcmeCFXd3dy5raW5kbGVzd2luZGxlLm9yZ4IOd3d3Lm5vbmdu
    dS5vcmeCE3d3dy5wbGF5ZnJlZWRvbS5vcmeCD3d3dy5wbGF5b2dnLmNvbYIPd3d3
    LnBsYXlvZ2cubmV0gg93d3cucGxheW9nZy5vcmeCGnd3dy51cGdyYWRlZnJvbXdp
    bmRvd3MuY29tghp3d3cudXBncmFkZWZyb213aW5kb3dzLm9yZ4Ibd3d3LnVwZ3Jh
    ZGVmcm9td2luZG93czguY29tght3d3cudXBncmFkZWZyb213aW5kb3dzOC5vcmeC
    EXd3dy52Y2RpbWFnZXIub3Jnggx3d3c2LmdudS5vcmeCD3d3dzYubm9uZ251Lm9y
    Z4IUeDg2LTMyLmduZXdzZW5zZS5vcmeCFHg4Ni02NC5nbmV3c2Vuc2Uub3JnMEwG
    A1UdIARFMEMwCAYGZ4EMAQIBMDcGCysGAQQBgt8TAQEBMCgwJgYIKwYBBQUHAgEW
    Gmh0dHA6Ly9jcHMubGV0c2VuY3J5cHQub3JnMIIBBQYKKwYBBAHWeQIEAgSB9gSB
    8wDxAHcA36Veq2iCTx9sre64X04+WurNohKkal6OOxLAIERcKnMAAAGALWW3BAAA
    BAMASDBGAiEAz2MTU/no2ICHVo1+dI2gqbWsf7YvweFQqNfxQLqqJzwCIQD8ordw
    p1GrIB6JNsxU0tLkyEJEvzyAjt+UivFFFm2VzwB2ACl5vvCeOTkh8FZzn2Old+W+
    V32cYAr4+U1dJlwlXceEAAABgC1ltwMAAAQDAEcwRQIgUTbqVO9NvH9DrCWmcwwy
    rs8EtcoRNLghjMkmowTDmdQCIQD2D4ceFGZDwWU4/VyVb9jfh6pubbYWESY015/B
    D+qlJTANBgkqhkiG9w0BAQsFAAOCAQEALzMXd0xfzSoHc/dULMrKhQFV9OAuhvlB
    mKCQyAItipOgc3AS1SG21tcxZDlvLGlb8wAG8BnuogvHOaDKW1ZJ4HJeVKhCT6PF
    EWLCjHLWkZ3IZmFbvNOo1XLr7iCQKGM/EGkPy11Ijb/bM3LNN+VDf+dlznQpf6Av
    0KCe6HsbayOFIxo3lIkhziM8mIEdrOYKSbvZyRJOffNvifXhvMF2VKDE1g2plqCX
    5CroUwEpyfiWRNcr60H25AqX9PVnO2vkhrWZTQVD+zmC/KskVZCLqWuZBQHliasn
    jKScAxzYEJrX+fMP07z55Lpb4pROZrvmw11SqVsdgDo2S5baRN7YRg==
    -----END CERTIFICATE-----

Notes
-----

1. Specifying ``--chain`` without complementary options shows ``[redacted]`` per certificate.
2. Assumes BSD split(1). For GNU split(1), use ``--filter`` instead.

Troubleshooting
---------------

> dyld: Library not loaded: /usr/local/opt/openssl/lib/libssl.1.0.0.dylib

This is a known issue on macOS. The most common solution is to switch to an older version
of OpenSSL.

Using Homebrew, you can do the following:

.. code-block:: sh

   brew switch openssl 1.0.2t

.. |link1| replace:: ``here``
.. _link1: https://tinyurl.com/u2wtd4x

See |link1|_ for more information.
