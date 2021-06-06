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
/**para barras de vida y mana*/
/**Con este numero de dimension, el arreglo tiene 22 cubos*/
/**Para que el numero de cubos sea parejo, el numero de dimension debe ser impar*/
#define DIMENSION 21

typedef struct
{
    int fuerza;
    int inteligencia;
    int destreza;
    int magia;
    int defensa;

} stAtributos ;

typedef struct
{
    int id;
    int dinero;
    int pocioneshp;
    int pocionesmp;
    char arma[20];
    char escudo[20];

} stInventario;

typedef struct
{
    int id ;
    char nombre[15];                             /**Nombre que elegió el jugador*/
    char genero[15];                             /**Elegido por el jugador**/
    char clase[15];                              /**guerrero, mago**/
    int nivel;                                   /**del personaje*/
    int nivelDeJuego;
    stAtributos atribPersonaje;                  /**estructura anidada,dentro tiene otra estructura, que tiene atributos como fuerza,destreza. Varia según la clase elegida*/
    stInventario inv;
    int hp;
    int mp;

} stPersonaje;

/**prototipados*/
void Tienda(stPersonaje player); /**funcion tienda recibe la estructura personaje*/
int CargarBarra(char a[],int DIM); /*funcion para cargar la barra de vida o mana*/
void MostrarBarra(char a[],int v); /*funcion para mostrar barra de vida o mana*/

int main()
{

    FILE * archi=NULL;
    char save []= {"save.dat"};

    if(fopen(save,"rb")==NULL)
    {
        archi=fopen(save,"wb");
        fclose(archi);
    }

    srand(time(NULL));



    inicioDePersonaje(save);

    mostrarPersonaje(save);

    /*prueba de función tienda*/
    stPersonaje aux;

    Tienda(aux);

    /**prueba de función Cargar barra y mostrar barra*/

    char vida[DIMENSION];
    char mana[DIMENSION];
    int validosV = CargarBarra(vida,DIMENSION);
    int validosM = CargarBarra(mana,DIMENSION);

    printf("Antes del golpe:\n\n");
    printf("HP\n");
    MostrarBarra(vida,validosV);
    printf("MP\n\n");
    MostrarBarra(mana,validosM);
    /**supongamos que la hp es de jugador, y recibe un golpe de 11*/
    printf("Despues del golpe:\n\n");
    /**se modifica validos*/
    validosV = validosV - 11;
    MostrarBarra(vida,validosV);


    return 0;
}

void inicioDePersonaje (char save [],int idjug)
{
    FILE * archi=fopen(save,"ab");

    if (archi!=NULL)
    {


        stPersonaje per;
        int tipo=0;

        printf("\nIngrese su nombre\n");
        gets(per.nombre);

        printf("Indica el genero con el cual te identificas\n");
        gets(per.genero);

        printf("Se le presentaran las siguientes clases las cual puede elegir\n");

        tipo=obtenerClase(per.clase);

        per.nivel=1;
        per.nivelDeJuego=0;
        per.id=idjug;

        switch (tipo)
        {
        case 1:
            atributosGuerrero(&per.atribPersonaje);
            strcpy(per.inv.arma,"Espada basica");
            strcpy(per.inv.escudo,"");
            break;
        case 2:
            atributosNigromante(&per.atribPersonaje);
            strcpy(per.inv.arma,"Baculo basico");
            strcpy(per.inv.escudo,"");
            break;
        case 3:
            atributosAsesino(&per.atribPersonaje);
            strcpy(per.inv.arma,"Daga basica");
            strcpy(per.inv.escudo,"");
            break;
        case 4:
            atributosHechicero(&per.atribPersonaje);
            strcpy(per.inv.arma,"Baculo basico");
            strcpy(per.inv.escudo,"");
            break;

        }


        per.hp=100;
        per.mp=100;
        per.inv.dinero=0;
        per.inv.pocioneshp=1;
        per.inv.pocionesmp=1;


        fwrite(&per,sizeof(stPersonaje),1,archi);
        fclose(archi);
    }
}


int obtenerClase (char clase[])
{
    int opc=0,tipo=0;


    printf("1. Guerrero: Clasico combatiente perfectamente ideado para el combate directo,\ncon sus propias capacidades de aguante que hacen trizas a los enemigos en un abrir y cerrar de ojos.\nSu arma afin serian las hachas, tanto de una como de dos manos, asi como los martillos.\n\n");
    printf("2. Nigromante: Los nigromantes usan el poder de la misma muerte para sacarle todo el provecho posible\ny causar estragos y temores a los enemigos, incluso utiliza los cadáveres ya sea como para curarse,\ncomo para invocar en su lugar caidos y siervos, no transformarse en un poderoso liche. Su arma afin es el baculo.\n\n",164);
    printf("3. Asesino: Expertos verdugos escurridizos que se tienen que exponer normalmente bastante,\npero con una cantidad de da%co brutal con habilidades capaces de deshacerse de los enemigos en un abrir y cerrar de ojos\nprovocando muertes por doquier. Su arma afin seria la daga y/o el cuchillo.\n\n");
    printf("4. Hechicero: Siempre causaran estragos elementales entre las filas enemigas.\nSus hechizos elementales van desde ataques devastadores a protectores e incluso curadores,\nsiempre dispuestos a entrar en pleno fragor de la batalla. El arma afin son los baculos.\n\n");

    printf("\nDentro de estas clases, %ccual crees que sera la mas adecuada para ti?\n",168);
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
int CargarBarra(char a[],int dim)
{
    int i = 0;

    for(i = 0; i < dim; i++)
    {
        a[i] = 219;
    }

    return i;
}
void MostrarBarra(char a[],int v)
{
    int i = 0;

    for(i = 0; i < v; i++)
    {
        printf("%c",a[i]);
    }
    printf("\n");
}

void mostrarPersonaje (char save [])
{
    FILE * archi=fopen(save,"rb");

    stPersonaje aux;

    fread(&aux,sizeof(stPersonaje),1,archi);

    printf("\n----------------------------------------------------------\n");
    printf("\nDatos del jugador");
    printf("\nNombre:..............:%s",aux.nombre);
    printf("\nGenero:..............:%s",aux.genero);
    printf("\nNivel:...............:%d",aux.nivel);
    printf("\nMana:................:%d",aux.mp);
    printf("\nVida:................:%d",aux.hp);
    printf("\nNivel Actual:........:%d",aux.nivelDeJuego);
    printf("\n----------------------------------------------------------");
    printf("\nAtributos del jugador");
    printf("\nFuerza:..............:[%d]",aux.atribPersonaje.fuerza);
    printf("\nMagia:...............:[%d]",aux.atribPersonaje.magia);
    printf("\nDefensa:.............:[%d]",aux.atribPersonaje.defensa);
    printf("\nDestreza:............:[%d]",aux.atribPersonaje.destreza);
    printf("\nInteligencia:........:[%d]",aux.atribPersonaje.inteligencia);
    printf("\n----------------------------------------------------------");
    printf("\nInventario del jugador");
    printf("\nArma:................:[%s]",aux.inv.arma);
    printf("\nDinero:..............:[%d]",aux.inv.dinero);
    printf("\nPociones de vida:....:[%d]",aux.inv.pocioneshp);
    printf("\nPociones de mana:....:[%d]",aux.inv.pocionesmp);
    printf("\nEscudo:..............:[%s]",aux.inv.escudo);
    printf("\n----------------------------------------------------------\n");

    fclose(archi);
}

void nuevaPartida (char archivo [])
{
    ///A modo de prueba para asignarle un id

    FILE * archi=fopen(archivo,"rb");
    stPersonaje aux;

    if (archi!=NULL)
    {

        fread(&aux,sizeof(stPersonaje),1,archi);













    }








}


void crearPersonajeNuevo ()
{
    printf("%cDesea crear un personaje nuevo?",168)









}

void funcionDeCombatesPorNivel ()
{
    stPersonaje aux;


    do
    {



    switch (aux.nivelDeJuego)
    {
    case 1:
        ///Nivel 1, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;
    case 2:
        ///Nivel 2, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;
    case 3:
        ///Nivel 3, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 4:
        ///Nivel 4, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 5:
        ///Nivel 5, batalla con primer jefe
        ///funcion de primer jefe
        break;

    case 6:
        ///Nivel 6, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 7:
        ///Nivel 7, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 8:
        ///Nivel 8, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 9:
        ///Nivel 9, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 10:
        ///Nivel 10, batalla con segundo jefe
        ///funcion de segundo jefe
        break;

    case 11:
        ///Nivel 11, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 12:
        ///Nivel 12, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 13:
        ///Nivel 13, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 14:
        ///Nivel 14, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 15:
        ///Nivel 15, batalla con tercer jefe
        ///funcion de tercer jefe
        break;

    case 16:
        ///Nivel 16, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 17:
        ///Nivel 17, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 18:
        ///Nivel 18, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 19:
        ///Nivel 19, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 20:
        ///Nivel 20, batalla con cuarto jefe
        ///funcion de cuarto jefe
        break;

    case 21:
        ///Nivel 21, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 22:
        ///Nivel 22, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 23:
        ///Nivel 23, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 24:
        ///Nivel 24, batalla con moustruo basico
        ///funcion de mounstruo basico
        break;

    case 25:
        ///Nivel 25, batalla con jefe final
        ///funcion de jefe final
        break;








    }while((continuar=='s')&&(aux.nivel<25)&&(aux.hp>0));








    }








}



void nivel1 ()
{

    int hpMonstruo=100;



    do
    {






    }while ()






}
