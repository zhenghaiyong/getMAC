
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdlib.h>

#include <net/if_dl.h>
#include <string.h>

bool get_mac_address(unsigned char* mac_addr, const char* if_name = "en0")
{
    ifaddrs* iflist;
    bool found = false;
    printf("if_name: %s\n", if_name);
    if (getifaddrs(&iflist) == 0) {
        for (ifaddrs* cur = iflist; cur; cur = cur->ifa_next) {
            if ((cur->ifa_addr->sa_family == AF_LINK) &&
                    (strcmp(cur->ifa_name, if_name) == 0) &&
                    cur->ifa_addr) {
              sockaddr_dl* sdl = (sockaddr_dl*)cur->ifa_addr;
              //LLADDR(s) ((caddr_t)((s)->sdl_data + (s)->sdl_nlen))
              memcpy(mac_addr, LLADDR(sdl), sdl->sdl_alen);

              printf("sdl_alen: %d\n", sdl->sdl_alen);
              for(int i=0; i<sdl->sdl_alen; i++){
                printf("%02x:",mac_addr[i]);
              }

              found = true;
              break;
            }
        }

        freeifaddrs(iflist);
    }
    return found;
}

int main(int argc, char** argv)
{
  unsigned char MAC[200];
  const char* DeviceName = "en0";

  get_mac_address(MAC,DeviceName);
  //  printf("(ether proto 0x888e) and (ether dst host %02x:%02x:%02x:%02x:%02x:%02x)\n",MAC[0],MAC[1],MAC[2],MAC[3],MAC[4],MAC[5]);
  //  printf("%02x, %d\n",MAC[3],MAC[3]);
  //  printf("%02x, %d\n",MAC[5],MAC[5]);
  return 0;

}
