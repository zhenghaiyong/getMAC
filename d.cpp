#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
int show_eths()
{
  int i=0;
  int sockfd;
  struct ifconf ifconf;
  unsigned char buf[512];
  struct ifreq ifreq;    //初始化ifconf
  ifconf.ifc_len = 512;
  ifconf.ifc_buf = buf;
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
  {
    perror("socket");
     return 1;
  }
  ioctl(sockfd, SIOCGIFCONF, &ifconf);    //获取所有接口信息
  //接下来一个一个的获取IP地址
  ifreq = (struct ifreq*)buf;
  for(i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--)
  {
   // if(ifreq->ifr_flags == AF_INET){            //for ipv4
    printf("name = [%s]\n", ifreq->ifr_name);
    printf("local addr = [%s]\n",
    inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
    ifreq++;
  }
  return 0;
}
int DG_AL_eth_chip_link_status(D_VOID)
{
 int fd;
 struct ifreq ifr;
 int ret = 0;
 
 if (( fd = socket( AF_INET, SOCK_DGRAM, IPPROTO_IP )) <= 0)
 {
  printf("%s: socket err.\n", __func__);
  return 0;
 }
 memset(&ifr, 0, sizeof(ifr));
 strncpy(ifr.ifr_name, "eth0", sizeof(ifr.ifr_name) - 1);
 if (ioctl(fd, SIOCGIFFLAGS, (char *)&ifr) < 0)
 {  
  close(fd); 
  printf("%s: ioctl err.\n", __func__);
  return 0;  
 }
 if (((ifr.ifr_flags) & IFF_UP) &&(ifr.ifr_flags & IFF_RUNNING))
 {
  ret = 1;
 }
 else
 {
  ret = 0;
 }
        printf("IFF_RUNNING=0x%x,ifr.ifr_flags=0x%x\n",IFF_RUNNING,ifr.ifr_flags);
 close(fd); 
 return ret;
}
int main(int argc,char *argv[])
{
    int ret=0;
    ret=DG_AL_eth_chip_link_status();
    show_eths();
    printf("ret=%d\n",ret);
    return 0;
}
