#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/ioctl.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <sys/errno.h> 
#include <net/if.h> 
#include <net/if_dl.h> 
#include <net/ethernet.h> 
#include <sys/types.h> 
#include <sys/sysctl.h> 


using namespace std; 

int main(int argc, char *argv[]) 
{ 
char * buf = NULL; 
struct ifconf ifc; 
int socketID = 0; 

// we need a socket to call ioctl on 
socketID = socket(AF_INET, SOCK_STREAM, 0); 
if (socketID == -1) 
{ 
printf("error calling socket\n"); 
return -1; 
} 

// keep calling ioctl until we provide it a big enough buffer 
{ 
int numreqs = 100; 
buf = (char*) malloc(sizeof(struct ifreq) * numreqs); 

for (;;)
{ 
ifc.ifc_len = sizeof(struct ifreq) * numreqs; 
ifc.ifc_buf = buf; 

if (ioctl(socketID, SIOCGIFCONF, &ifc) < 0) 
{ 
printf("error calling ioctl: %s\n", strerror(errno)); 
return -1; 
} 

if (ifc.ifc_len / sizeof(struct ifreq) == numreqs) 
{ 
// there's more to read 
numreqs *= 2; 
buf = (char*) realloc(buf, sizeof(struct ifreq) * numreqs); 
} 
else 
{ 
// we're read as much as we can 
break; 
} 
} 
} 

// iterate through each interface 
for (int n = 0; n < ifc.ifc_len; ) 
{ 
struct ifreq * ifr = (struct ifreq*) (((char*) ifc.ifc_req ) + 
n); 

n += sizeof(ifr->ifr_name) + 
(ifr->ifr_addr.sa_len > sizeof(struct sockaddr) 
? ifr->ifr_addr.sa_len : sizeof(struct sockaddr)); 

if (ifr->ifr_addr.sa_family != AF_INET) 
continue; 

// make sure the interface is UP 
{ 
if (ioctl(socketID, SIOCGIFFLAGS, ifr) < 0) 
{ 
printf("error calling ioctl: %s\n", strerror(errno)); 
continue; 
} 

if (!(ifr->ifr_flags & IFF_UP)) 
continue; 
} 

// the IP address 
{ 
struct in_addr *ia; 
ia = (struct in_addr *) ((ifr->ifr_ifru.ifru_addr.sa_data)+2); 
printf("%6s %-15s\n", ifr->ifr_name, inet_ntoa(*ia)); 
} 

// mac address 
{ 
int mib[6], len; 

mib[0] = CTL_NET; 
mib[1] = AF_ROUTE; 
mib[2] = 0; 
mib[3] = AF_LINK; 
mib[4] = NET_RT_IFLIST; 
mib[5] = if_nametoindex(ifr->ifr_name); 

if (mib[5] == 0) 
{ 
printf("error calling if_nametoindex\n"); 
continue; 
} 

if (sysctl(mib, 6, NULL, (size_t*)&len, NULL, 0) < 0) 
{ 
printf("sysctl 1 error\n"); 
continue; 
} 

char * macbuf = (char*) malloc(len); 
if (sysctl(mib, 6, macbuf, (size_t*)&len, NULL, 0) < 0) 
{ 
printf("sysctl 2 error"); 
continue; 
} 

struct if_msghdr * ifm = (struct if_msghdr *)macbuf; 
struct sockaddr_dl * sdl = (struct sockaddr_dl *)(ifm + 1); 
unsigned char * ptr = (unsigned char *)LLADDR(sdl); 
printf("MacAddress: %02x:%02x:%02x:%02x:%02x:%02x\n", *ptr, 
*(ptr+1), *(ptr+2), 
*(ptr+3), *(ptr+4), *(ptr+5)); 

free(macbuf); 
} 

} 

// clean up 
free(buf); 
close(socketID); 

return 0; 
}
