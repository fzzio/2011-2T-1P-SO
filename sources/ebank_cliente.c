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


#include <transaccion.h>
#include <cliente.h>

#define PORT 3512
#define MAXDATASIZE 250

void imprimirHeader();
void imprimirMenu(Cliente *cliente);
Cliente *esValida(char *cuenta);
void imprimirHeader2(Cliente *cliente);
void imprimirResumen(int ctaOrigen, int ctaReferencia, float montoTransaccion);
void imprimirFin();

//funcion de prueba, temporal
Cliente *convertirACliente(char *cadena);

int fd_socket_cliente, fd_comunicacion;
char ctaIngresadaA[15], monto[10], ctaIngresadaB[15];
int inf_tam_byte; //Tamanio de lo que se recibe en las transferencias
int guardarTransaccion(int tipoTransaccion, Cliente *clienteOrigen, Cliente *clienteDestino, float monto);

int main(int argc, char *argv[]) {
	//int valCtaIngresada;
	float monto;
	int clienteNo;
	int op;
	struct sockaddr_in dir_servidor;
	Cliente *cliente;
	Cliente *cliente2;
	Transaccion *transaccion;
	do{
		/* Creamos el socket */
		if ((fd_socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) == -1){
			perror("Error de Coneccion");
			exit(1);
		}
		printf("\nSocket creado...\n");
		/* Establecemos direccion_servidor con la direccion del server*/

		dir_servidor.sin_family = AF_INET;
		dir_servidor.sin_port = htons(PORT);
		dir_servidor.sin_addr.s_addr = INADDR_ANY;
		bzero(&(dir_servidor.sin_zero), 8);

		/* Intentamos conectarnos con el servidor */
		if (connect(fd_socket_cliente, (struct sockaddr *)&dir_servidor,sizeof(struct sockaddr)) == -1) {
			perror("connect");
			exit(1);
		}
		printf("\nConexion establecida...\n");

		ctaIngresadaA[0] = '\0';
		monto = 0.0;
		clienteNo = -1;
		op = -1;
		cliente = NULL;
		cliente2 = NULL;
		transaccion = NULL;
		//Cliente_Eliminar(cliente);
		//Cliente_Eliminar(cliente2);
		//Transaccion_Eliminar(transaccion);

		imprimirHeader();
		printf("\tIngrese su numero de cuenta: ");
		gets(ctaIngresadaA);
		cliente = esValida(ctaIngresadaA);

		if(cliente == NULL){
			printf("No existe el cliente, se aniadio a la base de datos.");
		}
		
		sleep(1);

		//Cliente ingresado es correcto o nuevo
		printf("Cliente correcto \n");
		op = -1;
		if(cliente!=NULL){
			do{
				imprimirMenu(cliente);
				scanf("%d", &op);
				fflush(stdin);
			}while(op<1 || op>3);

			switch(op){
				case 1: imprimirHeader();
						imprimirHeader2(cliente);
						printf("\t >>> Deposito <<<\n");
						printf("*************************************************************************\n");
						printf("\tIngrese monto a DEPOSITAR: ");
						scanf("%f", &monto);
						imprimirResumen(Cliente_getNumCta(cliente), Cliente_getNumCta(cliente), monto);
						guardarTransaccion(1, cliente, cliente, monto);
						break;
				case 2: imprimirHeader();
						imprimirHeader2(cliente);
						printf("\t >>> Retiro <<<\n");
						printf("*************************************************************************\n");
						printf("\tIngrese monto a RETIRAR: ");
						scanf("%f", &monto);
						imprimirResumen(Cliente_getNumCta(cliente), Cliente_getNumCta(cliente), monto);
						guardarTransaccion(2, cliente, cliente, monto);
						break;
				case 3: imprimirHeader();
						imprimirHeader2(cliente);
						printf("\t >>> Transferencia <<<\n");
						printf("*************************************************************************\n");
						do{
							printf("\tIngrese No. Cuenta del destinatario: ");
							gets(ctaIngresadaB);
							cliente2 = esValida(ctaIngresadaB);
						}while(cliente2==NULL);
						printf("\tIngrese monto a TRANSFERIR: ");
						scanf("%f", &monto);
						imprimirResumen(Cliente_getNumCta(cliente), Cliente_getNumCta(cliente2), monto);
						guardarTransaccion(3, cliente, cliente, monto);
						break;
				default:
						printf("\t Opcion Incorrecta\n");
						break;
			}
		}

		imprimirFin();
		close(fd_socket_cliente);
	}while(1);

	return(0);
}


Cliente *esValida(char *cuenta){
	char str[250];
	int cta = 0;

	if (send(fd_socket_cliente, cuenta, strlen(cuenta)+1, 0) == -1){
		//printf("error feo\n");
		perror("send");
		exit(1);
	}

	//esperamos respuesta
	sleep(1);

	inf_tam_byte=recv(fd_socket_cliente, str, MAXDATASIZE, 0);
	if (inf_tam_byte <= -1 || inf_tam_byte == 0){
		str[0] = '\0';
		perror("recv");
		exit(1);
	}else{
		//si llega aqui es porque si habia datos en la trama recibida
		str[inf_tam_byte] = '\0';
		//fprintf(stderr, "error misterioso %s\n", str);
		return(convertirACliente(str));
	}
}

void imprimirHeader(){
	system("clear");
	printf("\n*************************************************************************\n");
	printf("*\t\t\t\tE - bank\t\t\t\t*\n");
	printf("*************************************************************************\n");
}

void imprimirHeader2(Cliente *cliente){
	printf("\tBienvenido!\n");
	printf("\tCta. No: %d\tCliente Saldo: %.2f\tFecha: 06/12/2011\n", Cliente_getNumCta(cliente), Cliente_getSaldo(cliente));
	printf("*************************************************************************\n");
}

void imprimirMenu(Cliente *cliente){
	imprimirHeader();
	imprimirHeader2(cliente);
	printf("\tTransacciones disponibles:\n\t1. Deposito\n\t2. Retiro\n\t3. Transferencia\n");
	printf("\n\tSeleccione opcion: ");
}

Cliente *convertirACliente(char *cadena){
	int nCuentaNo, result;
	float nSaldo;
	char sCuentaNo[10];
	char sSaldo[10];

	//recibo 1011 26.00

	if(sscanf(cadena, "%s %s", sCuentaNo, sSaldo)<0){
		return NULL;
	}

	nCuentaNo = atoi(sCuentaNo);
	nSaldo = atof(sSaldo);

	Cliente *C;
	C = Cliente_new();
	Cliente_setNumCta(C, nCuentaNo);
	Cliente_setSaldo(C, nSaldo);

	return (C);
}

void imprimirResumen(int ctaOrigen, int ctaReferencia, float montoTransaccion){
	printf("\n\t* Resumen de la Transaccion *\n");
	printf("\tCuenta Origen: %d\n", ctaOrigen);
	printf("\tCuenta Referencia: %d\n", ctaReferencia);
	printf("\tMonto Transaccion: %.2f\n", montoTransaccion);
}

void imprimirFin(){
	imprimirHeader();
	printf("\n\t>>> GRACIAS POR UTILIZAR NUESTRO SERVICIO <<<\n");
	//getchar();
}

int guardarTransaccion(int tipoTransaccion, Cliente *clienteOrigen, Cliente *clienteDestino, float monto){
	char tramaTotal[250];

	//19440880 + 20.30 19440880

	switch(tipoTransaccion){
		case 1: //Deposito
				sprintf(tramaTotal, "%d + %.2f %d", Cliente_getNumCta(clienteOrigen), monto, Cliente_getNumCta(clienteDestino));
				break;
		case 2: //Retiro
				sprintf(tramaTotal, "%d - %.2f %d", Cliente_getNumCta(clienteOrigen), monto, Cliente_getNumCta(clienteDestino));
				break;
		case 3: //Transferencia
				sprintf(tramaTotal, "%d + %.2f %d", Cliente_getNumCta(clienteOrigen), monto, Cliente_getNumCta(clienteDestino));
				break;

	}

	//strcat(tramaTotal, '\0');

	fprintf(stderr, "El misterio de la trama total %s\n",tramaTotal);

	if (send(fd_socket_cliente, tramaTotal, strlen(tramaTotal), 0) == -1){
		fprintf(stderr, "error guardarTransaccion\n");
		perror("send");
		//exit(1);
		return (-1);
	}
	//esperamos respuesta
	sleep(1);
	fflush(stdin);
	return(1);
}
