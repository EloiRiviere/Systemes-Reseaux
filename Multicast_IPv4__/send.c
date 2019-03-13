#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>     /* pour memset */
#include <errno.h>
#include <unistd.h>     // pour le read
#include <netdb.h>      /* pour getaddrinfo*/
#include <arpa/inet.h>
#include <ctype.h>      /* pour toupper */

#define DGRAM_MAX 1024  /* taille MAX en réception */

#define GROUP "230.0.0.0"
#define PORT "5000"
#define DGRAM_MAX 1024
#define SERVER_NAME "localhost"

int main()
{
  printf("Serveur\n");

  int s, ret;
  socklen_t len_src_addr;
  struct addrinfo hints, *result;
	struct sockaddr_storage src_addr;
	char request[DGRAM_MAX];

  memset(&hints, 0, sizeof(struct addrinfo));
  // hints.ai_flags = 0;
  // hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_flags = AI_PASSIVE; /* For wildcard IP address */
  hints.ai_family = AF_INET6;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  ret = getaddrinfo(SERVER_NAME, PORT, &hints, &result);
	if (ret != 0)
  {
	   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
	   exit(EXIT_FAILURE);
	}

  /* Création de la socket IPv4/IPv6 */
	if((s=socket(result->ai_family, result->ai_socktype, result->ai_protocol))==-1)
  {
		perror("socket"); exit(1);
	}

  /* Attachement de la socket */
  if (bind(s, result->ai_addr, result->ai_addrlen) == -1)
  {
    perror("bind"); exit(1);
  }

  freeaddrinfo(result);

  // fixer champ TTL (Time To Live)
  int ttl=2;
  if(setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl)) == -1)
  {
     perror("set IP_MULTICAST_TTL");
  }

  while(1)
  {
     /* Attente et lecture d'une requête */
     len_src_addr=sizeof src_addr;
     if((ret=recvfrom(s, request, DGRAM_MAX-1, 0, (struct sockaddr*) &src_addr, &len_src_addr))==-1)
     {
       perror("recvfrom"); exit(1);
     }
     request[ret]=0;

     puts(request);

     /* traitement de la requête(passage en majuscule) */
     {
        int i=0;

        while(request[i]) {
       request[i]=toupper(request[i]);
       ++i;
        }
     }

    /* Émission du datagramme réponse */
    if(sendto(s, request, strlen(request)+1, 0, (struct sockaddr*) &src_addr, sizeof(src_addr))==-1)
    {
      perror("sendto"); exit(1);
    }
  }

return 0;
}
