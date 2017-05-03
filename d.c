/*
 * 	getmac.c
 * 	
 * 	Simple Demo:	Get MAC address of a specified NIC on FreeBSD
 *
 * 	To compile: gcc getmac.c -o getmac
 *
 * 	Tested on FreeBSD-4.6 RELEASE & FreeBSD-5.2-current
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int			mib[6], len;
	char			*buf;
	unsigned char		*ptr;
	struct if_msghdr	*ifm;
	struct sockaddr_dl	*sdl;

	if (argc != 2) {
		fprintf(stderr, "Usage: getmac <interface>\n");
		return 1;
	}

	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = AF_LINK;
	mib[4] = NET_RT_IFLIST;
	if ((mib[5] = if_nametoindex(argv[1])) == 0) {
		perror("if_nametoindex error");
		exit(2);
	}

    printf("%d, %d, %d, %d, %d, %d\n",mib[0],mib[1],mib[2],mib[3],mib[4],mib[5]);
    printf("%d\n",mib);

	if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
		perror("sysctl 1 error");
		exit(3);
	}

	if ((buf = malloc(len)) == NULL) {
		perror("malloc error");
		exit(4);
	}

	if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
		perror("sysctl 2 error");
		exit(5);
	}

	ifm = (struct if_msghdr *)buf;
	sdl = (struct sockaddr_dl *)(ifm + 1);
	ptr = (unsigned char *)LLADDR(sdl);
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *ptr, *(ptr+1), *(ptr+2),
			*(ptr+3), *(ptr+4), *(ptr+5));
	
	return 0;
}
