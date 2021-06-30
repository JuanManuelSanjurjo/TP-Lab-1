#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "gotoxy.h"
/**para tienda*/
#define FIL 10
#define COL 30
#define ART 8
/**archivos*/
#define SAVE "save.dat"
#define MARCADORES "marcadores.dat"
#define UTILIDADES "utilidades.txt"


typedef struct
{
    int fuerza;
    int inteligencia;
    int destreza;
    int magia;
    int defensa;
} stAtributos ; ///Estructura N1 la cual se usa en stPersonaje, define los atributos del personaje el cual iran cambiando

typedef struct
{
    int dinero;
    int pocioneshp;
    int pocionesmp;
    char arma[20];
    char escudo[20];
} stInventario; ///Estructura N2, Es el inventario del personaje, la cual tambien ira modificandose en el correr del juego

typedef struct
{
    char nombre[15];                                            /**Nombre que elegió el jugador*/
    char genero[15];                                            /**Elegido por el jugador*/
    char tipoClase[15];                                         /**guerrero, hechicero, nigromante o asesino*/
    int clase;                                                  /**Entero el cual se asigna el numero de clase para el manejo de ataque y defensa*/
    int nivelDeJuego;                                           /**nivel en que se quedó el jugador*/
    stAtributos atribPersonaje;                                 /**estructura anidada,dentro tiene otra estructura,que tiene atributos como fuerza,destreza. Varia según la clase elegida*/
    stInventario inv;                                           /**Estructura inventario*/
    int hp;                                                     /**Vida del personaje*/
    int mp;                                                     /**Mana del personaje*/
    int tiempo;                                                 /**Tiempo jugado*/
} stPersonaje;                                                  /**Estructura de personaje con la cual se juega*/


typedef struct
{
    char nombre[15];
    char clase[15];
    int nivelMaximo;
    stInventario invMarcador;
    int tiempoJuego;
} stMarcador;                                                   /**Estructura de marcadores se guarda cuando termina de jugar*/

/**Constantes tamaño de estructuras**/
const int DIM = sizeof(stPersonaje); /** para las funciones de archivo**/
const int DIMMAR = sizeof(stMarcador); /**para las funciones de archivo**/

/// constantes visuales
const int continuarX=135;
const int linea1=46;
const int linea2=47;
const int linea3=48;

// VISULAES
void fadeIN(char frase[],int x,int y);
void fadeINTimed(char frase[],int x,int y, int time);
void continuar();
void fadeOUT(char frase[],int x,int y);
void fadeInOut (char frase[],int x,int y);
void cascadaTexto(char texto[], int x,int y);
void limpiaLinea (int x, int y);
void pantallaInicio();
void fadeINPantalla(char frase[],int x, int y);
void golpe();


/**tienda*/
void Tienda(stPersonaje *player);
void AsignarNombres(char p[][30]);
void AsignarArticulos(int a[]);
void MostrarMenuTienda(char p[][30],int a[]);
int CompraPocionesHp(stInventario *inv,int pocioneshpcant);
int CompraPocionesMP(stInventario *inv,int pocionesmpcant);
int CompraEspada(stPersonaje *player, int espadacant);
int CompraHacha(stPersonaje *player,int hachacant);
int CompraEscudo(stPersonaje *player,int escudocant);
int CompraCalavera(stPersonaje *player,int calaveracant);
int CompraBaston(stPersonaje *player,int bastoncant);
int CompraDaga(stPersonaje *player,int dagacant);

/**menu  principal*/
/**nuevo juego**/
void NuevoJuego(stPersonaje *player);                   /**incluye crear personaje**/

/**creacion personaje*/
void InicioDePersonaje(stPersonaje *player);
int obtenerClase(char clase[]);
void AtributosGuerrero(stAtributos *aux);
void AtributosHechicero(stAtributos *aux);
void AtributosNigromante(stAtributos *aux);
void AtributosAsesino(stAtributos *aux);
void MostrarPersonaje(stPersonaje *player);

/**cargar juego**/
void CargaJuego(stPersonaje*player);                    /**incluye cargar personaje ya creado desde el archivo**/
int CargaDePersonaje(char nombreBuscar[],stPersonaje *player);  /**carga un registro para copiar a stPersonaje*/
void MuestraArchivoMarcadores();                        /**abre el archivo y muestra una tabla de posiciones**/
void MuestraUnMarcador(stMarcador aux);                 /**Muestra los marcadores*/
/**juego**/
int cuentaRegistros();
void mostrarSaves();
void GuardarProgreso(stPersonaje *player);              /**guarda progreso en el archivo*/
int Jugar(stPersonaje *player);                         /**va a ser llamada ya por NuevoJuego o CargarJuego, contiene todos los
                                                        niveles, la tienda y todo el gameplay. Devuelve flag para saber si sigue jugando o murio*/
int CicloPelea(stPersonaje *aux, int hpMon, int danoMon, char nombreMon[]);
int cicloPeleaBossN(stPersonaje *aux,int hpMon,int danoMon,char nombreMon[]);
void RecompensaPelea(stPersonaje *aux, int mejoraAtrib, int dinero);        /**sube atrib y el dinero*/

/**niveles del juego**/
int Nivel1(stPersonaje *player);
int nivelBossN (stPersonaje * player);
void CalcularTiempoJuego(int tiempo);
int Nivel2(stPersonaje *player);
int Nivel3(stPersonaje *player);
int Nivel4(stPersonaje *player);
int Nivel5(stPersonaje *player);
int Nivel7(stPersonaje *player);
int Nivel8 (stPersonaje * player);
int Nivel9 (stPersonaje * player);
int Nivel11 (stPersonaje * player);
int Nivel12 (stPersonaje * player);
int Nivel13 (stPersonaje * player);
int Nivel14 (stPersonaje * player);
int nergalYereshkigal(stPersonaje *aux,int hpMon,int danoMon);  /**primer boss*/
int acertijoEsfigie () ;                                       /**Segundo boss*/
int validacionYresultado (int opcion,int correcta);
int peleaEsfigie (stPersonaje * player);
int cicloPeleaBossN (stPersonaje *aux,int hpMon,int danoMon,char nombreMon[]);
int nivelBossN (stPersonaje * player);
int acertijoEsfigie();

/**ataque pj**/
int AtaqueBasico(stPersonaje *aux);
int AtaqueCargado(stPersonaje *aux);

/**ataque bicho**/
int AtaqueBicho1(stPersonaje *aux,int danoMon);
int AtaqueBicho2(stPersonaje * aux,int danoMon);
int AtaqueBicho3(stPersonaje *aux,int danoMon);
int AtaqueBicho4(stPersonaje *aux,int danoMon);

/**utilidades**/
void PausaLimpia();

/**Historia del juego**/
void Introduccion();

/**marcadores*/
void mostrarMarcadores();
void mostrarUnMarcador(stMarcador aux);
void guardarEnArchivoMarcadores(stPersonaje *aux);

int main()
{


    SetConsoleTitle("THE LEGEND OF C");
    srand(time(NULL));
    system("mode 150, 50");


    //pantallaInicio();
    int controlmenu=0;

    stPersonaje partida; /**estructura de la partida: se modifica segun la save o se crea
    una nueva si no hay save anterior*/

    do
    {
        system("cls");

        hidecursor(1);
        fadeINTimed("1:NUEVO JUEGO",65,20,80);
        fadeINTimed("2:CARGAR JUEGO",65,22,80);
        fadeINTimed("3:MARCADORES",65,24,80);
        fadeINTimed("4:SALIR   ",65,26,80);

        color(0);
        hidecursor(0);
        scanf("%i",&controlmenu);
        system("cls");
        hidecursor(1);

        switch(controlmenu)
        {
        case 1:
            cascadaTexto("**ELEJISTE EMPEZAR UNA PARTIDA NUEVA.\n", 0,3);
            NuevoJuego(&partida);
            break;
        case 2:
            cascadaTexto("**ELEJISTE CARGAR UNA PARTIDA ANTERIOR.\n",0,3);
            CargaJuego(&partida);
            break;
        case 3:
            color(7);
            mostrarMarcadores();
            continuar();
            break;
        case 4:
            cascadaTexto("Gracias por jugar The Legend Of C!!",55,22);
            _sleep(500);
            gotoxy(0,linea1);
            exit(0);
            break;
        default:
            cascadaTexto("Gracias por jugar The Legend Of C!!",55,22);
            _sleep(500);
            gotoxy(0,linea1);
            exit(0);
        }
    }
    while(controlmenu!=4);

    return 0;
}

void InicioDePersonaje(stPersonaje *player) ///Crea un personaje nuevo, se invoca de Nuevo Juego menu
{
    /**crea una nueva save**/

    FILE * archi = NULL;
    int cerroarchivo = 0;
    int escribioarchivo = 0;

    archi = fopen(SAVE,"ab");

    if (archi!=NULL)
    {

        cascadaTexto("Ingrese su nombre: ",0,whereY()+1);
        fflush(stdin);
        gets(player->nombre);

        cascadaTexto("Indica el genero con el cual te identificas:  ",0,whereY()+1);
        fflush(stdin);
        gets(player->genero);

        cascadaTexto("Se le presentaran las siguientes clases las cual puede elegir",0,whereY()+1);
        continuar();

        player->clase = obtenerClase(player->tipoClase);
        player->nivelDeJuego = 1;                           ///NIvel del juego 1 ya que arranca por nivel 1 dentro del switch de juego
        player->tiempo = 0;                                 ///Se setea en 0 el tiempo de juego

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
            strcpy(player->inv.escudo," ");

            break;
        case 4:
            atributosHechicero(&player->atribPersonaje);
            strcpy(player->inv.arma,"Baston basico");
            strcpy(player->inv.escudo,"Biblia");
            break;
        }

        if(player->clase == 1 || player->clase == 3)        ///cambia los atributos dependiendo la clase
        {
            player->hp = 100;
            player->mp = 80;
        }
        else if(player->clase == 2 || player->clase == 4)
        {
            player->hp = 80;
            player->mp = 100;
        }

        player->inv.dinero=0;
        player->inv.pocioneshp=1;
        player->inv.pocionesmp=1;

        escribioarchivo = fwrite(player,DIM,1,archi);

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


int obtenerClase(char clase[])  ///Obtiene una clase la cual le da atributos al personaje
{
    int opc = 0;
    int tipo =0;


    fadeIN("1. Guerrero: Eres el elegido para ser el salvador de la aldea, llevas contigo la bendicion y la fuerza de la luz\n para combatir lo que acecha en las tinieblas. Clasico combatiente perfectamente ideado para el combate directo,\ncon sus propias capacidades de aguante que hacen trizas a los enemigos en un abrir y cerrar de ojos.\nSus armas afines son las espadas y las hachas. Toda la fe de la aldea esta puesta en este valeroso guerrero.\n\n",0,14);
    fadeIN("2. Nigromante: Tras la invasion de las criaturas en la aldea, en lugar de combatirlas, aprendiste sus artes oscuras.\n Usas el poder de la mismisima muerte y destruccion para sacarle todo el provecho posible, causar estragos y \ntemores a los enemigos, incluso utilizas sus cadaveres para invocar en siervos en su lugar. \nSu arma afin es el baculo y el baston. La gente le teme a los nigromantes.\n\n",0,whereY());
    fadeIN("3. Asesino: No defiendes a la aldea ni a las criaturas del mal. Haces tu propio camino y te preocupas por tus intereses\n.Expertos verdugos escurridizos que se tienen que exponer normalmente bastante, pero con una brutal con habilidad\n de deshacerse de los enemigos en un abrir y cerrar de ojos. Provocando muertes por doquier. Su arma afin es \nla daga yo el cuchillo.\n\n",0,whereY());
    fadeIN("4. Hechicero: Aprendiste las artes de la hechiceria para defender a tu aldea. Siempre causaran estragos elementales entre\n las filas enemigas. Sus hechizos elementales van desde ataques devastadores a protectores e incluso curadores,\nsiempre dispuestos a entrar en pleno fragor de la batalla. El hechicero utiliza el poder de su inteligencia\n en carencia de su fuerza. Sus armas afines son el baculo y el baston.\n\n",0,whereY());

    cascadaTexto("Dentro de estas clases, cual crees que sera la mas adecuada para ti?\n",0,whereY()+1);
    scanf("%d",&opc);

    while ((opc>4)&&(opc<1))
    {
        printf("La opcion es incorrecta, ingrese un dato correcto\n");
        scanf("%d",&opc);
    }

    cascadaTexto("Ya lo veremos...\n",65,whereY()+2);
    continuar();

    switch (opc)                    ///Se le asigna la clase al string clase []
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

void atributosGuerrero (stAtributos * aux)  ///De acuerdo a la clase se la asignan atributos con las siguientes funciones para cada clase
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
    int seleccion = 0;                                              /**control case*/
    char control = 0;                                               /**control menu**/
    char nombres[FIL][COL];
    int articulos[ART];

    AsignarNombres(nombres);

    AsignarArticulos(articulos);

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
        system("cls");
        fadeIN("\nBienvenido/a a la tienda! Que desea??\n",0,6);

        printf("\nDINERO: %i\n",player->inv.dinero);
        printf("\nPOCIONES HP en inventario: %i\n",player->inv.pocioneshp);
        printf("\nPOCIONES MP en inventario: %i\n\n",player->inv.pocionesmp);

        MostrarMenuTienda(nombres,articulos);

        scanf("%i",&seleccion);

        switch(seleccion)
        {
        case 1:
            /**todas las clases pueden comprar pociones**/
            if(articulos[0] > 0)
            {
                articulos[0] = CompraPocionesHp(&player->inv,articulos[0]); /**recibe inventario porque usa el dinero y pocionhp*/
            }
            else
            {
                printf("NO HAY MAS POCIONES DE HP.\n");
            }
            break;
        case 2:
            /**todas las clases pueden comprar pociones*/
            if(articulos[1] > 0)
            {
                articulos[1] = CompraPocionesMP(&player->inv,articulos[1]);
            }
            else
            {
                printf("NO HAY MAS POCIONES DE MP.\n");
            }
            break;
        case 3:                             ///comprar espada para guerrero
            if(player->clase == 1)
            {
                if(articulos[2] == 1)
                {
                    articulos[2] = CompraEspada(player,articulos[2]);
                }
                else
                {
                    printf("SOLO PUEDES COMPRAR 1 ESPADA GUERRERO!\n");
                }
            }
            else
            {
                printf("SOLO LOS GUERREROS PUEDEN COMPRAR LA ESPADA!\n");
            }
            break;
        case 4:                              ///comprar baston para mago
            if(player->clase == 4)
            {
                if(articulos[3] == 1)
                {
                    articulos[3] = CompraBaston(player,articulos[3]);
                }
                else
                {
                    printf("SOLO PUEDES COMPRAR 1 BASTON MAGO!\n");
                }
            }
            else
            {
                printf("SOLO LOS HECHICEROS PUEDEN COMPRAR EL BASTON.\n");
            }
            break;
        case 5:                                 ///Comprar calavera para nigromante
            if(player->clase == 2)
            {
                if(articulos[4] == 1)
                {
                    articulos[4] = CompraCalavera(player,articulos[4]);
                }
                else
                {
                    printf("SOLO PUEDES COMPRAR 1 CALAVERA NIGROMANTE!\n");
                }
            }
            else
            {
                printf("SOLO LOS NIGROMANTES PUEDEN COMPRAR LA CALAVERA.\n");
            }
            break;
        case 6:                                 ///Comprar escudo para guerrero
            if(player->clase == 1 || player->clase == 4)
            {
                if(articulos[5] == 1)
                {
                    articulos[5] = CompraEscudo(player,articulos[5]);
                }
                else
                {
                    printf("SOLO PUEDES COMPRAR 1 ESCUDO GUERRERO!\n");
                }
            }
            else
            {
                printf("SOLO LOS GUERREROS O LOS MAGOS PUEDEN COMPRAR ESCUDO.\n");
            }
            break;
        case 7:                                         ///Comprar hacha para guerrero
            if(player->clase == 1)
            {
                if(articulos[6] == 1)
                {
                    articulos[6] = CompraHacha(player,articulos[6]);
                }
                else
                {
                    printf("SOLO PUEDES COMPRAR 1 HACHA GUERRERO!\n");
                }
            }
            else
            {
                printf("SOLO LOS GUERREROS PUEDEN COMPRAR EL HACHA.\n");
            }
            break;
        case 8:                                            ///Comprar dagas para asesino
            if(player->clase == 3)
            {
                if(articulos[7] > 0 )
                {
                    articulos[7] = CompraDaga(player,articulos[7]);
                }
                else
                {
                    printf("NO HAY MAS DAGAS PARA TI PICARON.\n");
                }
            }
            else
            {
                printf("SOLO LOS ASESINOS PUEDEN COMPRAR LAS DAGAS.\n");
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

int CompraPocionesHp(stInventario *inv,int pocioneshpcant)      ///Funcion que asigna pociones al personaje
{
    int preciohp = 20;
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
            compra = cantidad * preciohp;
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

int CompraPocionesMP(stInventario *inv,int pocionesmpcant)              ///Funciones que asigna pociones de mana
{
    int preciomp = 20;
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
            compra = cantidad * preciomp;
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

int CompraEspada(stPersonaje *player, int espadacant)                   ///Funcion para comprar espada
{
    int precioespada = 50;
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio de la espada es: %i\n",precioespada);
    printf("Desea comprar la espada?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = precioespada;
        if( player->inv.dinero >= compra)
        {
            /**la espada sube 5 de fuerza*/
            player->atribPersonaje.fuerza = player->atribPersonaje.fuerza + 5;
            /** y ademas sube 3 de destreza*/
            player->atribPersonaje.destreza = player->atribPersonaje.destreza + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            espadacant = 0;
            strcpy(player->inv.arma,"Espada de Luz");
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return espadacant;
}

int CompraBaston(stPersonaje *player, int bastoncant)                   ///Funcion para comprar baston
{
    const int preciobaston = 50;
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio del bast%cn es: %i\n",162,preciobaston);
    printf("Desea comprar el bast%cn?\n",162);
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = preciobaston;
        if( player->inv.dinero >= compra)
        {
            /**el bastón sube 5 de magia*/
            player->atribPersonaje.magia = player->atribPersonaje.magia + 5;
            /** y ademas sube 3 de inteligencia*/
            player->atribPersonaje.inteligencia = player->atribPersonaje.inteligencia + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            bastoncant = 0;
            strcpy(player->inv.escudo,"Baston elemental");
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return bastoncant;
}

int CompraCalavera(stPersonaje *player, int calaveracant)           ///Funcion para comprar calavera
{
    int preciocalavera = 50;
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio de la calavera es: %i\n",preciocalavera);
    printf("Desea comprar la calavera?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = preciocalavera;
        if( player->inv.dinero >= compra)
        {
            /**la calavera sube 5 de defensa*/
            player->atribPersonaje.defensa = player->atribPersonaje.defensa + 5;
            /** y ademas sube 3 de magia*/
            player->atribPersonaje.magia = player->atribPersonaje.magia + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            calaveracant = 0;
            strcpy(player->inv.escudo,"Escudo Calavera");
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return calaveracant;
}

int CompraEscudo(stPersonaje *player, int escudocant)                       ///Funcion para comprar escudo
{
    int precioescudo = 50;
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio del escudo es: %i\n",precioescudo);
    printf("Desea comprar el escudo?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = precioescudo;
        if( player->inv.dinero >= compra)
        {
            /**la calavera sube 5 de defensa*/
            player->atribPersonaje.defensa = player->atribPersonaje.defensa + 5;
            /** y ademas sube 3 de destreza*/
            player->atribPersonaje.destreza = player->atribPersonaje.destreza + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            escudocant = 0;
            strcpy(player->inv.escudo,"Escudo Inri");
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return escudocant;
}

int CompraHacha(stPersonaje *player, int hachacant)                     ///Funcion para comprar hacha
{
    int preciohacha = 50;
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio del hacha es: %i\n",preciohacha);
    printf("Desea comprar el hacha?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = preciohacha;
        if( player->inv.dinero >= compra)
        {
            /**el sube 5 de fuerza*/
            player->atribPersonaje.fuerza = player->atribPersonaje.fuerza + 5;
            /** y ademas sube 3 de destreza*/
            player->atribPersonaje.destreza = player->atribPersonaje.destreza + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            hachacant = 0;
            strcpy(player->inv.arma,"Hacha Valkiria");
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return hachacant;
}

int CompraDaga(stPersonaje *player, int dagacant)                       ///Funcion para comprar daga
{
    int preciodaga = 50;
    int seleccion = 0;
    char control = 0;
    int compra = 0;

    printf("El precio de la daga es: %i\n",preciodaga);
    printf("Desea comprar la daga?\n");
    scanf("\n%c",&control);
    if(control == 's')
    {
        compra = preciodaga;
        if( player->inv.dinero >= compra)
        {
            /**la daga sube 3 de fuerza*/
            player->atribPersonaje.fuerza = player->atribPersonaje.fuerza + 3;
            /** y ademas sube 3 de destreza*/
            player->atribPersonaje.destreza = player->atribPersonaje.destreza + 3;
            /**le cobramos*/
            player->inv.dinero = player->inv.dinero - compra;
            dagacant = dagacant - 1;
            strcpy(player->inv.arma,"Diente del diablo");
            printf("Gracias por su compra!\n");
        }
        else
        {
            printf("No tienes suficiente dinero!! =( \n");
        }
    }

    return dagacant;
}

void MostrarPersonaje(stPersonaje *player)                                      ///Muestra el personaje en su totalidad con un puntero
{
    gotoxy(0,6);
    printf("\nNombre:..............:\t\t%s",player->nombre);
    printf("\nGenero:..............:\t\t%s",player->genero);
    printf("\nClase:...............:\t\t%s",player->tipoClase);
    printf("\nVida:................:\t\t%d",player->hp);
    printf("\nMana:................:\t\t%d",player->mp);
    printf("\nNivel Actual:........:\t\t%d",player->nivelDeJuego);
    fadeINTimed("\n----------------------------------------------------------",0,whereY(),50);
    fadeINTimed("\nAtributos del jugador",0,whereY(),50);
    printf("\nFuerza:..............:\t\t[%d]",player->atribPersonaje.fuerza);
    printf("\nMagia:...............:\t\t[%d]",player->atribPersonaje.magia);
    printf("\nDefensa:.............:\t\t[%d]",player->atribPersonaje.defensa);
    printf("\nDestreza:............:\t\t[%d]",player->atribPersonaje.destreza);
    printf("\nInteligencia:........:\t\t[%d]",player->atribPersonaje.inteligencia);
    fadeINTimed("\n----------------------------------------------------------",0,whereY(),50);
    fadeINTimed("\nInventario del jugador",0,whereY(),50);
    printf("\nArma:................:\t\t[%s]",player->inv.arma);
    printf("\nEscudo:..............:\t\t[%s]",player->inv.escudo);
    printf("\nPociones de vida:....:\t\t[%d]",player->inv.pocioneshp);
    printf("\nPociones de mana:....:\t\t[%d]",player->inv.pocionesmp);
    printf("\nDinero:..............:\t\t[%d]",player->inv.dinero);
    fadeINTimed("\n----------------------------------------------------------",0,whereY(),50);

}

int CicloPelea(stPersonaje *aux,int hpMon,int danoMon,char nombreMon[])                 ///Funcion basica de pelea la cual es llamada por todos los niveles
{
    int opc=0;
    int pasaNivel=0;
    int golpePj = 0;
    int golpeMon = 0;

    system("cls");
    fadeIN("*BATALLA*",65,3);
    cascadaTexto("Te encuentras con ",0,whereY()+2);
    gotoxy(whereX(),whereY());
    printf("%s",nombreMon);
    cascadaTexto("Su hp es de: ",0,whereY()+1);
    gotoxy(whereX()+1,whereY());
    printf("%d",hpMon);
    cascadaTexto("Su ataque es de: ",0,whereY()+1);
    gotoxy(whereX()+1,whereY());
    printf("%d",danoMon);

    do
    {
        fadeIN("*BATALLA*",65,3);
        gotoxy(64,13);
        printf("HP PJ:     %4d",aux->hp);
        gotoxy(64,14);
        printf("MP PJ:     %4d",aux->mp);
        gotoxy(64,15);
        printf("HP MON:    %4d",hpMon);
        gotoxy(64,16);
        printf("PocionesHP:%4d",aux->inv.pocioneshp);
        gotoxy(64,17);
        printf("PocionesMP:%4d",aux->inv.pocionesmp);

        gotoxy(58,21);
        printf("1:\tAtaque normal");
        gotoxy(58,22);
        printf("2:\tAtaque Especial");
        gotoxy(58,23);
        printf("3:\tTomar pocion hp");
        gotoxy(58,24);
        printf("4:\tTomar pocion mp");
        gotoxy(58,25);

        fadeIN("Que accion desea realizar",0,linea1);
        scanf("%d",&opc);
        limpiaLinea(0,linea1);

        switch(opc)
        {
        case 1:
            ///ataque normal
            gotoxy(0,27);
            printf("%s le hace un ataque normal a %s ",aux->nombre,nombreMon);
            golpePj = AtaqueBasico(aux);
            golpe();
            printf("y le infringe un golpe de %i\n",golpePj);
            hpMon = hpMon - golpePj;
            break;
        case 2:
            ///ataque especial
            gotoxy(0,27);
            printf("Ataque cargado...\n");
            golpePj = AtaqueCargado(aux);
            golpe();
            printf("tu golpe es de %i\n",golpePj);
            hpMon = hpMon - golpePj;
            aux->mp = aux-> mp-10;
            break;
        case 3:
            ///pocion de vida
            gotoxy(0,27);
            printf("Tomas una pocion HP...\n");
            aux->hp = aux->hp + 25;
            aux->inv.pocioneshp = aux->inv.pocioneshp - 1;
            break;
        case 4:
            ///pocion de mana
            gotoxy(0,27);
            printf("Tomas una pocion MP...\n");
            aux->mp= aux->mp+ 25;
            aux->inv.pocionesmp = aux->inv.pocionesmp - 1;
            break;
        }

        ///turno del monstruo
        switch(aux->clase)
        {
        case 1: /**caso guerrero*/
            gotoxy(0,29);
            printf("%s ataca a %s ",nombreMon,aux->nombre);
            golpeMon = AtaqueBicho1(aux,danoMon);
            golpe();
            aux->hp = aux->hp - golpeMon;
            printf("y le infringe un golpe de %i\n",golpeMon);
            break;
        case 2: /**caso nigromante*/
            gotoxy(0,29);
            printf("El bicho ataca nigromante:\n");
            golpeMon = AtaqueBicho2(aux,danoMon);
            golpe();
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        case 3: /**caso asesino**/
            gotoxy(0,29);
            printf("El bicho ataca asesino:\n");
            golpeMon = AtaqueBicho3(aux,danoMon);
            golpe();
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        case 4: /**caso hechicero**/
            gotoxy(0,29);
            printf("El bicho ataca hechicero:n");
            golpeMon = AtaqueBicho4(aux,danoMon);
            golpe();
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        }
        continuar();
        limpiaLinea(0,27);
        limpiaLinea(0,28);

    }
    while ( (aux->hp > 0) && ( hpMon > 0 ));

    if(hpMon <= 0)
    {
        cascadaTexto("Has derrotado a ",0,linea1);
        gotoxy(whereX(),whereY());
        printf("%s",nombreMon);
        pasaNivel = 1;
    }
    else
    {
        cascadaTexto("Has perdido!",0,linea1);
        pasaNivel = 0;
    }
    continuar();

    return pasaNivel;
}

int AtaqueBasico(stPersonaje *aux)                          ///Calcula el ataque basico del personaje
{
    int ataque1 = 0;

    switch (aux->clase)
    {
    case 1:
        /**guerrero*/
        ataque1=rand()%5+aux->atribPersonaje.fuerza;
        break;
    case 2:
        /**nigromante**/
        ataque1=rand()%5+aux->atribPersonaje.magia;
        break;
    case 3:
        /**asesino**/
        ataque1=rand()%10+aux->atribPersonaje.destreza;
        break;
    case 4:
        /**hechicero**/
        ataque1=rand()%5+aux->atribPersonaje.magia;
        break;
    }

    return ataque1;
}

int AtaqueCargado(stPersonaje *aux)                 ///Calcula el ataque cargado del personaje
{
    int ataque2 = 0;

    switch (aux->clase)
    {
    case 1:
        /**guerrero*/
        ataque2=rand()%5+aux->atribPersonaje.fuerza + aux->atribPersonaje.destreza;
        break;

    case 2:
        /**nigromante**/
        ataque2=rand()%5+aux->atribPersonaje.magia + aux->atribPersonaje.inteligencia;
        break;

    case 3:
        /**asesino**/
        ataque2=rand()%10+aux->atribPersonaje.destreza+15;
        break;
    case 4:
        /**hechicero**/
        ataque2=rand()%5+aux->atribPersonaje.inteligencia+aux->atribPersonaje.magia;
        break;

    }

    return ataque2;
}

void NuevoJuego(stPersonaje *player)                                        ///Funcion que organiza las batallas y demas funciones
{
    int correjuego = 1;
    char decisionguarda = 0;

    cascadaTexto("**CREAR PERSONAJE, SE INICIAN LAS STATS POR DEFECTO\n",0,whereY()+1);
    InicioDePersonaje(player);

    system("cls");

    cascadaTexto("*ESTE ES SU PERSONAJE*",0,5);
    MostrarPersonaje(player);
    continuar();
    system("cls");

    Introduccion(); /**te muestra la introduccion de la historia**/
    continuar();
    system("cls");


    while(correjuego == 1)
    {
        cascadaTexto("*EMPIEZA EL JUEGO*\n",0,6);
        continuar();
        correjuego = Jugar(player);
    }


    PausaLimpia();

    printf("DESEA GUARDAR EL PROGRESO? s/n\n");
    scanf("\n%c",&decisionguarda);

    if(decisionguarda =='s')
    {
        printf("*SE GUARDARA TU PROGRESO EN EL ARCHIVO\n");

        GuardarProgreso(player); /**escribe en el archivo**/

        printf("SE GUARDO EL PROGRESO, SE ESCRIBE EN MARCADORES.\n");
        continuar();
        guardarEnArchivoMarcadores(player);
    }
    else
    {
        printf("NO SE GUARDARA TU PROGRESO...\n");
    }

}
void mostrarSaves()
{
    FILE *pArchi = fopen(SAVE,"rb");
    stPersonaje aux;

    printf("CUENTA REGISTROS:    %d\n", cuentaRegistros());

    if(pArchi){

        while(fread(&aux,sizeof(stPersonaje),1,pArchi)>0){

            printf("------------------------\n");
            printf("Nombre:..........: %s\n",aux.nombre);
            printf("Nivel:..........: %d\n",aux.nivelDeJuego);

        }

        fclose(pArchi);
    }

}
int cuentaRegistros (){
    FILE * pArchi = fopen(SAVE,"rb");
    int i=0;

    if(pArchi){
        fseek(pArchi, 0,SEEK_END);
        i=ftell(pArchi)/sizeof(stPersonaje);
        fclose(pArchi);
    }
    return i;
}

void GuardarProgreso(stPersonaje *player)                                             ///Guarda el progreso realizado en la partida
{
    FILE* pfile = NULL;
    stPersonaje aux = *player;


    pfile = fopen(SAVE,"ab");

    if (pfile){
        fwrite(&aux,DIM,1,pfile);

        fclose(pfile);

    }else {
        printf("NO SE PUDO ABRIR EL ARCHIVO.\n");
    }
}

int Jugar(stPersonaje *player)                                                      ///Organiza los niveles de juego, y llama a los distintos niveles
{
    int inicioReloj = 0;
    int finReloj = 0;
    int tiempoJuego = 0;
    int flagGame = 1; /**mientras esto sea 1, el juego sigue, cuando muere o quiere salir
    se vuelve 0**/
    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char controlNivel = 0; /**cuando pierde, le pregunta si quiere volver a intentar*/
    char seguir ='s';

    fadeIN("Empiezas desde el nivel : ",0,7);
    gotoxy(whereX(),whereY());
    printf("%i",player->nivelDeJuego);
    continuar();


    while(flagGame == 1 && seguir=='s')
    {
        inicioReloj = clock();

        switch(player->nivelDeJuego)
        {

        case 1:

            pasaNivel = Nivel1(player); /**nivel1 devuelve un flag*/

            if(pasaNivel == 1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 1 !!!\n",0,linea1);
                player->nivelDeJuego = 2;

            }
            else
            {
                flagGame = 0; ///pierde la partida
            }

            break;

        case 2:

            pasaNivel = Nivel2(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 2 !!!\n",0,linea1);
                player->nivelDeJuego = 3;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }

            break;

        case 3:

            pasaNivel = Nivel3(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 3 !!!",0,linea1);
                player->nivelDeJuego = 4;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;

        case 4:
            pasaNivel = Nivel4(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 4 !!!",0,linea1);
                player->nivelDeJuego = 5;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;

        case 5:

            pasaNivel = Nivel5(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 5 !!!",0,linea1);
                player->nivelDeJuego = 6;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;
        case 6:
            pasaNivel = nergalYereshkigal(player,150,60);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 6 !!!\n",0,linea1);
                RecompensaPelea(player,2,150);
                player->nivelDeJuego = 7;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;
        case 7:
            pasaNivel = Nivel7(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 7 !!!",0,linea1);
                player->nivelDeJuego = 8;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;

        case 8:
            pasaNivel = Nivel8(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 8 !!!",0,linea1);
                player->nivelDeJuego = 9;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;
        case 9:

            pasaNivel = Nivel9(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 9 !!!",0,linea1);
                player->nivelDeJuego = 10;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;

        case 10:


            pasaNivel = peleaEsfigie(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 10 !!!\n",0,linea1);
                player->nivelDeJuego = 11;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;

        case 11:
             pasaNivel = Nivel11(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 11 !!!",0,linea1);
                player->nivelDeJuego = 12;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;

        case 12:
            pasaNivel = Nivel12(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 12 !!!",0,linea1);
                player->nivelDeJuego = 13;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;

        case 13:
            pasaNivel = Nivel13 (player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 13 !!!",0,linea1);
                player->nivelDeJuego = 14;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;

        case 14:
            pasaNivel = Nivel14(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 14, se viene un jefe!!!",0,linea1);
                player->nivelDeJuego = 15;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;

        case 15:
            pasaNivel = nivelBossN(player);

            if(pasaNivel==1)
            {
                limpiaLinea(0,linea1);
                cascadaTexto("Has superado el nivel 15!!!",0,linea1);
                continuar();
                system("cls");
                cascadaTexto("Has terminado el juego felicitaciones!!!",65,23);
                _sleep(500);
                player->nivelDeJuego = 16;
            }
            else
            {
                flagGame = 0; ///pierde la partida
            }
            break;
        }

        printf("Desea continuar? s/n");
        fflush(stdin);
        scanf("%c",&seguir);

        if(seguir!='s')
        {
            flagGame = 0;
        }

        finReloj = clock();

    }
    tiempoJuego = (int)(finReloj - inicioReloj)/CLOCKS_PER_SEC;

    player->tiempo = tiempoJuego;

    return flagGame;
}

int AtaqueBicho1(stPersonaje *aux,int danoMon)                                                  ///Calcula los ataques de los monstruos
{
    /**caso guerrero**/
    int ataque = 0;
    int resistenciapj = (int) aux->atribPersonaje.defensa/2;


    ataque = (rand()%10 + danoMon) - resistenciapj;

    return ataque;
}
int AtaqueBicho2(stPersonaje *aux,int danoMon)
{
    int ataque = 0;
    int resistenciapj = (int) (aux->atribPersonaje.defensa)/2;

    ataque = (rand()%7 + danoMon) - resistenciapj;

    return ataque;
}
int AtaqueBicho3(stPersonaje * aux, int danoMon)
{
    int ataque = 0;
    int resistenciapj = (int) aux->atribPersonaje.destreza;

    ataque = (rand()%7 + danoMon) - resistenciapj;

    return ataque;
}

int AtaqueBicho4(stPersonaje *aux, int danoMon)
{
    int ataque = 0;
    int resistenciapj = (int)aux->atribPersonaje.defensa;

    ataque = (rand()%7 + danoMon) - resistenciapj;

    return ataque;
}

void RecompensaPelea(stPersonaje * aux,int mejoraAtrib,int dinero)                                ///Le asigna las recompensas al personaje despues de terminado un nivel
{
    system("cls");
    fadeIN("ANTES DE LA RECOMPENSA\n",0,5);
    MostrarPersonaje(aux);
    int elijeatrib = 0;
    int decisionpuntos = 0;
    printf("\n\n**Recibes %i de dinero.\n",dinero);
    aux->inv.dinero = aux->inv.dinero + dinero;
    fadeIN("**Puedes mejorar atributos!!\n",0,whereY()+1);
    printf("1:\tMejorar fuerza\n2:\tMejorar inteligencia\n3:\tMejorar destreza\n4:\tMejorar magia\n5:\tMejorar defensa\n");

    do
    {
        printf("**Tienes %i puntos para gastar:\n",mejoraAtrib);
        fadeIN("En que deseas gastarlos?",0,whereY()+1);
        fadeIN("\t1.Fuerza     2.Inteligencia     3.Destreza     4.Magia     5.Defensa",0,whereY()+1);
        scanf("%i",&elijeatrib);

        switch(elijeatrib)
        {
        case 1:
            do
            {
                printf("Cuantos puntos quieres poner en fuerza? \n");
                scanf("%i",&decisionpuntos);
                if(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib)
                {
                    printf("No tienes esos puntos!\n");
                }
            }
            while(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib);
            mejoraAtrib = mejoraAtrib - decisionpuntos;
            aux->atribPersonaje.fuerza = aux->atribPersonaje.fuerza + decisionpuntos;
            decisionpuntos = 0;
            break;
        case 2:
            do
            {
                printf("Cuantos puntos quieres poner en inteligencia? \n");
                scanf("%i",&decisionpuntos);
                if(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib)
                {
                    printf("No tienes esos puntos!\n");
                }
            }
            while(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib);
            mejoraAtrib = mejoraAtrib - decisionpuntos;
            aux->atribPersonaje.inteligencia = aux->atribPersonaje.inteligencia + decisionpuntos;
            decisionpuntos = 0;
            break;
        case 3:
            do
            {
                printf("Cuantos puntos quieres poner en destreza? \n");
                scanf("%i",&decisionpuntos);
                if(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib)
                {
                    printf("No tienes esos puntos!\n");
                }
            }
            while(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib);
            mejoraAtrib = mejoraAtrib - decisionpuntos;
            aux->atribPersonaje.destreza = aux->atribPersonaje.destreza + decisionpuntos;
            decisionpuntos = 0;
            break;
        case 4:
            do
            {
                printf("Cuantos puntos quieres poner en magia? \n");
                scanf("%i",&decisionpuntos);
                if(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib)
                {
                    printf("No tienes esos puntos!\n");
                }
            }
            while(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib);
            mejoraAtrib = mejoraAtrib - decisionpuntos;
            aux->atribPersonaje.magia = aux->atribPersonaje.magia + decisionpuntos;
            decisionpuntos = 0;
            break;
        case 5:
            do
            {
                printf("Cuantos puntos quieres poner en defensa? \n");
                scanf("%i",&decisionpuntos);
                if(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib)
                {
                    printf("No tienes esos puntos!\n");
                }
            }
            while(decisionpuntos <= 0 || decisionpuntos > mejoraAtrib);
            mejoraAtrib = mejoraAtrib - decisionpuntos;
            aux->atribPersonaje.defensa = aux->atribPersonaje.defensa + decisionpuntos;
            decisionpuntos = 0;
            break;



        }
    }
    while (mejoraAtrib > 0);

    system("cls");
    fadeIN("DESPUES DE LA RECOMPENSA.",0,5);
    MostrarPersonaje(aux);
}

void CargaJuego(stPersonaje *player)                     ///Funcion que carga una partida, es invocada desde el main
{
    int correjuego = 0;
    char decisionguarda = 0;
    char opcionPartida[15];
    int encontroPartida = 0;
    char reintentar = 0;

    do
    {

        printf("Ingrese el nombre de su personaje:\n");
        fflush(stdin);
        gets(opcionPartida);

        encontroPartida = CargaDePersonaje(opcionPartida,player);

        if(encontroPartida == 0)
        {
            printf("Esa partida no existe. Buscar otra? s/n\n");
            scanf("\n%c",&reintentar);
        }
        else
        {
            reintentar = 'n';

            system("cls");
            MostrarPersonaje(player);

            PausaLimpia();

            do
            {
                cascadaTexto("*EMPIEZA EL JUEGO*\n",0,4);
                correjuego = Jugar(player);
            }
            while(correjuego == 1);

            PausaLimpia();

            printf("DESEA GUARDAR EL PROGRESO? s/n\n");
            scanf("\n%c",&decisionguarda);

            if(decisionguarda =='s')
            {
                printf("*SE GUARDARA TU PROGRESO EN EL ARCHIVO\n");

                GuardarProgreso(player); /**escribe en el archivo**/

                printf("SE GUARDO EL PROGRESO, SE ESCRIBE EN MARCADORES...\n");

                guardarEnArchivoMarcadores(player);

            }
            else
            {
                printf("NO SE GUARDARA TU PROGRESO...\n");
            }

        }

    }while(encontroPartida == 0 && reintentar == 's' );
}

int CargaDePersonaje(char nombreBuscar[],stPersonaje *player)
{
    FILE* pfile = NULL;
    stPersonaje aux;
    stPersonaje mayor;
    int flag = 0;
    pfile = fopen(SAVE,"rb");
    int cerrarArchivo = 0;
    int mayorN = 0;

    if(pfile!=NULL)
    {

        while(flag == 0 && fread(&aux,DIM,1,pfile) > 0)
        {

            if(strcmp(aux.nombre,nombreBuscar) == 0)
            {
                flag = 1;
                mayor = aux;
                mayorN = aux.nivelDeJuego;

            }
        }

        while( fread(&aux,DIM,1,pfile) > 0 )
        {

            if(strcmp(aux.nombre,nombreBuscar) == 0)
            {
                if(aux.nivelDeJuego > mayorN)
                {
                    mayor = aux;
                    mayorN = aux.nivelDeJuego;

                }
            }
        }
        *player = mayor;

        cerrarArchivo = fclose(pfile);

        if(cerrarArchivo!=0)
        {
            printf("NO SE PUDO CERRAR EL ARCHIVO.\n");
        }
    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO.\n");
    }

    return flag;
}

int Nivel1(stPersonaje *player)
{
    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,60,10,"Orco espadachin");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}
int Nivel2(stPersonaje *player)
{

    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,50,20,"Hechicero Calavera");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

int Nivel3(stPersonaje *player)
{
    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,70,30,"Espectro");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

int Nivel4(stPersonaje *player)
{
    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,50,35,"Lobo del Terror");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

int Nivel5(stPersonaje *player)
{
    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,100,40,"Dark Fact");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

int Nivel7(stPersonaje * player)
{

    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,100,50,"Moria Casan");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

int Nivel8 (stPersonaje * player)
{

    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,150,40,"Azukiarai");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

int Nivel9 (stPersonaje * player)
{

    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,100,55,"Serpiente Emplumada");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

int Nivel11 (stPersonaje * player)
{

    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,150,50,"Krasue");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}


int Nivel12 (stPersonaje * player)
{

    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,100,65,"Fafner");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

int Nivel13 (stPersonaje * player)
{

    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,150,60,"Tupilaq");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

int Nivel14 (stPersonaje * player)
{

    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char accedetienda = 0;

    pasaNivel = CicloPelea(player,100,65,"Jormundgander");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

/** \brief secuencia de acertijo esfige
 * \return devuelve pasaSinPelear 1 o 0
 */
int acertijoEsfigie ()
{
    int opRand=1;
    int opcion;
    int pasaSinPelear=0;

    opRand=rand()%6;
    system("cls");
    cascadaTexto("Te encuentras con La Esfinge, una critatura mitica de destruccion y de mala suerte...",0,3);
    cascadaTexto("No parece que quiera atacarte, al menos por ahora...",0,4);
    continuar();
    limpiaLinea(0,linea1);
    fadeInOut("Te he de presentar un acertijo para probar tu valia!",0,linea1);
    fadeInOut("Si eres digno y eliges correctamente de las opciones que te ofrecere, te dejare pasar",0,linea2);
    fadeInOut("Decide con cuidado...\n",0,linea3);

    continuar();
    limpiaLinea(0,linea1);

    switch(opRand)
    {
    case 1:
        cascadaTexto("Cual es la criatura que en la maniana camina en cuatro patas, al medio dia en dos y en la noche en tres?",0,9);
        _sleep(500);
        fadeIN("1. m-r-b-e-h-o     2. g-t-t-r-o-u-a      3. o-m-o-n ",0,11);
        scanf("%d",&opcion);
        pasaSinPelear=validacionYresultado(opcion,1);
        break;
    case 2:
        cascadaTexto("De noche llegan y no las llamaron. De dia no estan, pero no las robaron ",0,9);
        _sleep(500);
        fadeIN("1. c-h-a-z-e-l-u      2. e-l-l-t-s-a-e-r-s     3. n-u-l-a ",0,11);
        scanf("%d",&opcion);
        pasaSinPelear=validacionYresultado(opcion,2);
        break;
    case 3:
        cascadaTexto("Pequenio como un pulgar, en el aire soy ligero. Si no me has visto ya, descuida, porque me escucharas primero.",0,9);
        _sleep(500);
        fadeIN("1. j-o-s-a-h      2. b-j-a-e-a     3. l-i-o-c-r-i-b ",0,11);
        scanf("%d",&opcion);
        pasaSinPelear=validacionYresultado(opcion,3);
        break;
    case 4:
        cascadaTexto("Incansable y sin fatiga, va de colina en colina. Y aunque ni anda ni corre con piernas, solo hay frio tras su huida.",0,9);
        _sleep(500);
        fadeIN("1. e-l-l-t-s-a-e-r      2. l-o-s     3. b-r-a-c-a ",0,11);
        scanf("%d",&opcion);
        pasaSinPelear=validacionYresultado(opcion,2);
        break;
    case 5:
        cascadaTexto("Que nace, pero nunca llora, que discurre sin andar, desemboca sin hablar, que tiene lecho mas nunca reposa?",0,9);
        _sleep(500);
        fadeIN("1. b-r-o-l-a      2. o-g-l-a     3. o-r-i ",0,11);
        scanf("%d",&opcion);
        pasaSinPelear=validacionYresultado(opcion,3);
        break;
    case 6:
        cascadaTexto("Qué puede ser lleno mas nunca se vacia? ¿Qué cosa tira pero empujar, nunca?",0,9);
        _sleep(500);
        fadeIN("1. n-u-l-a      2. p-e-z-a-e-r-n-s-a     3. o-c-p-a ",0,11);
        scanf("%d",&opcion);
        pasaSinPelear=validacionYresultado(opcion,1);
        break;
    default:
        cascadaTexto("Cual es la criatura que en la mañana camina en cuatro patas, al medio día en dos y en la nocheen tres? ",0,9);
        _sleep(500);
        fadeIN("1. m-r-b-e-h-o     2. g-t-t-r-o-u-a      3. o-m-o-n   ",0,11);
        scanf("%d",&opcion);
        pasaSinPelear=validacionYresultado(opcion,1);
    }

    return pasaSinPelear;
}


/** \brief  valida entrada de 1 a 3
 *          devueve a secuencia o muestra resultado
 * \param   opcion elegida
 * \param   opcion correcta
 * \return resultado 1 o 0
 */
int validacionYresultado (int opcion,int correcta)
{
    int resultado=0;

    while(opcion>3 || opcion<1)
    {
        gotoxy(0,linea1);
        printf("No te he dado esa opcion, te dare otra oportunidad\n");
        printf(" Opcion 1.    Opcion 2.     Opcion 3. ");
        scanf("%d",&opcion);
    }
    if (opcion==correcta)
    {
        limpiaLinea(0,linea1);
        cascadaTexto("Has acertado! y por eso te has ganado mi favor...>",0,linea1);
        cascadaTexto("te dejare pasar>",0,linea2);
        cascadaTexto("Pero no creas que tu camino seguira siendo tan amable.>",0,linea3);
        resultado=1;
    }
    else
    {
        limpiaLinea(0,linea1);
        cascadaTexto("Dicen que la ignorancia es una bendicion...>",0,linea1);
        cascadaTexto("aunque a un alto costo...>",0,linea2);
        cascadaTexto("y por eso lo pagaras con tu vida!>",0,linea3);

    }
    continuar();
    limpiaLinea(0,linea1);

    return resultado;
}
int peleaEsfigie (stPersonaje * player)
{
    int pasaNivelSinPelear;
    int pasaNivel = 0;
    char accedetienda = 0;

    pasaNivelSinPelear=acertijoEsfigie();

    if(pasaNivelSinPelear==1)
    {
        pasaNivel=1;
        limpiaLinea(0,linea1);
        cascadaTexto("Puedes continuar con tu viaje...",0,linea1);
    }
    else
    {
        pasaNivel=CicloPelea(player,50,20,"La Efigie");
    }

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        accedetienda = 0;

        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }
}

/** \brief Secuencia de pelea boss NyE
 *  retorna flag pasa nivel 0 o 1
 * \param aux stPersonaje*
 * \param hpMon int
 * \param danoMon int
 * \return int
 *
 */
int nergalYereshkigal(stPersonaje *aux,int hpMon,int danoMon)
{
    char accedetienda;
    int i=0;
    int opc=0;
    int pasaNivel=0;
    int golpePj = 0;
    int golpeMon = 0;


    system("cls");
    fadeIN("*BATALLA*",65,3);
    cascadaTexto("Te encuentras con Nergal y Ereshkigal",0,whereY()+2); // Nergal y Ereshkigal   // se vence uno a la vez sin pausa
    cascadaTexto("Ereshkigall, es la regente del inframundo sumerio, junto con su consorte Nergal",0,whereY()+1);
    cascadaTexto("Tendras que pelear con ambos al mismo tiempo, por loq ue te conviene concentrar los golpes en uno a la vez",0,whereY()+1);
    cascadaTexto("El hp combinado de los dos es: ",0,whereY()+1);
    gotoxy(whereX()+1,whereY());
    printf("%d",hpMon);
    cascadaTexto("Sus ataques son de: ",0,whereY()+1);
    gotoxy(whereX()+1,whereY());
    printf("%d",danoMon);

    do
    {
        fadeIN("*BATALLA*",65,3);
        gotoxy(65,13);
        printf("HP PJ: %i",aux->hp);
        gotoxy(65,14);
        printf("MP PJ: %i",aux->mp);
        gotoxy(65,15);
        printf("HP MON: %i",hpMon);
        gotoxy(65,16);
        printf("PocionesHP:%i",aux->inv.pocioneshp);
        gotoxy(65,17);
        printf("PocionesMP: %i",aux->inv.pocionesmp);

        gotoxy(58,21);
        printf("1:\tAtaque normal");
        gotoxy(58,22);
        printf("2:\tAtaque Especial");
        gotoxy(58,23);
        printf("3:\tTomar pocion hp");
        gotoxy(58,24);
        printf("4:\tTomar pocion mp");
        gotoxy(58,25);

        fadeIN("Que accion desea realizar",0,linea1);
        scanf("%d",&opc);
        limpiaLinea(0,linea1);

        switch(opc)
        {
        case 1:
            ///ataque normal
            gotoxy(0,27);
            printf("%s le hace un ataque normal a %s ",aux->nombre,"Nergal y Ereshkigal");
            golpePj = AtaqueBasico(aux);
            golpe();
            printf("y le infringe un golpe de %i\n",golpePj);
            hpMon = hpMon - golpePj;
            break;
        case 2:
            ///ataque especial
            gotoxy(0,27);
            printf("Ataque cargado...\n");
            golpePj = AtaqueCargado(aux);
            golpe();
            printf("tu golpe es de %i\n",golpePj);
            hpMon = hpMon - golpePj;
            aux->mp = aux-> mp-10;
            break;
        case 3:
            ///pocion de vida
            gotoxy(0,27);
            printf("Tomas una pocion HP...\n");
            aux->hp = aux->hp + 25;
            aux->inv.pocioneshp = aux->inv.pocioneshp - 1;
            break;
        case 4:
            ///pocion de mana
            gotoxy(0,27);
            printf("Tomas una pocion MP...\n");
            aux->mp= aux->mp+ 25;
            aux->inv.pocionesmp = aux->inv.pocionesmp - 1;
            break;
        }


        if(i==0 && hpMon<50)
        {
            limpiaLinea(0,linea1);
            cascadaTexto("nooooooo!...Nergal!",0,linea1);
            cascadaTexto("...como un simple humano pudo...en nuestro propio reino",0,linea2);
            cascadaTexto("Moriras aunque sea la ultima cosa que haga!..",0,linea3);
            i=1;
            continuar();
            limpiaLinea(0,linea1);
        }

        ///turno del monstruo
        switch(aux->clase)
        {
        case 1: /**caso guerrero*/
            gotoxy(0,29);
            printf("El bicho ataca al guerero \n");
            golpeMon = AtaqueBicho1(aux,danoMon);
            golpe();
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        case 2: /**caso nigromante*/
            gotoxy(0,29);
            printf("El bicho ataca nigromante:\n");
            golpeMon = AtaqueBicho2(aux,danoMon);
            golpe();
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        case 3: /**caso asesino**/
            gotoxy(0,29);
            printf("El bicho ataca asesino:\n");
            golpeMon = AtaqueBicho3(aux,danoMon);
            golpe();
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        case 4: /**caso hechicero**/
            gotoxy(0,29);
            printf("El bicho ataca hechicero\n");
            golpeMon = AtaqueBicho4(aux,danoMon);
            golpe();
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        }
        continuar();
        limpiaLinea(0,27);
        limpiaLinea(0,28);
    }
    while ( (aux->hp > 0) && ( hpMon > 0 ));

    if(hpMon <= 0)
    {
        cascadaTexto("AHHHHH!...",0,linea1);
        cascadaTexto("Has derrotado a Nergal y Ereshkigal",0,linea2);
        pasaNivel = 1;
    }
    else
    {
        cascadaTexto("Has perdido!\n",0,linea1);
        cascadaTexto("Que un humano se atreviera a enfrentarnos merecia este castigo",0,linea2);
        cascadaTexto("...te tendremos un lugar reservado en el inframundo",0,linea3);
        pasaNivel = 0;
    }

    continuar();
    limpiaLinea(0,linea1);

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(aux,2,100);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(aux);
        }


        return 1; /**superó este nivel*/
    }
    else
    {
        return 0; /**no superó este nivel*/
    }

    continuar();
    limpiaLinea(0,linea1);

    return pasaNivel;
}


int cicloPeleaBossN (stPersonaje *aux,int hpMon,int danoMon,char nombreMon[])
{
    int opc=0;
    int pasaNivel=0;
    int golpePj = 0;
    int golpeMon = 0;

    printf("*BATALLA*\n");
    printf("Te encuentras con %s\n",nombreMon);
    printf("El cual es un zorro muy conocido de la mitologia japonesa\n");
    printf("No te la hara nada sencillo de ganarle, tienes la posibilidad de golperlo dos veces en cada turno, pero no por eso sera mas facil\n");
    printf("El da%co que te hara sera brutal pero podras resistirlo si estas preparado para el desafio\n",164);
    printf("Ten en cuenta las pociones de vida y la cantidad de vida y mana en todo momento\n",164);
    printf("Su hp es de: %i\n",hpMon);
    printf("Su ataque es de: %i\n\n",danoMon);

    do
    {
        fadeIN("*BATALLA*",65,3);
        gotoxy(65,13);
        printf("HP PJ: %i",aux->hp);
        gotoxy(65,14);
        printf("MP PJ: %i",aux->mp);
        gotoxy(65,15);
        printf("HP MON: %i",hpMon);
        gotoxy(65,16);
        printf("PocionesHP:%i",aux->inv.pocioneshp);
        gotoxy(65,17);
        printf("PocionesMP: %i",aux->inv.pocionesmp);

        gotoxy(58,21);
        printf("1:\tAtaque normal");
        gotoxy(58,22);
        printf("2:\tAtaque Especial");
        gotoxy(58,23);
        printf("3:\tTomar pocion hp");
        gotoxy(58,24);
        printf("4:\tTomar pocion mp");
        gotoxy(58,25);

        fadeIN("Que accion desea realizar",0,linea1);
        scanf("%d",&opc);
        limpiaLinea(0,linea1);

        switch(opc)
        {
        case 1:
            ///ataque normal
            printf("%s le hace un ataque normal a %s ",aux->nombre,nombreMon);
            golpePj = AtaqueBasico(aux);
            printf("y le infringe un golpe de %i\n",golpePj);
            hpMon = hpMon - golpePj;
            break;
        case 2:
            ///ataque especial
            printf("Ataque cargado...\n");
            golpePj = AtaqueCargado(aux);
            printf("tu golpe es de %i\n",golpePj);
            hpMon = hpMon - golpePj;
            aux->mp = aux-> mp-10;
            break;
        case 3:
            ///pocion de vida
            printf("Tomas una pocion HP...\n");
            aux->hp = aux->hp + 25;
            aux->inv.pocioneshp = aux->inv.pocioneshp - 1;
            break;
        case 4:
            ///pocion de mana
            printf("Tomas una pocion MP...\n");
            aux->mp= aux->mp+ 25;
            aux->inv.pocionesmp = aux->inv.pocionesmp - 1;
            break;
        }

        switch(opc)
        {
        case 1:
            ///ataque normal
            printf("%s le hace un ataque normal a %s ",aux->nombre,nombreMon);
            golpePj = AtaqueBasico(aux);
            printf("y le infringe un golpe de %i\n",golpePj);
            hpMon = hpMon - golpePj;
            break;
        case 2:
            ///ataque especial
            printf("Ataque cargado...\n");
            golpePj = AtaqueCargado(aux);
            printf("tu golpe es de %i\n",golpePj);
            hpMon = hpMon - golpePj;
            aux->mp = aux-> mp-10;
            break;
        case 3:
            ///pocion de vida
            printf("Tomas una pocion HP...\n");
            aux->hp = aux->hp + 25;
            aux->inv.pocioneshp = aux->inv.pocioneshp - 1;
            break;
        case 4:
            ///pocion de mana
            printf("Tomas una pocion MP...\n");
            aux->mp= aux->mp+ 25;
            aux->inv.pocionesmp = aux->inv.pocionesmp - 1;
            break;
        }

        ///turno del monstruo

        printf("%s ataca a %s ",nombreMon,aux->nombre);
        golpeMon = (rand()%7 + danoMon)-aux->atribPersonaje.defensa;
        aux->hp = aux->hp - golpeMon;
        printf("y le infringe un golpe de %i\n",golpeMon);

        PausaLimpia();

    }
    while ( (aux->hp > 0) && ( hpMon > 0 ));

    if(hpMon <= 0)
    {
        printf("Has derrotado a %s\n",nombreMon);
        pasaNivel = 1;
    }
    else
    {
        printf("Has perdido!\n");
        pasaNivel = 0;
    }


    return pasaNivel;
}
int nivelBossN (stPersonaje * player)
{
    int pasaNivel = 0; //valor desde ciclo de pelea, si recibe 1 pasa de nivel/
    char accedetienda = 0;

    pasaNivel = cicloPeleaBossN(player,300,200,"Kitsune");

    if(pasaNivel == 1)
    {
        printf("\n\nTu recompensa por la batalla:\n");
        RecompensaPelea(player,6,300);

        printf("\n\nDeseas acceder a la tienda? s/n\n");
        scanf("\n%c",&accedetienda);

        if(accedetienda=='s')
        {
            Tienda(player);
        }

        return pasaNivel; //superó este nivel*/
    }
    else
    {
        return pasaNivel; //no superó este nivel*/
    }
}



void Introduccion()
{
    fadeIN("\n\n\tDicese una vez de una aldea llamada Arcadia. La aldea Arcadia era un lugar pacifico\
 con mucha vegetacion y un paisaje arbolado,\n con un imponente volcan a lo lejos, el cual\
 por tradicion habia sido llamado Darius.Los habitantes de Arcadia amaban mucho \nsu tierra,\
 y su principal economia era el cultivo de maiz, el cual exportaban con las lejanas\
 aldeas, intercambiando bienes \ny monedas de plata. Todos vivian felices y pacificamente...",0,10);
    continuar();
    cascadaTexto("\t\t...sin embargo,un dia surgiria una amenaza para los habitantes de Arcadia.\n",0,18);
    hidecursor(0);

}

void AsignarNombres(char p[][30])
{
    strcpy(p[0],"1:\tPociones HP------");
    strcpy(p[1],"2:\tPociones MP------");
    strcpy(p[2],"3:\tEspada-----------");
    strcpy(p[3],"4:\tBaston-----------");
    strcpy(p[4],"5:\tCalavera---------");
    strcpy(p[5],"6:\tEscudo-----------");
    strcpy(p[6],"7:\tHacha------------");
    strcpy(p[7],"8:\tDaga-------------");
    strcpy(p[8],"9:\tSalir");
}

void AsignarArticulos(int a[])
{
    a[0] = 10; //hp
    a[1] = 10; //mp
    a[2] = 1; //espada
    a[3] = 1; //baston
    a[4] = 1; //calavera
    a[5] = 1; //escudo
    a[6] = 1; //hacha
    a[7] = 2; //daga
}

void MostrarMenuTienda(char p[][30],int a[])
{
    int i = 0;

    while(i < 8)
    {
        printf(" %s%i\n",p[i],a[i]);
        i++;
    }
    printf(" %s\n",p[i]);
}

/// FUNCIONES VISULAES

/** \brief Espera un input para continuar */
void continuar()
{
    int i=0;
    int x=continuarX;
    int y=linea1;
    int col[]= {8,7,15,7,8,0};
    do
    {
        hidecursor(0);
        gotoxy(x,y);
        color(col[i]);
        printf("Continuar");
        _sleep(100);
        i++;
        if(i>5)
        {
            i=0;
        }
    }
    while(!kbhit());
    getch();
    gotoxy(x,y);
    printf("          ");
    color(7);
}
/** \brief Limpia las 3 linea con espacios
 * \param x: deberia ir '0' por defecto
 * \param y: 'linea1' por defecto.
 */
void limpiaLinea (int x, int y)
{
    gotoxy(x,y);
    printf("                                                                                            ");
    gotoxy(x,y+1);
    printf("                                                                                            ");
    gotoxy(x,y+2);
    printf("                                                                                            ");
    color(7);
}
/** \brief fade IN de texto, texto queda, NO se borra
 * \param frase[] char TEXTO a presentar
 * \param x int        Eje X
 * \param y int        Eje Y
 */
void fadeIN(char frase[],int x,int y)
{
    int i=0;
    int col[]= {0,8,7,15,7};

    hidecursor(0);
    while(i<5)
    {
        color(col[i]);
        gotoxy(x,y);
        printf("%s",frase);
        _sleep(150);
        i++;
    }
    hidecursor(1);
}
void fadeINTimed(char frase[],int x,int y, int time)
{
    int i=0;
    int col[]= {0,8,7,15,7};

    hidecursor(0);
    while(i<5)
    {
        color(col[i]);
        gotoxy(x,y);
        printf("%s",frase);
        _sleep(time);
        i++;
    }
    hidecursor(1);
}
/** \brief fade OUT y regresa color normal de texto
 * \param frase[] char Texto MISMO que en fadeIN
 * \param x int        Eje X MISMO que en fadeIN
 * \param y int        Eje Y MISMO que en fadeIN
 */
void fadeOUT(char frase[],int x,int y)
{
    int i=0;
    int col[]= {15,7,8,0};

    hidecursor(0);
    while(i<4)
    {
        color(col[i]);
        gotoxy(x,y);
        printf("%s",frase);
        _sleep(150);
        i++;
    }
    hidecursor(1);
    color(7);
}
/** \brief Presenta texto en fade in, espera un input, fade out
 * \param  eje X: deberia ser '0' por defecto
 * \param  eje y: Numero de linea donde imprimir
 */
void fadeInOut (char frase[],int x,int y)
{
    fadeIN(frase,x,y);
    if(getch())
        fadeOUT(frase,x,y);
}

/** \brief Imprime caracteres por uno en cascada.
 *          cuando encuentra el '/0' corta el ciclo
 * \param texto[] char: Cadena de texto a mostrar
 * \param x int:        eje X siempre ira '0' salvo que se quiera alinear al centro
 * \param y int:        ira linea1,linea2,linea3 o cualquier linea del eje Y para imprimir
 */
void cascadaTexto(char texto[], int x,int y)
{
    int i=0;
    int flag=0;

    gotoxy(x,y);
    while((texto[i]!= '\0' &&flag==0))
    {
        color(15);
        printf("%c",texto[i]);
        _sleep(70);
        gotoxy(whereX()-1,y);
        color(7);
        printf("%c",texto[i]);
        i++;
        if(kbhit())
        {
            getch();
            gotoxy(x,y);
            printf("%s",texto);
            flag=1;
        }
    }
    if(getch);
}

/** \brief Presenta pantalla de inicio hacia el menu */
void pantallaInicio ()
{
    int i=0;
    hidecursor(0);
    while(i<1)
    {
        system("COLOR 07");
        _sleep(50);
        system("COLOR 87");
        _sleep(50);
        system("COLOR 77");
        _sleep(50);
        system("COLOR F7");
        _sleep(50);
        system("COLOR 77");
        _sleep(50);
        system("COLOR 87");
        _sleep(50);
        system("COLOR 07");
        i++;
    }
    Sleep(1000);
    cascadaTexto("LA LEYENDA DE C",65,23);
    Sleep(1000);
    continuar();
    fadeOUT("LA LEYENDA DE C",65,23);
    system("cls");
    Sleep(500);
    fadeInOut("\nBlanco Santiago\nFernandez Mariano\nSanjurjo Juan Manuel",5,35);
    system("cls");
}

void fadeINPantalla(char frase[],int x, int y)
{
    int i=0;
    hidecursor(0);
    system("COLOR 00");
    gotoxy(x,y);
    printf("%s",frase);
    _sleep(150);
    system("COLOR 08");
    gotoxy(x,y);
    printf("%s",frase);
    _sleep(150);
    system("COLOR 07");
    gotoxy(x,y);
    printf("%s",frase);
    _sleep(150);

    hidecursor(1);
}

/** \brief visual golpe con tono a 90hz*/
void golpe()
{
    int i=0;
    while(i<2)
    {
        system("COLOR F4");
        _sleep(50);
        system("COLOR 07");
        _sleep(50);
        i++;
    }
    Beep(90,500);
    system("COLOR 07");
}

void PausaLimpia()
{
    fflush(stdin);
    getchar();
    system("cls");
}

///Funciones de archivo Marcadores
void guardarEnArchivoMarcadores(stPersonaje *aux)
{
    FILE * archi = fopen(MARCADORES,"ab");
    int cerrarArchivo = 0;
    int escribirArchivo = 0;
    stMarcador Aguardar;

    if( archi!= NULL)
    {
        strcpy(Aguardar.clase,aux->tipoClase);

        strcpy(Aguardar.nombre,aux->nombre);

        Aguardar.nivelMaximo = aux->nivelDeJuego;

        Aguardar.invMarcador = aux->inv;

        Aguardar.tiempoJuego = aux->tiempo;

        escribirArchivo = fwrite(&Aguardar,DIMMAR,1,archi);

        if(escribirArchivo < 1)
            printf("NO SE PUDO ESCRIBIR EN EL ARCHIVO EN GuardarMarcadores\n");

        cerrarArchivo = fclose(archi);

        if(cerrarArchivo!= 0)
            printf("NO SE PUDO CERRAR EL ARCHIVO EN GuardarMarcadores\n");
    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO MARCADORES EN GuardarEnMarcadores\n");
    }

}

void mostrarMarcadores()
{

    FILE * archi = fopen(MARCADORES,"rb");
    int opc = 0;
    char nombreBuscar[30];
    int cerrarArchivo = 0;
    stMarcador aux;
    char opcion = 0;

    printf("1:\tMostrar todos los marcadores\n2:\tBuscar por nombre\n");
    scanf("%i",&opc);

    if(archi!=NULL)
    {

        switch (opc)
        {
        case 1:

            while(fread(&aux,DIMMAR,1,archi)>0)
            {
                mostrarUnMarcador(aux);
            }
            break;

        case 2:
            do
            {
                printf("Ingrese el nombre que desea buscar\n");
                fflush(stdin);
                gets(nombreBuscar);

                while(fread(&aux,DIMMAR,1,archi)>0)
                {
                    if(strcmp(aux.nombre,nombreBuscar) == 0)
                    {
                        mostrarUnMarcador(aux);
                        opcion = 'n';
                    }
                    else
                    {
                        printf("No se encontro un personaje con ese nombre!\nBuscar otro? s/n\n");
                        scanf("\n%c",&opcion);
                    }
                }
            }
            while(opcion == 's');

            break;
        }

        cerrarArchivo = fclose(archi);

        if(cerrarArchivo != 0)
            printf("NO SE PUDO CERRAR EL ARCHIVO MARCADORES EN MostrarMarcadores\n");
    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO MARCADORES EN MostrarMarcadores\n");
    }
}

void mostrarUnMarcador(stMarcador aux)
{
    printf("------------------------------------------\n");
    printf("Nombre: %s\n",aux.nombre);
    printf("Clase: %s\n",aux.clase);
    printf("Arma: %s\n",aux.invMarcador.arma);
    printf("Dinero: %d\n",aux.invMarcador.dinero);
    printf("Escudo: %s\n",aux.invMarcador.escudo);
    printf("Pociones hp: %d\n",aux.invMarcador.pocioneshp);
    printf("Pociones mp: %d\n",aux.invMarcador.pocionesmp);
    printf("Nivel maximo superado: %d\n",aux.nivelMaximo);
}

void CalcularTiempoJuego(int tiempo)
{
    int horas = 0;
    int minutos = 0;

    if(tiempo > 3600)
    {
        horas = tiempo/3600;

        minutos = (tiempo - 3600*horas)/60;

        printf("Has jugado %i horas y %i minutos.\n",horas,minutos);
    }
    else if(tiempo > 60)
    {
        minutos = tiempo/60;
        printf("Has jugado %i minutos.\n",minutos);
    }
    else
    {
        printf("Has jugado %i segundos.\n",tiempo);
    }
}
