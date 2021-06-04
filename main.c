#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
/**para tienda*/
#define PRECIOHP 20
#define PRECIOMP 20
#define PRECIOESPADA 50
#define PRECIOBASTON 50
#define PRECIOESCUDO 50
#define PRECIOCALAVERA 50
#define PRECIOHACHA 50
#define PRECIODAGA 50

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

/**prototipados*/
void Tienda(stPersonaje player); /**funcion tienda recibe la estructura personaje*/

int main()
{

    FILE * archi=NULL;
    char save []={"save.dat"};

    srand(time(NULL));

    inicioDePersonaje(save);
    
    /*prueba de función tienda*/
    stPersonaje aux;
    
    Tienda(aux);

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
void Tienda(stPersonaje player)
{
    /**tomo los campos de dinero,pocionhp y  pocion mp y los igualo a variables LOCALES para
    trabajar más cómodo*/
    int dinero = player.inv.dinero;
    dinero = 1000; /**para la prueba*/
    int pocioneshp = player.inv.pocioneshp; /**en inventario*/
    pocioneshp = 0; /**para prueba*/
    int pocionesmp = player.inv.pocionesmp; /**en inventario*/
    pocionesmp = 0; /**para prueba*/
    int seleccion = 0; /**selecciona el case del menu compra, esto es qué comprar*/
    char control = 0; /**controla la repetición del menu compra, permite seguir o salir del mismo*/
    int cantidad = 0; /**cantidad de objetos a comprar, se multiplica por la constante precio*/
    int compra = 0; /**precio final de la compra, debe ser menor a dinero del jugador*/
    int flag = 1; /**controla el bucle del menu objeto*/
    char decision = 0; /**controla la decision de compra*/
    char op = 0; /**en caso de no haber dinero, permite volver al menu anterior o salir del menu objeto*/
    
    /**la siguiente serie de variables, sirven para determinar la cantidad de objetos que hay disponibles*/
    /**todo armamento se puede comprar solo una vez,excepto la daga*/
    /**el maximo de pociones de hp y mp son 10*/
    int hayespada = 1;
    int hayhacha = 1;
    int hayescudo = 1;
    int haycalavera = 1;
    int haybaston = 1;
    int haydaga = 2;
    int haypocionhp = 10;
    int haypocionmp = 10;

    do
    {
        printf("\nBienvenido/a a la tienda! Que desea??\n");

        printf("\nDINERO: %i\n",dinero);
        printf("\nPOCIONES HP en inventario: %i\n",pocioneshp);
        printf("\nPOCIONES MP en inventario: %i\n\n",pocionesmp);

        printf("\tOBJETO\t\tCANTIDAD\n\n");
        printf("1:\tPocion HP\t%i\n2:\tPocion MP\t%i\n3:\tEspada\t\t%i\n4:\tBaston\t\t%i\n5:\tCalavera\t%i\n6:\tEscudo\t\t%i\n7:\tHacha\t\t%i\n8:\tDaga\t\t%i\n9:\tSalir\n\n",haypocionhp,haypocionmp,hayespada,haybaston,haycalavera,hayescudo,hayhacha,haydaga);

        scanf("%i",&seleccion);

        /**esto es menu compra(general)*/
        switch(seleccion)
        {
        case 1:
            /**esto es un menu objeto, en este caso 1, pociones vida*/
            if(haypocionhp > 0)
            {
                while(flag == 1)
                {
                    do
                    {
                        do
                        {
                            printf("Cuantas pociones de hp desea?\n");
                            scanf("%i",&cantidad);
                            if( cantidad < 1 || cantidad > 10)
                            {
                                printf("No puedes comprar esa cantidad!\n");
                            }
                        }
                        while( cantidad < 1 || cantidad > 10);
                        compra = cantidad * PRECIOHP;
                        printf("El total es: %i\n",compra);
                        printf("Esta seguro? s/n");
                        scanf("\n%c",&decision);
                        if(decision=='s')
                        {

                            if(compra <= dinero)
                            {
                                printf("Gracias por su compra!\n");
                                dinero = dinero - compra;
                                pocioneshp = pocioneshp + cantidad;
                                haypocionhp = haypocionhp - cantidad;
                                flag = 0;
                            }
                            else
                            {
                                printf("No tienes suficiente dinero!\n");
                                printf("Deseas volver? s/n\n");
                                scanf("\n%c",&op);
                                if(op=='s')
                                {
                                    decision == 's';
                                }
                                else
                                {
                                    flag = 0;
                                }
                            }
                        }
                        else
                        {
                            flag = 0;
                        }
                    }
                    while(tolower(decision)=='s' && flag!= 0);
                }
                /**todas las variables usadas vuelven a inicializarse para usarse de nuevo*/
                seleccion = 0;
                control = 0;
                cantidad = 0;
                compra = 0;
                flag = 1;
                decision = 0;
                op = 0;
            }
            else
            {
                printf("NO HAY MAS POCIONES DE HP.\n");
            }
            break;
        case 2:
            /**menu objeto 2 pociones mana*/
            if(haypocionmp > 0)
            {
                while(flag == 1)
                {
                    do
                    {
                        do
                        {
                            printf("Cuantas pociones de mp desea?\n");
                            scanf("%i",&cantidad);
                            if(cantidad < 1 || cantidad > 10)
                            {
                                printf("No puedes comprar esa cantidad!!\n");
                            }
                        }
                        while(cantidad < 1 || cantidad > 10);
                        compra = cantidad * PRECIOMP;
                        printf("El total es: %i\n",compra);
                        printf("Esta seguro? s/n");
                        scanf("\n%c",&decision);
                        if(decision=='s')
                        {

                            if(compra <= dinero)
                            {
                                printf("Gracias por su compra!\n");
                                dinero = dinero - compra;
                                pocionesmp = pocionesmp + cantidad;
                                haypocionmp = haypocionmp - cantidad;
                                flag = 0;
                            }
                            else
                            {
                                printf("No tienes suficiente dinero!\n");
                                printf("Deseas volver? s/n\n");
                                scanf("\n%c",&op);
                                if(op=='s')
                                {
                                    decision == 's';
                                }
                                else
                                {
                                    flag = 0;
                                }
                            }
                        }
                        else
                        {
                            flag = 0;
                        }
                    }
                    while(tolower(decision)=='s' && flag!= 0);
                }
                seleccion = 0;
                control = 0;
                cantidad = 0;
                compra = 0;
                flag = 1;
                decision = 0;
                op = 0;
            }
            else
            {
                printf("NO HAY MAS POCIONES DE MP.\n");
            }
            break;
        case 3:
            if(hayespada == 1)
            {
                printf("El precio de la espada es: %i\n",PRECIOESPADA);
                printf("Desea comprar la espada?\n");
                scanf("\n%c",&control);
                if(control == 's')
                {
                    compra = PRECIOESPADA;
                    if( dinero >= compra)
                    {
                        /**la espada sube 5 de fuerza*/
                        player.atribPersonaje.fuerza = player.atribPersonaje.fuerza + 5;
                        /** y ademas sube 3 de destreza*/
                        player.atribPersonaje.destreza = player.atribPersonaje.destreza + 3;
                        /**le cobramos*/
                        dinero = dinero - compra;
                        hayespada = 0;
                        printf("Gracias por su compra!\n");
                    }
                    else
                    {
                        printf("No tienes suficiente dinero!! =( \n");
                    }
                }
                seleccion = 0;
                control = 0;
                compra = 0;
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 ESPADA GUERRERO!\n");
            }
            break;
        case 4:
            if(haybaston = 1)
            {
                printf("El precio del baston es: %i\n",PRECIOBASTON);
                printf("Desea comprar el baston?\n");
                scanf("\n%c",&control);
                if(control == 's')
                {
                    compra = PRECIOBASTON;
                    if( dinero >= compra)
                    {
                        /**el baston sube 5 de magia*/
                        player.atribPersonaje.magia = player.atribPersonaje.magia + 5;
                        /** y ademas sube 3 de inteligencia*/
                        player.atribPersonaje.inteligencia = player.atribPersonaje.inteligencia + 3;
                        /**le cobramos*/
                        dinero = dinero - compra;
                        haybaston = 0;
                        printf("Gracias por su compra!\n");
                    }
                    else
                    {
                        printf("No tienes suficiente dinero!! =( \n");
                    }
                }
                seleccion = 0;
                control = 0;
                compra = 0;
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 BASTON MAGO!\n");
            }
            break;
        case 5:
            if(haycalavera = 1)
            {
                printf("El precio de la calavera es: %i\n",PRECIOCALAVERA);
                printf("Desea comprar la calavera?\n");
                scanf("\n%c",&control);
                if(control == 's')
                {
                    compra = PRECIOCALAVERA;
                    if( dinero >= compra)
                    {
                        /**la calavera sube 5 de defensa*/
                        player.atribPersonaje.defensa = player.atribPersonaje.defensa + 5;
                        /** y ademas sube 3 de magia*/
                        player.atribPersonaje.magia = player.atribPersonaje.magia + 3;
                        /**le cobramos*/
                        dinero = dinero - compra;
                        haycalavera = 0;
                        printf("Gracias por su compra!\n");
                    }
                    else
                    {
                        printf("No tienes suficiente dinero!! =( \n");
                    }
                }
                seleccion = 0;
                control = 0;
                compra = 0;
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 CALAVERA NIGROMANTE!\n");
            }
            break;
        case 6:
            if(hayescudo = 1)
            {
                printf("El precio del escudo es: %i\n",PRECIOESCUDO);
                printf("Desea comprar el escudo?\n");
                scanf("\n%c",&control);
                if(control == 's')
                {
                    compra = PRECIOESCUDO;
                    if( dinero >= compra)
                    {
                        /**el escudo sube 5 de defensa*/
                        player.atribPersonaje.defensa = player.atribPersonaje.defensa + 5;
                        /** y ademas sube 3 de fuerza*/
                        player.atribPersonaje.defensa = player.atribPersonaje.defensa + 3;
                        /**le cobramos*/
                        dinero = dinero - compra;
                        hayescudo = 0;
                        printf("Gracias por su compra!\n");
                    }
                    else
                    {
                        printf("No tienes suficiente dinero!! =( \n");
                    }
                }
                seleccion = 0;
                control = 0;
                compra = 0;
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 ESCUDO GUERRERO!\n");
            }
            break;
        case 7:
            if(hayhacha = 1)
            {
                printf("El precio del hacha es:%i\n",PRECIOHACHA);
                printf("Desea comprar el hacha?\n");
                scanf("\n%c",&control);
                if(control == 's')
                {
                    compra = PRECIOHACHA;
                    if( dinero >= compra)
                    {
                        /**el hacha sube 5 de fuerza*/
                        player.atribPersonaje.fuerza = player.atribPersonaje.fuerza + 5;
                        /** y ademas sube 3 de destreza*/
                        player.atribPersonaje.destreza = player.atribPersonaje.destreza + 3;
                        /**le cobramos*/
                        dinero = dinero - compra;
                        hayhacha = 0;
                        printf("Gracias por su compra!\n");
                    }
                    else
                    {
                        printf("No tienes suficiente dinero!! =( \n");
                    }
                }
                seleccion = 0;
                control = 0;
                compra = 0;
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 HACHA GUERRERO!\n");
            }
            break;
        case 8:
            if(haydaga > 0 )
            {
                printf("El precio de la daga es: %i\n",PRECIODAGA);
                printf("Desea comprar la daga?\n");
                scanf("\n%c",&control);
                if(control == 's')
                {
                    compra = PRECIODAGA;
                    if( dinero >= compra)
                    {
                        /**la daga sube 3 de fuerza*/
                        player.atribPersonaje.fuerza = player.atribPersonaje.fuerza + 3;
                        /** y ademas sube 3 de destreza*/
                        player.atribPersonaje.destreza = player.atribPersonaje.destreza + 3;
                        /**le cobramos*/
                        dinero = dinero - compra;
                        haydaga = haydaga - 1;
                        printf("Gracias por su compra!\n");
                    }
                    else
                    {
                        printf("No tienes suficiente dinero!! =( \n");
                    }
                }
                seleccion = 0;
                control = 0;
                compra = 0;
            }
            else
            {
                printf("NO HAY MAS DAGAS PARA TI PICARON.\n");
            }
            break;
        case 9:
            /**salir del menu*/
            control = 'n';
            break;
        }
        if(seleccion!= 9)
        {
            printf("Desea comprar algo mas? s/n\n");
            scanf("\n%c",&control);
            system("cls");
        }
        else
        {
            control = 'n';
            system("cls");
        }
    }
    while(tolower(control)=='s');
}








    return 0;
}
