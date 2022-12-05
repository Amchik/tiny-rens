#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#include "include/rens.h"
#include "include/log.h"

int init_server(const char* hostname, const char* port) {
	struct addrinfo hints, *result, *rp;
	int sfd, s;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family    = AF_UNSPEC;
	hints.ai_socktype  = SOCK_DGRAM;
	hints.ai_flags     = AI_PASSIVE;

	s = getaddrinfo(hostname, port, &hints, &result);
	if (s != 0) {
		logf(LERR, "Failed to getaddrinfo() on %s:%s", hostname, port);
		logf(LERR, "OS error %d: %s", errno, strerror(errno));

		return -1;
	}

	for (rp = result; rp != 0; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
				rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(sfd);
	}
	freeaddrinfo(result);

	if (rp == 0) {
		logf(LERR, "Failed to bind on %s:%s", hostname, port);
		logf(LERR, "OS error %d: %s", errno, strerror(errno));

		return -2;
	}

	return sfd;
}
