/* vim: ft=c
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "rens.h"

#define RENS_QR 1
#define RENS_AA 2
#define RENS_TC 4
#define RENS_RD 8
#define RENS_RA 16

#define RENS_RCODE_OK       0
#define RENS_RCODE_FORMAT   1
#define RENS_RCODE_SERVFAIL 2
#define RENS_RCODE_NXDOMAIN 3
#define RENS_RCODE_NOTIMPL  4
#define RENS_RCODE_REFUSED  5

struct RensHeader {
	uint16_t id;
	uint8_t  opcode: 4;
	uint8_t  rcode:  4;
	uint8_t  flags:  5;

	uint16_t qdcount;
	uint16_t ancount;
	uint16_t nscount;
	uint16_t arcount;
};
struct RensQuestion {
	rens_buffer qname;
	uint16_t    qtype;
	uint16_t    qclass;
};
struct RensResource {
	rens_buffer name;
	uint16_t    qtype;
	uint16_t    qclass;
	uint32_t    ttl;
	rens_buffer rdata; /* <- rdlength here */
};

/* Read DNS message header, requires 12 bytes
 */
struct RensHeader rens_read_header(const uint8_t *buff);

/* Write DNS message header, requires 12 bytes
 */
size_t rens_write_header(const struct RensHeader *header, uint8_t *buff);


/* Read DNS message question, required offset pointer
 */
struct RensQuestion rens_read_question(const uint8_t *buff);

/* Write DNS message question, required offset pointer
 */
size_t rens_write_question(const struct RensQuestion *question, uint8_t *buff);


/* Read DNS message resource, required offset pointer
 */
struct RensResource rens_read_resource(const uint8_t *buff);

/* Write DNS message resource, required offset pointer
 */
size_t rens_write_resource(const struct RensResource *resource, uint8_t *buff);

/* Uncompress message into out
 */
size_t rens_uncompress_message(uint8_t *in, size_t off, uint8_t *out, size_t n);

