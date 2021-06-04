#include <stdio.h>
#include <stdlib.h>

int main()
{

    #include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct
{
    int fuerza;
    int inteligencia;
    int destreza;
    int magia;
    int defensa;

}stAtributos ;

typedef struct
{
    int dinero;
    int pocioneshp;
    int pocionesmp;
    char arma[20];
    char escudo[20];

}stInventario;

typedef struct
{
    char nombre[15];                             /**Nombre que elegió el jugador*/
    char genero[15];                             /**Elegido por el jugador**/
    char clase[15];                              /**guerrero, mago**/
    int nivel;                                   /**del personaje*/
    stAtributos atribPersonaje;                  /**estructura anidada,dentro tiene otra estructura, que tiene atributos como fuerza,destreza. Varia según la clase elegida*/
    stInventario inv;
    int hp;
    int mp;

}stPersonaje;












int main()
{

    FILE * archi=NULL;
    char save []={"save.dat"};

    srand(time(NULL));

    inicioDePersonaje(save);


    return 0;
}

void inicioDePersonaje (char save [])
{
    FILE * archi=fopen(save,"wb");

    stPersonaje per;
    int tipo=0;

    printf("\nIngrese su nombre\n");
    gets(per.nombre);

    printf("Indica el genero con el cual te identificas\n");
    gets(per.nombre);

    printf("Se le presentaran las siguientes clases las cual puede elegir\n");

    tipo=obtenerClase(per.clase);

    per.nivel=1;

    switch (tipo)
    {
    case 1:
        atributosGuerrero(&per.atribPersonaje);
        strcpy(per.inv.arma,"Espada basica");
        break;
    case 2:
        atributosNigromante(&per.atribPersonaje);
        strcpy(per.inv.arma,"Baculo basico");
        break;
    case 3:
        atributosAsesino(&per.atribPersonaje);
        strcpy(per.inv.arma,"Daga basica");
        break;
    case 4:
        atributosHechicero(&per.atribPersonaje);
        strcpy(per.inv.arma,"Baculo basico");
        break;

    }

    per.inv.dinero=0;
    per.inv.pocioneshp=1;
    per.inv.pocionesmp=1;

}


int obtenerClase (char clase[])
{
    int opc=0,tipo=0;


    printf("1. Guerrero: Clásico combatiente perfectamente ideado para el combate directo,\n con sus propias capacidades de aguante que hacen trizas a los enemigos en un abrir y cerrar de ojos.\nSu arma afín serían las hachas, tanto de una como de dos manos, así como los martillos.\n ");
    printf("2. Nigromante: Los nigromantes usan el poder de la misma muerte para sacarle todo el provecho posible y causar estragos y temores a los enemigos,\nincluso utiliza los cadáveres ya sea como para curarse, como para invocar en su lugar caídos y siervos,\no transformarse en un poderoso liche. Su arma afín es el baculo. ");
    printf("3. Asesino: Expertos verdugos escurridizos que se tienen que exponer normalmente bastante,\npero con una cantidad de daño brutal con habilidades capaces de deshacerse de los enemigos en un pispas\nprovocando muertes por doquier. Su arma afín sería la daga y/o el cuchillo.\n");
    printf("4. Hechicero: Siempre causarán estragos elementales entre las filas enemigas.\nSus hechizos elementales van desde ataques devastadores a protectores e incluso curadores,\nsiempre dispuestos a entrar en pleno fragor de la batalla. El arma afín son los báculos.\n");

    printf("\nDentro de estas clases, ¿cual crees que sera la mas adecuada para ti?\n");
    scanf("%d",&opc);

    while ((opc>4)&&(opc<1))
    {
        printf("La opcion es incorrecta, ingrese un dato correcto\n");
        scanf("%d",&opc);
    }

    printf("Ya lo veremos\n");

    switch (opc)
    {
    case 1:
        strcpy(clase,"Guerrero");
        break;
    case 2:
        strcpy(clase,"Nigromante");
        break;
    case 3:
       strcpy(clase,"Asesino");
        break;
    case 4:
        strcpy(clase,"Hechicero");
        break;

    }


    return opc;

}

void atributosGuerrero (stAtributos * aux)
{
    aux->destreza=rand()%5+11;
    aux->fuerza=rand()%10+11;
    aux->inteligencia=rand()%5+6;
    aux->magia=rand()%5+1;
    aux->defensa=rand()%10+11;

    return aux;
}

void atributosNigromante(stAtributos * aux)
{
    aux->destreza=rand()%5+1;
    aux->fuerza=rand()%5+6;
    aux->inteligencia=rand()%10+11;
    aux->magia=rand()%10+11;
    aux->defensa=rand()%5+6;

    return aux;
}

void atributosAsesino (stAtributos * aux)
{
    aux->destreza=rand()%10+15;
    aux->fuerza=rand()%10+6;
    aux->inteligencia=rand()%10+11;
    aux->magia=rand()%5+6;
    aux->defensa=rand()%5+10;

    return aux;
}

void atributosHechicero (stAtributos * aux)
{
    aux->destreza=rand()%5+1;
    aux->fuerza=rand()%5+3;
    aux->inteligencia=rand()%10+11;
    aux->magia=rand()%10+11;
    aux->defensa=rand()%5+6;

    return aux;
}







    return 0;
}
