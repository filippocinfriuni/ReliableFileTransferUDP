/*  */

#define MAX_SIZE_LEN_FIELD	4

ssize_t writeN(int fd, const void *buf, size_t n);
// spiegazione

int readN(int fd, void *buf, size_t n);
// spiegazione

int controllaRichiesta(char * richiesta); //sistemare prima c'era MAXLINE !!
// spiegazione

int scarta(void);
// per determinare se il pacchetto va scartato

char setBit(char n, int k);

int readBit(char n, int k);

void headerPrint(char * header){
    int i;

    // stampo l'header
	printf("HEADER\n");
    printf("richiesta: ");
    for(i=0; i<8; i++){
        printf("%d ", readBit(header[0], 8-1-i));
    }
    printf(" - payload len: ");
    printf("%hu", header[1]);
    printf(" - offset: ");
    if(MAX_SIZE_LEN_FIELD == 4){
        printf("%lu \n", header[3]);
    }
    else{
        printf("%llu \n", header[3]);
    }
}