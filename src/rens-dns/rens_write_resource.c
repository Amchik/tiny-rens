#include <string.h>

#include "../include/rens.h"

size_t rens_write_resource(const struct RensResource *resource, uint8_t *buff) {
	size_t namelen;

	namelen = resource->name.len;
	memcpy(buff, resource->name.ptr, namelen);

	buff[namelen    ] = resource->qtype >> 8;
	buff[namelen + 1] = resource->qtype & 0x00FF;

	buff[namelen + 2] = resource->qclass >> 8;
	buff[namelen + 3] = resource->qclass & 0x00FF;

	buff[namelen + 4] =  resource->ttl >> 24;
	buff[namelen + 5] = (resource->ttl >> 16) & 0xFF;
	buff[namelen + 6] = (resource->ttl >>  8) & 0xFF;
	buff[namelen + 7] =  resource->ttl        & 0xFF;

	buff[namelen + 8] = resource->rdata.len >> 8;
	buff[namelen + 9] = resource->rdata.len & 0xFF;

	memcpy(buff + namelen + 10, resource->rdata.ptr, resource->rdata.len);

	return namelen + 10 + resource->rdata.len;
}
