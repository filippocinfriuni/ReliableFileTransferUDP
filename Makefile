# parametri client
NOME_PROG_CLIENT =	client
CARTELLA_CLIENT =	Client
CARTELLA_FUNZ_AGG_C =	funzioniAusiliarie
VERSIONE_CLIENT =	7

# parametri server
NOME_PROG_SERVER =	server
CARTELLA_SERVER =	Server
CARTELLA_FUNZ_AGG_S =	funzioniAusiliarie
VERSIONE_SERVER =	4


PROG_PER_CLIENT = ./$(CARTELLA_CLIENT)/$(CARTELLA_FUNZ_AGG_C)/aContrRichiesta.c ./$(CARTELLA_CLIENT)/$(CARTELLA_FUNZ_AGG_C)/scarta.c ./$(CARTELLA_CLIENT)/$(CARTELLA_FUNZ_AGG_C)/bitManipulation.c
PROG_PER_SERVER = ./$(CARTELLA_CLIENT)/$(CARTELLA_FUNZ_AGG_C)/bitManipulation.c


CC = gcc
CFLAGS = -pthread -DPRINT #-w
		# -pthread: per consentire esecuzione thread, LASCIARLO ATTIVO
		# -w: per compilare senza che cc mostri i warnings a terminale
		# -DPRINT: fa la stampa di alcuni informazioni aggiuntive per il debug durante l'esecuzione del client

# RICORDA DI ELIMINARE I COMMENTI
# OBJECTS=helloworld.o #File oggetto
# SRCS=helloworld.c #File sorgente


client: ./$(CARTELLA_CLIENT)/clientUDP$(VERSIONE_CLIENT).c $(PROG_PER_CLIENT)
	$(CC) -o $(NOME_PROG_CLIENT) ./$(CARTELLA_CLIENT)/clientUDP$(VERSIONE_CLIENT).c $(PROG_PER_CLIENT) $(CFLAGS)
	
server: ./$(CARTELLA_SERVER)/serverUDP$(VERSIONE_SERVER).c $(PROG_PER_SERVER)
	$(CC) -o $(NOME_PROG_SERVER) ./$(CARTELLA_SERVER)/serverUDP$(VERSIONE_SERVER).c $(PROG_PER_SERVER) $(CFLAGS)
	
clean:
	rm $(NOME_PROG_SERVER)
	rm $(NOME_PROG_CLIENT)
	
cleanserver:
	rm $(NOME_PROG_SERVER)

cleanclient:
	rm $(NOME_PROG_CLIENT)
