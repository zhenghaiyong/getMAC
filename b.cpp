#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdlib.h>

#include <net/if_dl.h>
#include <string.h>

int main(){
  struct ifaddrs * ifad;
  struct ifaddrs * curr;
  char * mac_addr;

  getifaddrs(&ifad);


  for(curr = ifad; curr != NULL; curr = curr->ifa_next){
    if ((curr->ifa_addr->sa_family == AF_LINK) &&
        curr->ifa_addr) {
      
      printf("name %s\n", curr->ifa_name);
      printf("addr -%s-\n", (char *)(curr->ifa_addr)->sa_data);
      sockaddr_dl* sdl = (sockaddr_dl*)curr->ifa_addr;
      memcpy(mac_addr, LLADDR(sdl), sdl->sdl_alen);
      printf("addr -%s-\n", (char *)(mac_addr));
                
    }
  }
  freeifaddrs(ifad);
}
