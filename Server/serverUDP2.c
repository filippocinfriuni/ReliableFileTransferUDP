#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define SERV_PORT   5193
#define MAXLINE     1024

struct thread_info {    /* Used as aragument to thread_start() */
  pthread_t thread_id;        /* ID returned by pthread_create() */
  int       thread_num;       /* Application-defined thread # */
  char     *argv_string;      /* From command-line argument */
};

struct arg_struct {
  struct sockaddr_in addr; //
  char buff[MAXLINE]; //Buffer dove viene salvata la richiesta del client 
};

int create_thread(char buff[MAXLINE], struct sockaddr_in addr);
void clientRequestManager(struct arg_struct* arg);

//IMPOSTARE SETUP INIZIALE

int main(int argc, char **argv) {
  int sockfd;
  socklen_t lenadd = sizeof(struct sockaddr_in);
  struct sockaddr_in addr;
  char buff[MAXLINE];
  int i = 0;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { /* crea il socket */
    perror("errore in socket");
    exit(1);
  }

  memset((void *)&addr, 0, lenadd); // Imposta a zero ogni bit dell'indirizzo IP
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY); // Il server accetta pacchetti su una qualunque delle sue interfacce di rete 
  addr.sin_port = htons(SERV_PORT); // numero di porta del server 

  if (bind(sockfd, (struct sockaddr *)&addr, lenadd) < 0) { // assegna l'indirizzo al socket 
    perror("errore in bind");
    exit(1);
  }
  
  while (1) { //RECVFROM
    if ( (recvfrom(sockfd, buff, MAXLINE, 0, (struct sockaddr *)&addr, &lenadd)) < 0) {
      perror("errore in recvfrom\n");
      exit(1);
    }
   
    printf("Ricevuto messaggio da IP: %s e porta: %i\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
    printf("Contenuto:%s\n", buff);

    /* Creazione Thread per ogni richiesta, poich� abbiamo sicuramente ricevuto qualcosa */
    printf("thread creato: %d \n",create_thread(buff,addr));
    
  }
  exit(0);
}

int create_thread(char buff[MAXLINE], struct sockaddr_in addr){
  pthread_t tinfo = 0;
  
  /* Allocate memory for pthread_create() arguments */
  struct arg_struct* p_arg = malloc(sizeof(struct arg_struct));
  p_arg->addr = addr;
  strncpy(p_arg->buff,buff,MAXLINE);

  /* The pthread_create() call, stores the thread ID into corresponding element of tinfo[] */
  if ( pthread_create(&tinfo, NULL, (void *)clientRequestManager, p_arg)) { 
    printf("pthread_create failed \n");
    exit(1);
  }
  
  return tinfo;
}

void clientRequestManager(struct arg_struct* arg){ 
  /* head: (lunghezza fissa)
    tipo di messaggio (2bit)
    lunghezza contenuto payload (2byte)
    flag errore (1 bit)
    request number (13 bit, per completare 2 byte)
    ack number (1 bit) l'ack del msg num pacchetto tot relativo alla request number
    num. offset (4byte)
    last packet flag (1byte)
    pacchetto: (lunghezza fissa)
    payload nel pacchetto: (lunghezza var)
  */

  //----------------- CREAZIONE SOCKET -----------------
  int sockReq;
  if (( sockReq = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { /* crea socket per nuovo thread */
    perror("errore in socket");
    exit(1);
  }
  
  struct sockaddr_in c_addr = arg->addr;

  //----------------- GESTIONE RICHIESTA -----------------
  char* binary = (char*)malloc (sizeof (char) * 100);
	binary = charToBinary(arg->buff,binary);
  printf("\n%s\n\n",arg->buff);
  readRequest(binary);
  free(binary);

  // creo il socket
  int sockfd;
  struct sockaddr_in clientaddr;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if ((sockfd) < 0) {			// controllo errori
		perror("errore in socket");
		exit(1);
	}
	memset((void *)&clientaddr, 0, sizeof(clientaddr));	// azzera servaddr
	clientaddr.sin_family = AF_INET;			// assegna il tipo di indirizzo
	clientaddr.sin_port = htons(SERV_PORT);		// assegna la porta del server
	/* assegna l'indirizzo del server, preso dalla riga di comando.
	L'indirizzo è una stringa da convertire in intero secondo network byte order. */
	if (inet_pton(AF_INET, indirizzoServ, &servaddr.sin_addr) <= 0) {
		/* inet_pton (p=presentation) vale anche per indirizzi IPv6 */
		fprintf(stderr, "errore in inet_pton per %s", indirizzoServ);
		exit(1);
	}
  
  char risposta[MAXLINE];
  while(1){
  	scanf("%s", risposta);
  	printf("risposta inserita: %s \n", risposta);
  	//RISPOSTA AL CLIENT, 3) Rispondere, con l'invio numero di porta (se serve) su cui inviare il file 
  	if(sendto(sockReq, risposta, strlen(risposta), 0, (struct sockaddr_in*)&(arg->addr), sizeof(struct sockaddr_in)) < 0) {
    		perror("errore in sendto");
    		exit(1);
  	}
    if(strncmp(risposta, "stop", 4) == 0){
      break;
    }
  }
  

  free(arg);
  printf("Risposta inviata\n");
}
