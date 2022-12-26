#include <string.h>

#include "../include/rens.h"

size_t rens_uncompress_message(uint8_t *in, size_t off, uint8_t *out, size_t n) {
	size_t i, nwrite, to_write;

	nwrite = 0;
	for (i = off; in[i] != 0 && nwrite < n; ) {
		if ((in[i] & 0xC0) == 0xC0) {
			i = ((in[i] ^ 0xC0) << 8) + in[i + 1];
			continue;
		}

		to_write = in[i] + nwrite + 1 < n ? in[i] : n - nwrite;
		memcpy(out + nwrite, in + i, to_write + 1);
		i += to_write + 1;
		nwrite += to_write + 1;
	}

	if (nwrite < n)
		out[nwrite] = 0;

	return nwrite + 1;
}
