#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
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
/**archivos*/
#define SAVE "save.dat"
#define MARCADORES "marcadores.dat"


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
    int idInventario;
    int dinero;
    int pocioneshp;
    int pocionesmp;
    char arma[20];
    char escudo[20];
} stInventario;

typedef struct
{
    int idMarcador;
    int idPersonaje; /**id para relacionar con stMarcadores*/
    char nombre[15]; /**Nombre que elegió el jugador*/
    char genero;  /**Elegido por el jugador**/
    char tipoClase[15]; /**guerrero, hechicero, nigromante o asesino**/
    int clase;
    int nivelDeJuego; /**nivel en que se quedó el jugador*/
    stAtributos atribPersonaje; /**estructura anidada,dentro tiene otra estructura,
    que tiene atributos como fuerza,destreza. Varia según la clase elegida*/
    stInventario inv;
    int hp;
    int mp;
} stPersonaje;

typedef struct /** IN PROGREZ*/
{
    int idMarcador;
    int idPersonaje;
    int Puntaje;
    int nivelMaximo;
    int tiempoJuego;
} stMarcador;

const int DIM = sizeof(stPersonaje); /** para las funciones de archivo**/

/**tienda*/
void Tienda(stPersonaje *player);
int CompraPocionesHp(stInventario *inv,int pocioneshpcant);
int CompraPocionesMP(stInventario *inv,int pocionesmpcant);
int CompraEspada(stPersonaje *player, int espadacant);
int CompraHacha(stPersonaje *player,int hachacant);
int CompraEscudo(stPersonaje *player,int escudocant);
int CompraCalavera(stPersonaje *player,int calaveracant);
int CompraBaston(stPersonaje *player,int bastoncant);
int CompraDaga(stPersonaje *player,int dagacant);
/**menu  principal*/
void MenuPrincipal();
/**nuevo juego**/
void NuevoJuego(); /**incluye crear personaje**/
/**creacion personaje*/
void InicioDePersonaje(stPersonaje *player);
int obtenerClase(char clase[]);
void AtributosGuerrero(stAtributos *aux);
void AtributosHechicero(stAtributos *aux);
void AtributosNigromante(stAtributos *aux);
void AtributosAsesino(stAtributos *aux);
/**cargar juego**/
void CargaJuego(); /**incluye cargar personaje ya creado desde el archivo**/
void Marcadores(); /**abre el archivo y muestra una tabla de posiciones**/
/**juego**/
void GuardarProgreso(stPersonaje *player); /**guarda progreso en el archivo*/
int Jugar(stPersonaje *player); /**va a ser llamada ya por NuevoJuego o CargarJuego, contiene todos los
niveles, la tienda y todo el gameplay. Devuelve flag para saber si sigue jugando o murio*/
int CicloPelea(stPersonaje *aux, int hpMon, int danoMon, char nombreMon[]);
void RecompensaPelea(stPersonaje *aux, int mejoraAtrib, int dinero); /**sube atrib y el dinero*/
int AtaqueBasico(stPersonaje aux);
int AtaqueCargado(stPersonaje aux);

/**para hacer*/
/**marcadores,RecompensaPelea,CargarJuego*/

int main()
{
    SetConsoleTitle("THE LEYEND OF C");
    srand(time(NULL));

    stPersonaje partida; /**estructura de la partida: se modifica segun la save o se crea
    una nueva si no hay save anterior*/

    int controlmenu = 0;
    char controlgeneral = 0;
    printf("1:\tNUEVO JUEGO\n2:\tCARGAR JUEGO\n3:\tMARCADORES\n4:SALIR\n");
    scanf("%i",&controlmenu);

    do
    {
        switch(controlmenu)
        {
        case 1:
            NuevoJuego(&partida);
            break;
        case 2:
            CargaJuego(&partida);
            break;
        case 3:
            Marcadores();
            break;
        case 4:
            controlgeneral = 'n';
            break;
        }
        printf("Volver al menu principal? s/n\n");
        scanf("\n%c",&controlgeneral);
    }
    while(controlgeneral == 's');


    return 0;
}

void InicioDePersonaje(stPersonaje *player)
{
    /**crea una nueva save**/

    FILE * archi = NULL;
    int cerroarchivo = 0;
    int escribioarchivo = 0;

    fopen(SAVE,"wb");

    if (archi!=NULL)
    {
        printf("\nIngrese su nombre\n");
        fflush(stdin);
        gets(player->nombre);

        printf("Indica el genero con el cual te identificas\n");
        scanf("\n%c",&player->genero);

        printf("Se le presentaran las siguientes clases las cual puede elegir\n");

        player->clase = obtenerClase(player->tipoClase);

        player->nivelDeJuego=0;
        player->idPersonaje = 1;
        player->idMarcador = 1;

        switch (player->clase)
        {
        case 1:
            atributosGuerrero(&player->atribPersonaje);
            strcpy(player->inv.arma,"Espada basica");
            strcpy(player->inv.escudo,"Escudo basico");
            break;
        case 2:
            atributosNigromante(&player->atribPersonaje);
            strcpy(player->inv.arma,"Baculo basico");
            strcpy(player->inv.escudo,"Grimorio");
            break;
        case 3:
            atributosAsesino(&player->atribPersonaje);
            strcpy(player->inv.arma,"Daga basica");
            break;
        case 4:
            atributosHechicero(&player->atribPersonaje);
            strcpy(player->inv.arma,"Baston basico");
            strcpy(player->inv.escudo,"Biblia");
            break;
        }

        if(player->tipoClase == 1 || player->tipoClase == 3)
        {
            player->hp = 100;
            player->mp = 80;
        }
        else if(player->tipoClase == 2 || player->tipoClase == 4)
        {
            player->hp = 80;
            player->mp = 100;
        }

        player->inv.dinero=0;
        player->inv.pocioneshp=1;
        player->inv.pocionesmp=1;

        escribioarchivo = fwrite(&player,DIM ,1,archi);

        if(escribioarchivo <= 0)
        {
            printf("NO SE PUDO ESCRIBIR EN EL ARCHIVO SAVE.\n");

        }

        cerroarchivo = fclose(archi);

        if(cerroarchivo!= 0)
        {
            printf("NO SE PUDO CERRAR EL ARCHIVO SAVE.\n");
        }
    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO SAVE.\n");
    }
}


int obtenerClase(char clase[])
{
    int opc = 0;
    int tipo =0;


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
}

void atributosNigromante(stAtributos * aux)
{
    aux->destreza=rand()%5+1;
    aux->fuerza=rand()%5+6;
    aux->inteligencia=rand()%10+11;
    aux->magia=rand()%10+11;
    aux->defensa=rand()%5+6;
}

void atributosAsesino (stAtributos * aux)
{
    aux->destreza=rand()%10+15;
    aux->fuerza=rand()%10+6;
    aux->inteligencia=rand()%10+11;
    aux->magia=rand()%5+6;
    aux->defensa = 0;
}

void atributosHechicero (stAtributos * aux)
{
    aux->destreza=rand()%5+1;
    aux->fuerza=rand()%5+3;
    aux->inteligencia=rand()%10+11;
    aux->magia=rand()%10+11;
    aux->defensa=rand()%5+6;
}
void Tienda(stPersonaje *player)
{
    int seleccion = 0; /**control case*/
    char control = 0; /**control menu**/

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

        printf("\nDINERO: %i\n",player->inv.dinero);
        printf("\nPOCIONES HP en inventario: %i\n",player->inv.pocioneshp);
        printf("\nPOCIONES MP en inventario: %i\n\n",player->inv.pocionesmp);

        printf("\tOBJETO\t\tCANTIDAD\n\n");
        printf("1:\tPocion HP\t%i\n2:\tPocion MP\t%i\n3:\tEspada\t\t%i\n4:\tBaston\t\t%i\n5:\tCalavera\t%i\n6:\tEscudo\t\t%i\n7:\tHacha\t\t%i\n8:\tDaga\t\t%i\n9:\tSalir\n\n",haypocionhp,haypocionmp,hayespada,haybaston,haycalavera,hayescudo,hayhacha,haydaga);

        scanf("%i",&seleccion);

        switch(seleccion)
        {
        case 1:
            if(haypocionhp > 0)
            {
                haypocionhp = CompraPocionesHp(&player->inv,haypocionhp); /**recibe inventario porque usa el dinero y pocionhp*/
            }
            else
            {
                printf("NO HAY MAS POCIONES DE HP.\n");
            }
            break;
        case 2:
            if(haypocionmp > 0)
            {
                haypocionmp = CompraPocionesMP(&player->inv,haypocionmp);
            }
            else
            {
                printf("NO HAY MAS POCIONES DE MP.\n");
            }
            break;
        case 3:
            if(hayespada == 1)
            {
                hayespada = CompraEspada(player,hayespada);
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 ESPADA GUERRERO!\n");
            }
            break;
        case 4:
            if(haybaston = 1)
            {
                haybaston = CompraBaston(player,haybaston);
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 BASTON MAGO!\n");
            }
            break;
        case 5:
            if(haycalavera = 1)
            {
                haycalavera = CompraCalavera(player,haycalavera);
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 CALAVERA NIGROMANTE!\n");
            }
            break;
        case 6:
            if(hayescudo = 1)
            {
                hayescudo = CompraEscudo(player,hayescudo);
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 ESCUDO GUERRERO!\n");
            }
            break;
        case 7:
            if(hayhacha = 1)
            {
                hayhacha = CompraHacha(player,hayhacha);
            }
            else
            {
                printf("SOLO PUEDES COMPRAR 1 HACHA GUERRERO!\n");
            }
            break;
        case 8:
            if(haydaga > 0 )
            {
                haydaga = CompraDaga(player,haydaga);
            }
            else
            {
                printf("NO HAY MAS DAGAS PARA TI PICARON.\n");
            }
            break;
        case 9:
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
    while( control =='s');
}

int CompraPocionesHp(stInventario *inv,int pocioneshpcant)
{
    int seleccion = 0;
    char control = 0;
    int cantidad = 0;
    int compra = 0;
    int flag = 1;
    char decision = 0;
    char op = 0;

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

                if(compra <= inv->dinero)
                {
                    printf("Gracias por su compra!\n");
                    inv->dinero = inv->dinero - compra;
                    inv->pocioneshp = inv->pocioneshp + cantidad;
                    pocioneshpcant = pocioneshpcant - cantidad;
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
        while( decision =='s' && flag!= 0);
    }

    return pocioneshpcant;
}

int CompraPocionesMP(stInventario *inv,int pocionesmpcant)
{

    int seleccion = 0;
    char control = 0;
    int cantidad = 0;
    int compra = 0;
    int flag = 1;
    char decision = 0;
    char op = 0;

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
            compra = cantidad * PRECIOMP;
            printf("El total es: %i\n",compra);
            printf("Esta seguro? s/n");
            scanf("\n%c",&decision);
            if(decision=='s')
            {

                if(compra <= inv->dinero)
                {
                    printf("Gracias por su compra!\n");
                    inv->dinero = inv->dinero - compra;
                    inv->pocionesmp = inv->pocionesmp + cantidad;
                    pocionesmpcant = pocionesmpcant - cantidad;
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
        while( decision =='s' && flag!= 0);
    }

    return pocionesmpcant;
}

int CompraEspada(stPersonaje *player, int espadacant)
{
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio de la espada es: %i\n",PRECIOESPADA);
    printf("Desea comprar la espada?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = PRECIOESPADA;
        if( player->inv.dinero >= compra)
        {
            /**la espada sube 5 de fuerza*/
            player->atribPersonaje.fuerza = player->atribPersonaje.fuerza + 5;
            /** y ademas sube 3 de destreza*/
            player->atribPersonaje.destreza = player->atribPersonaje.destreza + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            espadacant = 0;
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return espadacant;
}

int CompraBaston(stPersonaje *player, int bastoncant)
{
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio del bast%cn es: %i\n",162,PRECIOBASTON);
    printf("Desea comprar el bast%cn?\n",162);
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = PRECIOBASTON;
        if( player->inv.dinero >= compra)
        {
            /**el bastón sube 5 de magia*/
            player->atribPersonaje.magia = player->atribPersonaje.magia + 5;
            /** y ademas sube 3 de inteligencia*/
            player->atribPersonaje.inteligencia = player->atribPersonaje.inteligencia + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            bastoncant = 0;
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return bastoncant;
}

int CompraCalavera(stPersonaje *player, int calaveracant)
{
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio de la calavera es: %i\n",PRECIOCALAVERA);
    printf("Desea comprar la calavera?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = PRECIOCALAVERA;
        if( player->inv.dinero >= compra)
        {
            /**la calavera sube 5 de defensa*/
            player->atribPersonaje.defensa = player->atribPersonaje.defensa + 5;
            /** y ademas sube 3 de magia*/
            player->atribPersonaje.magia = player->atribPersonaje.magia + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            calaveracant = 0;
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return calaveracant;
}

int CompraEscudo(stPersonaje *player, int escudocant)
{
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio del escudo es: %i\n",PRECIOESCUDO);
    printf("Desea comprar el escudo?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = PRECIOESCUDO;
        if( player->inv.dinero >= compra)
        {
            /**la calavera sube 5 de defensa*/
            player->atribPersonaje.defensa = player->atribPersonaje.defensa + 5;
            /** y ademas sube 3 de destreza*/
            player->atribPersonaje.destreza = player->atribPersonaje.destreza + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            escudocant = 0;
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return escudocant;
}

int CompraHacha(stPersonaje *player, int hachacant)
{
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio del hacha es: %i\n",PRECIOHACHA);
    printf("Desea comprar el hacha?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = PRECIOHACHA;
        if( player->inv.dinero >= compra)
        {
            /**el sube 5 de fuerza*/
            player->atribPersonaje.fuerza = player->atribPersonaje.fuerza + 5;
            /** y ademas sube 3 de destreza*/
            player->atribPersonaje.destreza = player->atribPersonaje.destreza + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            hachacant = 0;
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return hachacant;
}

int CompraDaga(stPersonaje *player, int dagacant)
{

    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio de la daga es: %i\n",PRECIODAGA);
    printf("Desea comprar la daga?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = PRECIODAGA;
        if( player->inv.dinero >= compra)
        {
            /**la daga sube 3 de fuerza*/
            player->atribPersonaje.fuerza = player->atribPersonaje.fuerza + 3;
            /** y ademas sube 3 de destreza*/
            player->atribPersonaje.destreza = player->atribPersonaje.destreza + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            dagacant = dagacant - 1;
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return dagacant;
}

void MostrarPersonaje(stPersonaje *player)
{
    printf("Tu personaje es:\n");

    printf("\n----------------------------------------------------------\n");
    printf("\nDatos del jugador");
    printf("\nNombre:..............:\t\t%s",player->nombre);
    printf("\nGenero:..............:\t\t%s",player->genero);
    printf("\nMana:................:\t\t%d",player->mp);
    printf("\nVida:................:\t\t%d",player->hp);
    printf("\nNivel Actual:........:\t\t%d",player->nivelDeJuego);
    printf("\n----------------------------------------------------------");
    printf("\nAtributos del jugador");
    printf("\nFuerza:..............:\t\t[%d]",player->atribPersonaje.fuerza);
    printf("\nMagia:...............:\t\t[%d]",player->atribPersonaje.magia);
    printf("\nDefensa:.............:\t\t[%d]",player->atribPersonaje.defensa);
    printf("\nDestreza:............:\t\t[%d]",player->atribPersonaje.destreza);
    printf("\nInteligencia:........:\t\t[%d]",player->atribPersonaje.inteligencia);
    printf("\n----------------------------------------------------------");
    printf("\nInventario del jugador");
    printf("\nArma:................:\t\t[%s]",player->inv.arma);
    printf("\nDinero:..............:\t\t[%d]",player->inv.dinero);
    printf("\nPociones de vida:....:\t\t[%d]",player->inv.pocioneshp);
    printf("\nPociones de mana:....:\t\t[%d]",player->inv.pocionesmp);
    printf("\nEscudo:..............:\t\t[%s]",player->inv.escudo);
    printf("\n----------------------------------------------------------\n");

}

/*void funcionDeCombatesPorNivel ()
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

    }///while((continuar=='s')&&(aux.nivel<25)&&(aux.hp>0));
    }*/

int CicloPelea(stPersonaje *aux,int hpMon,int danoMon,char nombreMon[])
{
    int opc=0;
    int pasaNivel=0;
    do
    {
        printf("te encontraste con %s",nombreMon);
        scanf("%d",&opc);
        switch(opc)
        {
        case 1:
            ///ataque normal
            hpMon = hpMon - ataqueBasico(aux);
            break;

        case 2:
            ///ataque especial
            hpMon = hpMon -ataqueCargado(aux);
            aux->mp = aux-> mp-10;
            break;

        case 3:
            ///pocion de vida
            aux->hp=aux->hp+20;
            break;

        case 4:
            ///pocion de mana
            aux->mp=aux->mp+20;
            break;
        }

        ///turno del monstruo
        switch(aux->clase)
        {
        case 1: /**caso guerrero*/
            aux->hp = aux->hp - (rand()%6 + danoMon - ((aux->atribPersonaje.defensa+aux->atribPersonaje.destreza)/2));
        case 2: /**caso nigromante*/
            aux->hp = aux->hp - (rand()%6 + danoMon - ((aux->atribPersonaje.inteligencia + aux->atribPersonaje.defensa)/2));
        case 3: /**caso asesino**/
            aux->hp = aux->hp - (rand()%6 + danoMon - (aux->atribPersonaje.destreza));
        case 4: /**caso hechicero**/
            aux->hp = aux->hp - (rand()%6 + danoMon - ((aux->atribPersonaje.inteligencia + aux->atribPersonaje.defensa)/2));
        }

    }while ( (aux->hp > 0) && ( hpMon > 0 ));

    if(hpMon <= 0)
    {
        pasaNivel=1;
    }


    return pasaNivel;
}

void RecompensaPelea(stPersonaje *aux, int mejoraAtrib, int dinero)
{

    /**Se modifican parajo todos los atributos de acuerdo al nivel*/
}

int ataqueBasico (stPersonaje aux)
{
    int ataque1=0;
    int caso=0;
    caso=aux.clase;
    switch (caso)
    {
    case 1:
        ataque1=rand()%5+aux.atribPersonaje.fuerza;
        break;

    case 2:
        ataque1=rand()%5+aux.atribPersonaje.magia;
        break;

    case 3:
        ataque1=rand()%10+aux.atribPersonaje.destreza;
        break;

    case 4:
        ataque1=rand()%5+aux.atribPersonaje.inteligencia;
        break;

    }

    return ataque1;
}

int ataqueCargado(stPersonaje aux)
{
    int ataque2=0;
    int caso=0;
    caso=aux.clase;
    switch (caso)
    {
    case 1:
        ataque2=rand()%5+aux.atribPersonaje.fuerza+10;
        break;

    case 2:
        ataque2=rand()%5+aux.atribPersonaje.magia+10;
        break;

    case 3:
        ataque2=rand()%10+aux.atribPersonaje.destreza+10;
        break;

    case 4:
        ataque2=rand()%5+aux.atribPersonaje.inteligencia+10;
        break;

    }

    return ataque2;
}

void MenuPrincipal()
{
    int controlmenu = 0;
    char controlgeneral = 0;
    printf("1:\tNUEVO JUEGO\n2:\tCARGAR JUEGO\n3:\tMARCADORES\n4:SALIR\n");
    scanf("%i",&controlmenu);

    do
    {
        switch(controlmenu)
        {
        case 1:
            NuevoJuego();
            break;
        case 2:
            CargaJuego();
            break;
        case 3:
            Marcadores();
            break;
        case 4:
            controlgeneral = 'n';
            break;
        }
        printf("Volver al menu principal? s/n\n");
        scanf("\n%c",&controlgeneral);
    }
    while(controlgeneral == 's');

    }

void NuevoJuego()
{
    stPersonaje player;
    int correjuego = 0;
    printf("**CREAR PERSONAJE, SE INICIAN LAS STATS POR DEFECTO\n");
    InicioDePersonaje(&player);
    printf("*ESTE ES SU PERSONAJE*");
    MostrarPersonaje(&player);

    do
    {
        printf("*EMPIEZA EL JUEGO*\n");
        correjuego = Jugar(&player);
    }
    while(correjuego == 1);

    printf("SALIO DEL JUEGO, SE GUARDA EL PROGRESO.\n");

    GuardarProgreso(&player); /**escribe en el archivo**/

    /*printf("SE GUARDO EL PROGRESO, SE ESCRIBE EN MARCADORES.\n");

    *Marcadores(&player);*/

}

void GuardarProgreso(stPersonaje *player)
{
    FILE* pfile = NULL;
    int cerroarchivo = 0;
    int escribioarchivo = 0;
    pfile = fopen(SAVE,"wb");

    if(pfile!=NULL)
    {
        printf("SE GUARDARA SU PROGRESO EN EL ARCHIVO SAVE...\n");

        escribioarchivo = fwrite(player,DIM,1,pfile);

        if(escribioarchivo <= 0)
        {
            printf("ERROR EN LA ESCRITURA DEL ARCHIVO SAVE.\n");
        }

        cerroarchivo = fclose(pfile);

        if(cerroarchivo!= 0)
        {
            printf("ERROR AL CERRAR EL ARCHIVO.\n");
        }
    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO SAVE.\n");
    }
}

int Jugar(stPersonaje *player)
{
    int flagGame = 1; /**mientras esto sea 1, el juego sigue, cuando muere o quiere salir
    se vuelve 0**/
    int nivelJuego = 1; /**empieza desde el nivel 1*/
    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char controlNivel = 0; /**cuando pierde, le pregunta si quiere volver a intentar*/

    while(flagGame == 1)
    {
        nivelJuego = 1;
        switch(nivelJuego)
            {
            case 1:
                do
                {
                    printf("NIVEL 1\n");

                    pasaNivel = CicloPelea(player,100,30,"FireSpawn");

                    if(pasaNivel == 1)
                    {
                        nivelJuego = 2;
                        RecompensaPelea(player,2,100);
                        controlNivel = 'n';
                        break;
                    }
                    else
                    {
                        printf("DESEA VOLVER A INTENTARLO? s/n\n");
                        scanf("\n%c",&controlNivel);
                    }
                }
                while(controlNivel== 's');
            }
        }

    return flagGame;
}
