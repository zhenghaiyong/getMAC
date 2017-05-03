#include <sys/param.h>;
#include <sys/ioctl.h>;
#include <sys/socket.h>;
#include <sys/sysctl.h>;

#include <net/ethernet.h>;
#include <net/if.h>;
#include <net/if_var.h>;
#include <net/if_dl.h>;
#include <net/if_types.h>;

#include <netinet/in.h>;
#include <netinet/in_var.h>;
#include <arpa/inet.h>;

#include <err.h>;
#include <errno.h>;
#include <fcntl.h>;
#include <stdio.h>;
#include <stdlib.h>;
#include <string.h>;
#include <unistd.h>;

int main(int argc,char **argv)
{
struct ifreq *ifrp,ifr;
struct ifconf ifc;
char buffer[720],name[16];
int socketfd,error,flags,len,space=0;
ifc.ifc_len=sizeof(buffer);
len=ifc.ifc_len;
ifc.ifc_buf=buffer;

socketfd=socket(AF_INET,SOCK_DGRAM,0);

if((error=ioctl(socketfd,SIOCGIFCONF,&ifc))<0)
	{
	perror("ioctl faild");
	exit(1);
	}

if(ifc.ifc_len<=len)
	{
	ifrp=ifc.ifc_req;
	do
	{
	struct sockaddr *sa=&ifrp->ifr_addr;
    strcpy(ifr.ifr_name,ifrp->ifr_name);
	if(strcmp(ifrp->ifr_name,name)!=0){
	strcpy(name,ifrp->ifr_name);
	printf("%s:",ifrp->ifr_name);
	if(!ioctl(socketfd,SIOCGIFFLAGS,&ifr))
	{
	flags=ifr.ifr_flags;
	printf("flags=%x<",flags<<16);
	if(ifr.ifr_flags&IFF_UP){
	printf("UP,");
	}
	else{
	printf("DOWN");
	}
	if(ifr.ifr_flags&IFF_BROADCAST){
	printf("BOROADCAST,");
	}
	if(ifr.ifr_flags&IFF_POINTOPOINT){
	printf("POINTOPOINT,");
	}
	if(ifr.ifr_flags&IFF_LOOPBACK){
	printf("LOOPBACK,");
	}
	if(ifr.ifr_flags&IFF_RUNNING){
	printf("RUNNING,");
	}
	if(ifr.ifr_flags&IFF_SIMPLEX){
	printf("SIMPLEX,");
	}
	if(ifr.ifr_flags&IFF_MULTICAST){
	printf("MULTICAST");
	}
	printf(">;\n");
	}
	if(!ioctl(socketfd,SIOCGIFADDR,&ifr)){
	printf("     inet %s",inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->;sin_addr));
	}
	if(!ioctl(socketfd,SIOCGIFNETMASK,&ifr)){
	printf(" netmask %s",inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->;sin_addr));
	}
	if(!ioctl(socketfd,SIOCGIFBRDADDR,&ifr)){
	printf(" broadcast %s\n",inet_ntoa(((struct sockaddr_in *)&ifr.ifr_broadaddr)->;sin_addr));
	}
	if(((struct sockaddr_dl *)sa)->;sdl_type==IFT_ETHER)
	printf("     ether %s\n",ether_ntoa((struct ether_addr *)LLADDR((struct sockaddr_dl *)sa)));
	//printf("types %x\n",((struct sockaddr_dl *)sa)->;sdl_type);
	}
	ifrp=(struct ifreq*)(sa->;sa_len+(caddr_t)&ifrp->;ifr_addr);
	space+=(int)sa->;sa_len+sizeof(ifrp->;ifr_name);
	}
	while(space<ifc.ifc_len);
	}
	exit(0);
}
