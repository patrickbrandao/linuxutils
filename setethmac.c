

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

#include <string.h>

#include <sys/types.h>
#include <net/if_arp.h>

#include <assert.h>
#include <string.h>
 

int main(int argc, char **argv){

	if(argc < 3 || argc > 3){
		printf("\n");
		printf("setethmac\n");
		printf("Altera o endereco MAC da interface de rede\n");
		printf("\n");
		printf("Autores: Patrick Brandao <patrickbrandao@gmail.com>, Marcio Araujo <marcioxmarcio@gmail.com>, todos os direitos reservados\n");
		printf("\n");
		printf("Use: setethmac (ethX[.nnn]) (new mac address)\n");
		printf("\n");
		return(1);	
	}

    struct ifreq ifr;
    int s;
    char *mac=NULL;
	char *dev=NULL;

	int mac_len = 0;
 
 	if(argc < 3){
		printf("use: setdevmac (dev) (mac)\n");
		return EXIT_FAILURE;
	}

	dev = argv[1];
	mac = argv[2];

	// validar mac address
	mac_len = strlen(mac);
	if(mac_len != 17){
		printf("invalid mac address\n");
		return EXIT_FAILURE;
	}

	sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
		&ifr.ifr_hwaddr.sa_data[0],
		&ifr.ifr_hwaddr.sa_data[1],
		&ifr.ifr_hwaddr.sa_data[2],
		&ifr.ifr_hwaddr.sa_data[3],
		&ifr.ifr_hwaddr.sa_data[4],
		&ifr.ifr_hwaddr.sa_data[5]
	);
 
    s = socket(AF_INET, SOCK_DGRAM, 0);
    assert(s != -1);
 
    strcpy(ifr.ifr_name, argv[1]);
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    assert(ioctl(s, SIOCSIFHWADDR, &ifr) != -1);
 
    return EXIT_SUCCESS;
}