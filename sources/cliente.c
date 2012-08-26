/*			PROYECTO DE SISTEMAS OPERATIVOS
 *			    I Parcial - 2011 2T
 * 
 * INTEGRANTES:
 * 	- Andrea Obando N.
 * 	- Fabricio Orrala P.
 * -----------------------------------------------------------------------
 * ARCHIVO:
 *      cliente.c
 * 
 * DESCRIPCION:
 * 	Este archivo contiene las implementacion de las funciones de
 *	manejo de CLIENTE
 * 
 * OBSERVACIONES:
 * 	
 *		
 */

#include <cliente.h> 

Cliente *Cliente_new() {
	Cliente *cli;
	cli = (Cliente*) malloc(sizeof(Cliente));
	Cliente_setNumCta(cli, 0);
	Cliente_setSaldo(cli, 0.0);
	return (cli);
}

void Cliente_setNumCta(Cliente *cli, int numerocta) {
	cli->cta_num = numerocta;
}
void Cliente_setSaldo(Cliente *cli, float monto) {
	cli->saldo = monto;
}

int Cliente_getNumCta(Cliente *cli) {
	return (cli->cta_num);
}

float Cliente_getSaldo(Cliente *cli) {
	return (cli->saldo);
}

void Cliente_SumResSaldo(Cliente *cli, float monto) {
	cli->saldo = cli->saldo + monto;
}

void Cliente_imprimir(Cliente *cli) {
	printf("Cliente #: %d ***\n", Cliente_getNumCta(cli));
}

int Cliente_esValido(int ctaNum) {
	//busqueda
	return (1);
}

void Cliente_Eliminar(Cliente **C) {
	//Revisar
	if (C!=NULL){
		free(*C);
		*C = NULL;
	}
}
