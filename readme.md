<div align="center">
  <h1>tiny-rens 🚀</h1>
  <p>Blazingly slow "DNS server" without cache</p>
</div>

## How it's works?

Just forward packages over HTTPS (DoH) without
caching or etc.
Good if `systemd-resolved` is is not available
in your country.

Run `tiny-rens` as a service and set it as default
nameserver. If you use `resolvconf`, do that:

```console
$ my-rc-services tiny-rens start-background
Started DNS server on 127.0.0.51:53
$ sudo su
# echo 'name_servers=127.0.0.51' >> /etc/resolvconf.conf
# resolvconf -u
```

Now you'r apps will use DoH for DNS queries.
TIP: if you use OpenRC as init system, you can use
[this service config](https://gist.github.com/ValgrindLLVM/00a877ec1211c53946e0b76b00f3a3c8).

By the way, "tiny" prefixes are usually shit
version of normal programs (tcc for example),
but in this case it better than rens. Anyway,
please use `netcat -u -l -p 53 -c 'curl ...'`
instead of this slow shit (written on cURL lol).

## Building

Building requires `libcurl` and ssl shit installed,
also c compiler and make. IQ that great that 18 is
optional but recommended.

```console
$ make
```

TIP: if you need edit config, just copy `default-config.mk`
to `config.mk`.

## Usage

```console
$ bin/rens -h
rens: src/main.c:47: int main(): Assertion `rp != 0' failed.
Aborted
```

Woops. Real help message:

```console
$ bin/tiny-rens -h
Usage: bin/tiny-rens [port | -] [hostname | -] [dnsserver]
       bin/tiny-rens [--help | -h]
Default: port=53 hostname=127.0.0.51
         dnsserver=1.1.1.1
```

Basic usage is testing it, because `tiny-rens` too slow
to use it as real dns server. To start `tiny-rens` on
port 5353 on localhost do: `tiny-rens 5353 localhost`.
Also `-` can be used as default param. To start `tiny-rens`
in default port on localhost do `tiny-rens - localhost`.

## Perfomance

```console
$ drill @127.0.0.1 -p 4444 google.com | tail -n4
;; Query time: 262 msec
;; SERVER: 127.0.0.1
;; WHEN: Sun Nov 27 03:43:12 2022
;; MSG SIZE  rcvd: 44
$ drill @1.1.1.1 google.com | tail -n4
;; Query time: 50 msec
;; SERVER: 1.1.1.1
;; WHEN: Sun Nov 27 03:44:16 2022
;; MSG SIZE  rcvd: 44
```

But I live in beautiful country, where unsecure DNS
packages can be modified due political reasons...

## To-Do

* Minor bug fixes

Thats final version of `tiny-rens`. Without cache.
Without normal perfomance.

