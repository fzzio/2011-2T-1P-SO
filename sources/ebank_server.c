#include <linux/unistd.h>
#include <linux/kernel.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3512
#define BACKLOG 2
#define TAMANIO_MAX 250

#define ARCHIVO_CUENTAS "./data/cuentas.DAT"
#define ARCHIVO_TRANSACCIONES "./data/transacciones.DAT"

char buff[TAMANIO_MAX];
int inf_tam_byte; //Tamanio de lo que se recibe en las transferencias

char recepNoCta[20]; //Donde almacenaremos el numero de cuenta que recibimos
char recepTransaccion[100];

char *verificarCtaBase(char *recepNoCta);
int guardarEnArchivo(char *nombreArchivo, char *string);
char *trama;
char *actualizar(char *cadena);

int main(int argc, char *argv[]) {
	system("clear");
	int fd_socket_servidor, fd_comunicacion;
	struct sockaddr_in dir_servidor;
	struct sockaddr_in dir_cliente;
	int sin_size;
	int clienteNum = 0;
	int contador;

	if ((fd_socket_servidor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Error en socket\n");
		exit(-1);
	}

	dir_servidor.sin_family = AF_INET;
	dir_servidor.sin_addr.s_addr = INADDR_ANY;
	dir_servidor.sin_port = htons(PORT);
	bzero(&(dir_servidor.sin_zero), 8);

	if (bind(fd_socket_servidor, (struct sockaddr*) &dir_servidor, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr, "Error en bind\n");
		exit(-1);
	}

	if (listen(fd_socket_servidor, BACKLOG) == -1) {
		fprintf(stderr, "Error en listen\n");
		exit(-1);
	}

	contador = 0;
	while (1) {
		trama = (char *)malloc(sizeof(char) *250);
		printf("Server esperando...\n");
		sin_size = sizeof(dir_cliente);
		
		if ((fd_comunicacion = accept(fd_socket_servidor, (struct sockaddr*) &dir_cliente, &sin_size)) == -1) {
			fprintf(stderr, "Error en accept\n");
			exit(-1);
		}
		
		// Empezamos el proceso del cliente que llega, para no tener un solo proceso para todos los clientes, creamos mas procesos.
		if (fork() == 0) {
			
			// 1. Server recibe es el numero de cuenta
			if ((inf_tam_byte = recv(fd_comunicacion, recepNoCta, TAMANIO_MAX, 0)) == -1) {
				fprintf(stderr, "Error en recibir\n");
				exit(1);
			}
			//Verificamos que exista y devolvemos una cadena con los datos de la cuenta.
			recepNoCta[inf_tam_byte] = '\0'; //Asignamos a la posicion final el fin de cadena '\0'
			printf("Cta Recibida: %s\n", recepNoCta);
			
			sleep(1);
			//de aqui se manda a imprimir o algo
			trama = verificarCtaBase(recepNoCta);
			
			if (trama == NULL) {
				printf("Dentro de trama=NULL: %s\n", contador);
				//cuenta incorrecta, se aniade
				//fprintf(stderr, "Cuenta incorrecta\n");
				//printf("Se aniadira a la base de datos\n");
				sprintf(trama, "%s + 10.00 %s",recepNoCta, recepNoCta);
				printf("Trama transacc %s\n", trama);
				guardarEnArchivo(ARCHIVO_TRANSACCIONES, trama);

				sprintf(trama, "%s 10.00",recepNoCta); //la trama se queda al final lista para enviarla
				guardarEnArchivo(ARCHIVO_CUENTAS, trama);
				printf("Trama transacc%s\n", trama);
			}

			if (send(fd_comunicacion, trama, strlen(trama)+1, 0) == -1) {
				perror("send");
				fprintf(stderr, "No se pudo enviar trama\n");
				//close(fd_comunicacion);
				exit(0);
			}
			
			sleep(1);

			if ((inf_tam_byte = recv(fd_comunicacion, recepTransaccion, TAMANIO_MAX, 0)) == -1) {
				perror("recv");
				exit(1);
			}
			
			recepTransaccion[inf_tam_byte] = '\0'; //Asignamos a la posicion final el fin de cadena '\0'
			printf("Transaccion Recibida: %s\n", recepTransaccion);

			sleep(1);
			if(strcmp(actualizar(recepTransaccion), "correcto")==0){
				printf("Datos actualizados");
			}
			guardarEnArchivo(ARCHIVO_TRANSACCIONES, recepTransaccion);

		} else {
			close(fd_comunicacion);
			/* Se suspende la ejecucion del proceso padre hasta que finalice el proceso hijo */
			while (waitpid(-1, NULL, WNOHANG) > 0);
		}
		
		//printf("Cliente %d: Desde %s \n", clienteNum, inet_ntoa(dir_cliente.sin_addr));
		//send(fd_comunicacion, "Bienvenido a mi servidor!!!\n", 30, 0);
		//close(fd_comunicacion);
		clienteNum++;

	}
	printf("Error en listen\n");
	exit(-1);
}

char *verificarCtaBase(char *recepNoCta) {
	char cuenta[100];
	char total[100];

	FILE *ofp; /* Declara apuntadores a archivo */

	if ((ofp = fopen(ARCHIVO_CUENTAS, "r")) == NULL) { /* Abre archivo de entrada */
		printf("Error en apertura del fichero para lectura \n ");
	} else {
		while (!feof(ofp)) {
			fscanf(ofp, "%s %s", cuenta, total); /* Lee datos de entrada */
			if (strcmp(cuenta, recepNoCta) == 0) {
				strcat(cuenta, " ");
				strcat(cuenta, total);
				fclose(ofp);
				fflush(stdin);
				return cuenta;
			}
		}
		fclose(ofp);
	}

	return NULL;
}

int guardarEnArchivo(char *nombreArchivo, char *string) {
	char guardaArchiv[100];

	if (sprintf(guardaArchiv, "cat>>%s<<EOF\n%s\nEOF", nombreArchivo, string)< 0) {
		return -1;
	}else {
		if (system(guardaArchiv) < 0) {
			return -1;
		} else {
			return 0;
		}
	}
}

char *actualizar(char *cadena){
   char cuentasDAT[] = ARCHIVO_CUENTAS;
   char cuentaorigen[100];
   char cuentadestino[100];
   char cuentad[100];
   char cuentao[100];
   char s[100];
   float totalo;
   float totald;
   char signo;
   float monto;
   sscanf(cadena,"%s %c %f %s",cuentaorigen,&signo,&monto,cuentadestino);
   FILE *ofp;

     if ((ofp = fopen(cuentasDAT,"rw+"))==NULL){    //Cuenta origen   
            printf ("Error en apertura del fichero para lectura \n ");
       }else{
        while (! feof(ofp)){
          fscanf(ofp,"%s %s",cuentao,s);    
          if (strcmp(cuentao,cuentaorigen)==0){
            fclose(ofp);
            totalo=atof(s);
            printf("%.2f\n",totalo);
            break;
          }
        }
   
        }
   if(strcmp(cuentaorigen,cuentadestino)==0){ // La transaccion es un deposito o retiro
       if ((ofp = fopen(cuentasDAT,"rw+"))==NULL){      
            printf ("Error en apertura del fichero para lectura \n ");
       }else{
        while (! feof(ofp)){
          fscanf(ofp,"%s",cuentao);    
          if (strcmp(cuentao,cuentaorigen)==0){
           
            if(signo=='+'){
               
                fprintf(ofp," %.2f\n",monto+totalo);
                fclose(ofp);
                return "correcto";
            }else{
                fprintf(ofp," %.2f",totalo-monto);
                fclose(ofp);
                return "correcto";
            }
          }
       }
    }
     fclose(ofp);
    return NULL;
   
    }
    else{ // La transaccion es una transferencia

    if ((ofp = fopen(cuentasDAT,"rw+"))==NULL){      
            printf ("Error en apertura del fichero para lectura \n ");
       }else{
        while (! feof(ofp)){
          fscanf(ofp,"%s %s",cuentad,s);    
          if (strcmp(cuentad,cuentadestino)==0){
            fclose(ofp);
                totald=atof(s);
            break;
          }
        }
   
        }

     if ((ofp = fopen(cuentasDAT,"rw+"))==NULL){      
            printf ("Error en apertura del fichero para lectura \n ");
       }else{
        while (! feof(ofp)){
          fscanf(ofp,"%s",cuentao);    
          if (strcmp(cuentao,cuentaorigen)==0){
            fprintf(ofp," %.2f",totalo-monto);
            fclose(ofp);
            break;   
          }
        }
      }
     if ((ofp = fopen(cuentasDAT,"rw+"))==NULL){      
            printf ("Error en apertura del fichero para lectura \n ");
       }else{
        while (! feof(ofp)){
          fscanf(ofp,"%s",cuentad);    
          if (strcmp(cuentad,cuentadestino)==0){
            fprintf(ofp," %.2f",totald+monto);
            fclose(ofp);
            return "correcto";   
          }
        }
      }
     fclose(ofp);
    return NULL;

    }
 
}
