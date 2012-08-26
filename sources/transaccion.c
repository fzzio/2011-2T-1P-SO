/*			PROYECTO DE SISTEMAS OPERATIVOS
 *			    I Parcial - 2011 2T
 * 
 * INTEGRANTES:
 * 	- Andrea Obando N.
 * 	- Fabricio Orrala P.
 * -----------------------------------------------------------------------
 * ARCHIVO:
 *      transaccion.c
 * 
 * DESCRIPCION:
 * 	Este archivo contiene las implementacion de las funciones de
 *	manejo de TRANSACCION
 * 
 * OBSERVACIONES:
 * 	
 *		
 */

#include <transaccion.h> 

Transaccion *Transaccion_new() {
	Transaccion *tr;
	tr = (Transaccion*) malloc(sizeof(Transaccion));

	Transaccion_setCtaCliente(tr, 0);
	Transaccion_setSigno(tr, '+');
	Transaccion_setMonto(tr, 0);
	Transaccion_setReferencia(tr, 0);

	return (tr);
}

//	Getters y Setters
void Transaccion_setCtaCliente(Transaccion *tr, int num_cta_cliente) {
	tr->cta_cliente = num_cta_cliente;
}

void Transaccion_setSigno(Transaccion *tr, char signo) {
	tr->signo = signo;
}

void Transaccion_setMonto(Transaccion *tr, float monto) {
	tr->monto = monto;
}

void Transaccion_setReferencia(Transaccion *tr, int num_cta_referencia) {
	tr->cta_referencia = num_cta_referencia;
}

int Transaccion_getCtaCliente(Transaccion *tr) {
	return (tr->cta_cliente);
}

char Transaccion_getSigno(Transaccion *tr) {
	return (tr->signo);
}
float Transaccion_getMonto(Transaccion *tr) {
	return (tr->monto);
}

int Transaccion_getCtaReferencia(Transaccion *tr) {
	return (tr->cta_referencia);
}

void Transaccion_imprimir(Transaccion *tr) {
	printf("TR %d %c %f %d\n", Transaccion_getCtaCliente(tr),
			Transaccion_getSigno(tr), Transaccion_getMonto(tr),
			Transaccion_getCtaReferencia(tr));
}

void Transaccion_Eliminar(Transaccion **T) {
	//Revisar
	if(T!=NULL){
		free(*T);
		*T = NULL;
	}
}

