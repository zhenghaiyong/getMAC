#include <assert.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include <net/if_dl.h>
/*
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
*/
static
void GetMacFromDevice(uint8_t mac[6], const char *devicename)
{
  /*
  int	sock;
  struct ifreq ifreq;
  sock=socket(AF_INET,SOCK_STREAM,0);
  strcpy(ifreq.ifr_name,devicename);
  //ioctl(sock,SIOCGIFHWADDR,&ifreq);
  ioctl(sock,SIOCGIFMAC,&ifreq);
  mac[0]=(uint8_t)ifreq.ifr_addr.sa_data[0],
    mac[1]=(uint8_t)ifreq.ifr_addr.sa_data[1],
    mac[2]=(uint8_t)ifreq.ifr_addr.sa_data[2],
    mac[3]=(uint8_t)ifreq.ifr_addr.sa_data[3],
    mac[4]=(uint8_t)ifreq.ifr_addr.sa_data[4],
    mac[5]=(uint8_t)ifreq.ifr_addr.sa_data[5]; 
  //printf("%s\n",ifreq.ifr_addr);
  */
  int fd;
  struct ifreq ifr;
  /*
#ifdef UNIX
  int err;
  fd = socket(PF_PACKET, SOCK_RAW, htons(0x0806));
#endif
  */
  //#ifdef MACOSX
  char buffer[512];
  struct ifconf ifc;
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  /*
  if((fd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
      perror("socket");
      exit(1);
    }
  */
  //#endif
  assert(fd != -1);
  
  assert(strlen(devicename) < IFNAMSIZ);
  //  strncpy(ifr.ifr_name, devicename, IFNAMSIZ);
  //  ifr.ifr_addr.sa_family = AF_INET;
  /*
#ifdef UNIX
  err = ioctl(fd, SIOCGIFHWADDR, &ifr);
  assert(err != -1);
  memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
  
  err = close(fd);
  assert(err != -1);
#endif
  */
  //#ifdef MACOSX
  //初始化 ifc 
  ifc.ifc_len = 512;
  ifc.ifc_buf = buffer;
  int result;
  result = ioctl(fd, SIOCGIFCONF, &ifc);
  printf("result: %d\n",result);
  printf("ifc.ifc_len: %d\n",ifc.ifc_len);
  printf("ifc.ifc_buf: %s\n",ifc.ifc_buf);
  printf("ifc.ifc_req: %s\n",ifc.ifc_req);
  printf("sizeof(ifreq): %d\n",sizeof(struct ifreq *));

  ifr = (struct ifreq *)buffer;
  int i;
  for(i=(ifc.ifc_len/sizeof(struct ifreq)); i>0; i--){
    if(ifr.ifr_flags == AF_INET){  //for ipv4
      printf("ifr.ifr_name[%d]: %s\n",IFNAMSIZ,ifr.ifr_name);
      printf("ifr.ifr_addr: %d\n",ifr.ifr_addr);
      struct sockaddr_dl *sdl = (struct sockaddr_dl *)&ifr.ifr_addr;
      memcpy(mac, LLADDR(sdl), 6);
    }
  }
  /*
  if (result >= 0) {
    printf("ifr.ifr_name[%d]: %s\n",IFNAMSIZ,ifr.ifr_name);
    printf("ifr.ifr_addr: %d\n",ifr.ifr_addr);
    struct sockaddr_dl *sdl = (struct sockaddr_dl *)&ifr.ifr_addr;
    memcpy(mac, LLADDR(sdl), 6);
  }
  */
  //#endif
  return;
}

int main(int argc, char** argv)
{
  uint8_t	MAC[6];
  const char DeviceName[]="en0";

  GetMacFromDevice(MAC, DeviceName);
  printf("(ether proto 0x888e) and (ether dst host %02x:%02x:%02x:%02x:%02x:%02x)\n",MAC[0],MAC[1],MAC[2],MAC[3],MAC[4],MAC[5]);
  printf("(ether proto 0x888e) and (ether dst host %d:%d:%d:%d:%d:%d)\n",MAC[0],MAC[1],MAC[2],MAC[3],MAC[4],MAC[5]);

}
