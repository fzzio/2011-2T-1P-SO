/*			PROYECTO DE SISTEMAS OPERATIVOS
 *			    I Parcial - 2011 2T
 * 
 * INTEGRANTES:
 * 	- Andrea Obando N.
 * 	- Fabricio Orrala P.
 * -----------------------------------------------------------------------
 * ARCHIVO:
 *      cliente.h
 * 
 * DESCRIPCION:
 * 	Este archivo contiene los encabezados de las funciones y las
 * 	estructuras de CLIENTE
 * 
 * OBSERVACIONES:
 * 	
 *		
 */

#ifndef CLIENTE_H
#define CLIENTE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct TCliente {
	int cta_num;
	float saldo;
} Cliente;

Cliente *Cliente_new();

//	Getters y Setters
void Cliente_setNumCta(Cliente *cli, int numerocta);
void Cliente_setSaldo(Cliente *cli, float monto);
int Cliente_getNumCta(Cliente *cli);
float Cliente_getSaldo(Cliente *cli);

void Cliente_SumResSaldo(Cliente *cli, float monto);
void Cliente_imprimir(Cliente *cli);
int Cliente_esValido(int ctaNum); //verificar despues con bool
void Cliente_Eliminar(Cliente **C);

#endif
