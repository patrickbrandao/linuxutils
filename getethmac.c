
#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>

#include <string.h>

// Obter o endereco MAC de uma interface


void help_std(){
	printf("\n");
	printf("getethmac\n");
	printf("Informa o endereco MAC de uma interface de rede ethernet (ou vlan)\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: getethmac (ethX[.nnn])\n");
	printf("\n");
	exit(1);	
}

int main(int argc, char *argv[]){
	struct ifreq skt;
	struct ifaddrs *loc_ifaddr;

	if (getifaddrs(&loc_ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	if(argc < 2){
		help_std();
	}

	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	strcpy(skt.ifr_name, argv[1]);
	
	if(0==ioctl(fd, SIOCGIFHWADDR, &skt)){
		printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
			(unsigned char) skt.ifr_addr.sa_data[0],
			(unsigned char) skt.ifr_addr.sa_data[1],
			(unsigned char) skt.ifr_addr.sa_data[2],
			(unsigned char) skt.ifr_addr.sa_data[3],
			(unsigned char) skt.ifr_addr.sa_data[4],
			(unsigned char) skt.ifr_addr.sa_data[5]
		);		
	}
}


