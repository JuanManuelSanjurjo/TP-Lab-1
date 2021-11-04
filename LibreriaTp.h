#ifndef LIBRERIATP_H_INCLUDED
#define LIBRERIATP_H_INCLUDED
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "gotoxy.h"



typedef struct
{
    int fuerza;
    int inteligencia;
    int destreza;
    int magia;
    int defensa;
} stAtributos ;                                                 ///Estructura N1 la cual se usa en stPersonaje, define los atributos del personaje el cual iran cambiando

typedef struct
{
    int cantidad;
    char nombre[20];
    char tipoObjeto[20];

} stInventario;                                                 ///Estructura N2, Es el inventario del personaje, la cual tambien ira modificandose en el correr del juego


typedef struct
{
    ///int idObjeto;
    char tipoDeObjeto [20];

    struct nodoInventario * listaInventario;

} celdaInventario;


typedef struct
{
    char nombre[15];                                            /**Nombre que elegió el jugador*/
    char genero[15];                                            /**Elegido por el jugador*/
    char tipoClase[15];                                         /**guerrero, hechicero, nigromante o asesino*/
    int clase;                                                  /**Entero el cual se asigna el numero de clase para el manejo de ataque y defensa*/
    int nivelDeJuego;                                           /**nivel en que se quedó el jugador*/
    stAtributos atribPersonaje;                                 /**estructura anidada,dentro tiene otra estructura,que tiene atributos como fuerza,destreza. Varia según la clase elegida*/
    celdaInventario inv [10];                                   /**Estructura inventario*/
    int hp;                                                     /**Vida del personaje*/
    int mp;                                                     /**Mana del personaje*/
    int tiempo;                                                 /**Tiempo jugado*/

} stPersonaje;                                                  /**Estructura de personaje con la cual se juega*/


typedef struct
{
    char nombre[15];
    char clase[15];
    int nivelMaximo;
    int tiempoJuego;

} stMarcador;

typedef struct _nodo
{
    stInventario dato;
    struct _nodo *siguiente;

} nodoInventario;


typedef struct
{
    char nombreDeUsuario [50];

    char password [20];

    stPersonaje personaje;

    stMarcador marcador;


} usuario;

typedef struct
{
    usuario dato;

    struct nodoArbolUsuarios * izq;
    struct nodoArbolUsuarios * der;

} nodoArbolUsuarios;

#endif // LIBRERIATP_H_INCLUDED
