#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>

#include <netdb.h>      /* pour getaddrinfo*/
#include <string.h>     /* pour memset */
#include <arpa/inet.h>  /* pour inet_ntop */
#define DGRAM_MAX 1024  /* taille MAX en réception */

#define GROUP "230.0.0.0"
#define PORT "5000"
#define SERVER_NAME "localhost"

// quitter groupe multicast
void quitter_groupe(int s, struct ip_mreqn *mreqn)
{
  // if(setsockopt(sdr, IPPROTO_IP, IP_DROP_MEMBERSHIP, mreqn, 	sizeof(mreqn)) == -1)
  if(setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, mreqn, sizeof(mreqn)) == -1)
  {
    perror("setsockopt: IP_DROP_MEMBERSHIP");
    printf("J'ai quitté le groupe !\n");
  }
  else
  {
    printf("Je n'ai pas pu quitter le groupe !\n");
  }
}

// rejoindre groupe multicast
void rejoindre_groupe(int s, struct ip_mreqn *mreqn)
{
  // if (setsockopt(sdr, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) mreq, sizeof(struct ip_mreq)) < 0)
  if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) mreqn, sizeof(struct ip_mreq)) < 0)
  {
    perror("setsockopt – IP_ADD_MEMBERSHIP");
    printf("J'ai rejoint le groupe !\n");
  }
  else
  {
    printf("Je n'ai pas pu rejoindre le groupe !\n");
  }
}

void memset_hints(struct addrinfo *hints)
{
  memset(hints, 0, sizeof(struct addrinfo));
  hints->ai_flags = 0;
  hints->ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints->ai_socktype = SOCK_DGRAM; /* Datagram socket */
  hints->ai_protocol = 0;          /* Any protocol */
  hints->ai_canonname = NULL;
  hints->ai_addr = NULL;
  hints->ai_next = NULL;
}

void creation_socket(int *s, struct addrinfo *result)
{
  /* Création de la socket IPv4/IPv6 */
  if((*s=socket(result->ai_family, result->ai_socktype, result->ai_protocol))==-1)
  {
    perror("socket"); exit(1);
  }
}

void attachement_socket(int s, struct addrinfo *result)
{
  /* Attachement de la socket */
	if (bind(s, result->ai_addr, result->ai_addrlen) == -1)
  {
		perror("bind"); exit(1);
  }
  freeaddrinfo(result);
}

struct ip_mreqn generate_mreqn(int s)
{
  struct ip_mreqn mreqn;
  if(inet_aton(GROUP, &mreqn.imr_multiaddr)==0)
  {
  	fprintf(stderr,"Pb Adr multicast : %s !\n", GROUP);
  	exit(1);
  }
  mreqn.imr_address.s_addr = htonl(INADDR_ANY);
  mreqn.imr_ifindex=0; /* n'importe quelle interface */
  if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &mreqn, sizeof(struct ip_mreq)) < 0)
  {
  	perror("setsockopt – IP_ADD_MEMBERSHIP");
  }

  return mreqn;
}

int main()
{
  printf("Client\n");

  // structures
  int s, ret;
  struct addrinfo hints, *result;
  struct sockaddr_storage src_addr;
	socklen_t len_src_addr;
 	char response[DGRAM_MAX];

  memset_hints(&hints);

  ret = getaddrinfo(SERVER_NAME, PORT, &hints, &result);
	if (ret != 0)
  {
	   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
	   exit(EXIT_FAILURE);
	}

  creation_socket(&s, result);

  struct ip_mreqn mreqn = generate_mreqn(s);

  rejoindre_groupe(s,&mreqn);


  char login[DGRAM_MAX];
  printf("Entrez votre login:\n");
  scanf("%s",&login);
  strcpy(strcat(login,":\t"),login);
  char msg[DGRAM_MAX];
  char requete[DGRAM_MAX];
  while(1)
  {
    printf("Entrez votre message ou 'q' pour quiter:\n");
    fgets(msg, DGRAM_MAX-strlen(login)+2, stdin);

    if(strcmp(msg,"q\n") == 0)
    {
      printf("Déconnexion\n");
      break;
    }

    printf("login: %s\n",login);
    printf("msg: %s\n",msg);

    strcpy(requete,login);
    strcat(requete,msg);

    printf("cat: %s\n",requete);

    /* Émission du datagramme */
    if(sendto(s, requete, strlen(requete)+1, 0, result->ai_addr, result->ai_addrlen)==-1)
    {
      perror("sendto"); exit(1);
    }

    /* Attente et lecture de la réponse */
    len_src_addr=sizeof(src_addr);
    if((ret=recvfrom(s, response, DGRAM_MAX-1, 0, (struct sockaddr*) &src_addr, &len_src_addr))==-1)
    {
      perror("recvfrom"); exit(1);
    }
    response[ret]=0;

    /* Traitement de la réponse */
    puts(response);
  }
  freeaddrinfo(result); // si result ne sert plus
  quitter_groupe(s,&mreqn);
	return 0;
}
