#			PROYECTO DE SISTEMAS OPERATIVOS
#			    I Parcial - 2011 2T
# 
# INTEGRANTES:
# 	- Andrea Obando N.
# 	- Fabricio Orrala P.
# -----------------------------------------------------------------------
# ARCHIVO:
#	Makefile
# 
# DESCRIPCION:
#	Este archivo Makefile contiene las reglas de compilacion que
#	se haran a los distintos codigos fuentes del proyecto
#		
#

SALIDA_CLIENTE = ebankCliente.o
SALIDA_SERVER = ebankServidor.o

HEADERS = -I ./headers/
SOURCES_CLIENTE = ./sources/ebank_cliente.c\
		  ./sources/cliente.c\
		  ./sources/transaccion.c

SOURCES_SERVER = ./sources/ebank_server.c

LIBS = -lpthread
CFLAGS = -g -o
FFLAGS = -Wall -pedantic

all:
	@echo 'Compilando Server...'
	gcc $(CFLAGS) $(SALIDA_SERVER) $(SOURCES_SERVER) $(HEADERS) $(LIBS) $(FFLAGS)
	@echo 'Compilando Cliente...'
	gcc $(CFLAGS) $(SALIDA_CLIENTE) $(SOURCES_CLIENTE) $(HEADERS) $(LIBS) $(FFLAGS)

clean:
	@echo 'Eliminando ejecutables'
	rm -rf ./sources/*.o
	rm -rf ./*.o
