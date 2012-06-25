#Bib + Include
LIBS = -L../libircclient-1.6/src/ -lircclient -lpthread -lnsl
INCLUDES = -I../libircclient-1.6/include/

BoBot: bobot.c
	gcc -o BoBot bobot.c -lsqlite3 $(LIBS) $(INCLUDES)
