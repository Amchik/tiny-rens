<div align="center">
  <h1>tiny-rens 🚀</h1>
  <p>Local "DNS server" (DNS over HTTPS provider) with caching</p>
</div>

## History

First version is `v1.0.0`, its just proxy in 1 file.
Version `v1.1.0` is more modular but without cache.
I recommend use `v1.1.0` (compile with `-flto`) if you don't need
cache, otherwise last version.

```console
$ git checkout v1.1.0
```

## How it's works?

Just forward packages over HTTPS (DoH) with
caching.
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

Building requires `libcurl4-openssl-dev` installed,
also c compiler and make. On ubuntu/debian: `apt install libcurl4-openssl-dev build-essentials`.
IQ that great that 18 is optional but recommended.

```console
$ make
```

TIP: if you need edit config, just copy `default-config.mk`
to `config.mk`.

You can add some cflags:

 CFLAGS                | Description
-----------------------|-------------------------------------------------------------
`-D DEBUG`             | Enable debug assertion. For now useless
`-D NO_DEBUG_LOG`      | Do not enable debug logging. `-g` will be equivalent to `-v`
`-D BUFF_SIZE=123`     | Set custom buffer size, `1024` by default 
`-D BUILD_RELEASE`     | Hide build commit from version string
`-D BUILD_COMMIT=".."` | Build commit message. Default is `dev (DATE TIME)`
`-D RENSC_DEFAULT_CAP=`| Default cache capability. Default is `64`

## Usage

```console
$ bin/tiny-rens -h
Usage: bin/tiny-rens [-gvqQhcC] [port] [hostname] [dns_server]
port, hostname and dns_server can be '-'

[c] no cache [C] cache wo/ ttl
[g] debug [v] info [q] errors [Q] nothing [h] help
tiny-rens v1.3.0 made by ValgrindLLVM with <3
```

Version `1.x.0` is fully compatible with `1.0.0`.

Basic usage is testing it, because `tiny-rens` too slow
to use it as real dns server. To start `tiny-rens` on
port 5353 on localhost do: `tiny-rens 5353 localhost`.
Also `-` can be used as default param. To start `tiny-rens`
in default port on localhost do `tiny-rens - localhost`.

TIP: use `-v` for information about listening ip/port and dns server.
`-g` uses for debugging DNS queries (without it analyzing).

If you have problems with cache, use `-c` to disable it.

## Perfomance

Without cache it ~150ms (DoH) vs ~50ms (unsecure DNS).

But I live in beautiful country, where unsecure DNS
packages can be modified due political reasons...

