/*			PROYECTO DE SISTEMAS OPERATIVOS
 *			    I Parcial - 2011 2T
 * 
 * INTEGRANTES:
 * 	- Andrea Obando N.
 * 	- Fabricio Orrala P.
 * -----------------------------------------------------------------------
 * ARCHIVO:
 *      transaccion.h
 * 
 * DESCRIPCION:
 * 	Este archivo contiene los encabezados de las funciones y las
 * 	estructuras de TRANSACCION
 * 
 * OBSERVACIONES:
 * 	
 *		
 */

#ifndef TRANSACCION_H
#define TRANSACCION_H

#include <string.h>
#include <stdlib.h>

typedef struct TTransaccion {
	int cta_cliente;
	char signo; // + o -
	float monto;
	int cta_referencia;
} Transaccion;

Transaccion *Transaccion_new();

//	Getters y Setters
void Transaccion_setCtaCliente(Transaccion *tr, int num_cta_cliente);
void Transaccion_setSigno(Transaccion *tr, char signo);
void Transaccion_setMonto(Transaccion *tr, float monto);
void Transaccion_setReferencia(Transaccion *tr, int num_cta_referencia);

int Transaccion_getCtaCliente(Transaccion *tr);
char Transaccion_getSigno(Transaccion *tr);
float Transaccion_getMonto(Transaccion *tr);
int Transaccion_getCtaReferencia(Transaccion *tr);

void Transaccion_imprimir(Transaccion *tr);
void Transaccion_Eliminar(Transaccion **T);

#endif
