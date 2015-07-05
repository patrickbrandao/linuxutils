#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <resolv.h>

#include <linux/rtnetlink.h>
#include <linux/if_arp.h> // interface up or down


struct rtnl_handle{int fd; struct sockaddr_nl local; struct sockaddr_nl	peer; __u32	seq;__u32 dump;};
//struct rtnl_handle rth;

int rcvbuf = 1024 * 1024;
int preferred_family = AF_UNSPEC; //ipb4 e ipv6    
//int oneline = 0;

typedef int (*rtnl_filter_t)(const struct sockaddr_nl *,struct nlmsghdr *n, void *);
struct rtnl_dump_filter_arg{rtnl_filter_t filter;void *arg1;rtnl_filter_t junk;void *arg2;};
typedef struct{__u8 family;__u8 bytelen;__s16 bitlen;__u32 flags;__u32 data[8];} inet_prefix;



int rtnl_dump_filter_l(struct rtnl_handle *rth,const struct rtnl_dump_filter_arg *arg){
	struct sockaddr_nl nladdr;
	struct iovec iov;
	struct msghdr msg = {
            .msg_name = &nladdr,
            .msg_namelen = sizeof(nladdr),
            .msg_iov = &iov,
            .msg_iovlen = 1,
        };
	char buf[16384];

	iov.iov_base = buf;
	while (1) {
		int status;	
                const struct rtnl_dump_filter_arg *a;
		iov.iov_len = sizeof(buf);
                status = recvmsg(rth->fd, &msg, 0);
		if (status < 0) {
			if (errno == EINTR || errno == EAGAIN)	continue;
			fprintf(stderr, "netlink receive error %s (%d)\n",strerror(errno), errno);
                        strcpy(buf,"");
			return -1;
		}

		if (status == 0) {fprintf(stderr, "EOF on netlink\n"); strcpy(buf,""); return -1;}

		for (a = arg; a->filter; a++) {
                    
			struct nlmsghdr *h = (struct nlmsghdr*)buf;

			while (NLMSG_OK(h, status)) {
				int err;
				if (nladdr.nl_pid != 0 || h->nlmsg_pid != rth->local.nl_pid || h->nlmsg_seq != rth->dump) {
					if (a->junk) {err = a->junk(&nladdr, h,a->arg2);
						if (err < 0){
                                                    strcpy(buf,"");
                                                    free(h);
                                                    return err;
                                                }    
					}
					goto skip_it;
				}

				if (h->nlmsg_type == NLMSG_DONE){strcpy(buf,""); return 0;}
				if (h->nlmsg_type == NLMSG_ERROR) {
					struct nlmsgerr *err = (struct nlmsgerr*)NLMSG_DATA(h);
					if (h->nlmsg_len < NLMSG_LENGTH(sizeof(struct nlmsgerr))) {fprintf(stderr, "ERROR truncated\n");
					} else {errno = -err->error; perror("RTNETLINK answers");}
					return -1;
				}
				err = a->filter(&nladdr, h, a->arg1);
				if (err < 0){
                                    free(h);
                                    strcpy(buf,"");
                                    return err;
                                }    
skip_it:
				h = NLMSG_NEXT(h, status);
			}
		} while (0);
		if (msg.msg_flags & MSG_TRUNC) {fprintf(stderr, "Message truncated\n");	continue;}
		if (status) {fprintf(stderr, "!!!Remnant of size %d\n", status); exit(1);}
	}
        
        
}

int rtnl_dump_filter(struct rtnl_handle *rth,rtnl_filter_t filter, void *arg1, rtnl_filter_t junk,  void *arg2){
	const struct rtnl_dump_filter_arg a[2] = {
		{ .filter = filter, .arg1 = arg1, .junk = junk, .arg2 = arg2 },
		{ .filter = NULL,   .arg1 = NULL, .junk = NULL, .arg2 = NULL }
	};
	return rtnl_dump_filter_l(rth, a);
}

int parse_rtattr(struct rtattr *tb[], int max, struct rtattr *rta, int len){
	memset(tb, 0, sizeof(struct rtattr *) * (max + 1));
	while (RTA_OK(rta, len)) {
		if (rta->rta_type <= max)tb[rta->rta_type] = rta;rta = RTA_NEXT(rta,len);
	}
	if (len)fprintf(stderr, "!!!Deficit %d, rta_len=%d\n", len, rta->rta_len);
        
        
	return 0;
}

static struct{
    int ifindex;
    int family;
    inet_prefix pfx;
    int scope, scopemask;
    int flags, flagmask;
    int up;
    char *label;
    int flushed;
    char *flushb;
    int flushp;
    int flushe;
} filter;

struct nlmsg_list{
	struct nlmsg_list *next;
	struct nlmsghdr	  h;
};

static int store_nlmsg(const struct sockaddr_nl *who, struct nlmsghdr *n, void *arg){
	struct nlmsg_list **linfo = (struct nlmsg_list**)arg;
	struct nlmsg_list *h;
	struct nlmsg_list **lp;

	h = malloc(n->nlmsg_len+sizeof(void*));
	if (h == NULL)
		return -1;

	memcpy(&h->h, n, n->nlmsg_len);
	h->next = NULL;

	for (lp = linfo; *lp; lp = &(*lp)->next);
	*lp = h;
	
	return 0;
}

//  funcao para abrir o soquete netlink
int do_open_soquete(struct rtnl_handle *rth){
	socklen_t addr_len;
	int sndbuf = 32768;
	memset(rth, 0, sizeof(*rth));
	rth->fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

	if (rth->fd < 0) {return -1;} //perror("Nao e possivel abrir soquete netlink");
	if (setsockopt(rth->fd,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf)) < 0) {return -1;} //perror("SNDBUF : Nao foi possivel enviar dados ao buffer netlink");
	if (setsockopt(rth->fd,SOL_SOCKET,SO_RCVBUF,&rcvbuf,sizeof(rcvbuf)) < 0) {return -1;} //perror("RCVBUF: Nao foi possivel receber dados do buffer netlink");
	memset(&rth->local, 0, sizeof(rth->local));
	rth->local.nl_family = AF_NETLINK;
	rth->local.nl_groups = 0;
	if (bind(rth->fd, (struct sockaddr*)&rth->local, sizeof(rth->local)) < 0) {return -1;} //perror("Nao e possivel vincular netlink ");
	addr_len = sizeof(rth->local);
	if (getsockname(rth->fd, (struct sockaddr*)&rth->local, &addr_len) < 0) {return -1;} //perror("Nao e possivel saber o nome do soquete");
	if (addr_len != sizeof(rth->local)) {return -1;} //fprintf(stderr, "Comprimento de endereço errado %d\n", addr_len);
	if (rth->local.nl_family != AF_NETLINK) {return -1;} //fprintf(stderr, "Familia de endereco errado %d\n", rth->local.nl_family);
	rth->seq = time(NULL);
        
        
	return 0;
}



int main(int argc, char **argv){

	if(argc > 1){
		printf("\n");
		printf("lsbr\n");
		printf("Lista de bridges\n");
		printf("\n");
		printf("Autores: Patrick Brandao <patrickbrandao@gmail.com>, Marcio Araujo <marcioxmarcio@gmail.com>, todos os direitos reservados\n");
		printf("\n");
		return(1);	
	}

	struct rtnl_handle rth = { .fd = -1 };
	
	if (do_open_soquete(&rth) < 0) return 0;

	struct nlmsg_list *linfo = NULL;
	struct nlmsg_list *ainfo = NULL;
	struct nlmsg_list *l, *n;        
	
	memset(&filter, 0, sizeof(filter));

// request        
	struct {
            struct nlmsghdr nlh;
            struct rtgenmsg g;
        } req;

	memset(&req, 0, sizeof(req));
	req.nlh.nlmsg_len = sizeof(req);
	req.nlh.nlmsg_type = RTM_GETLINK;
	req.nlh.nlmsg_flags = NLM_F_ROOT|NLM_F_MATCH|NLM_F_REQUEST;
	req.nlh.nlmsg_pid = 0;
	req.nlh.nlmsg_seq = rth.dump = ++rth.seq;
	req.g.rtgen_family = preferred_family;
	if(send(rth.fd, (void*)&req, sizeof(req), 0)< 0 ) exit(1);
        
        
       if (rtnl_dump_filter(&rth, store_nlmsg, &linfo, NULL, NULL) < 0) {exit(1);}//Dump terminado    
        
                
        for (l=linfo; l; l = n) {
            char *devname;
			int dlen;
			int show = 0;
			
            n = l->next; 
             
            struct nlmsghdr *N = &l->h;

            struct ifinfomsg *ifi = NLMSG_DATA(N);
            struct rtattr *tb[IFLA_MAX+1];
            int len = N->nlmsg_len;
         

           
            len -= NLMSG_LENGTH(sizeof(*ifi));	
            if (len < 0) continue;
            
            parse_rtattr(tb, IFLA_MAX, IFLA_RTA(ifi), len);
            
			devname = (char*)RTA_DATA(tb[IFLA_IFNAME]);
			
			// Filtros:
			dlen = strlen(devname);
	
			// todas as ethX, athX, wlanX tem no minimo 4 bytes	
			if(dlen < 4) continue;

			// pular tipo nao-eth ou nao-ath ou nao-wlan
			if( strncmp ( devname, "br", 2 ) == 0) show = 1;

			if(!show) continue;
	
			// exibir nome da interface
			printf("%s\n", devname);
            
        }
        
        
        fflush(stdout);
        free(l);
        
        
       return 0; 
}



