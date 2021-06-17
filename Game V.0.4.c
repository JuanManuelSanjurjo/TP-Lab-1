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
    char genero[15];  /**Elegido por el jugador*/
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
    long Puntaje; /**entero largo para almacenar cifras como 1123241**/
    int nivelMaximo;
} stMarcador;

typedef struct
{
    stPersonaje pj; /**guarda el pj de la partida, no se va a mostrar**/
    char nombre[15];
    char clase[15];
    int nivelJuego;
    int validez; /**variable flag por si se desea eliminar un registro, no se va a mostrar**/
} stGuardado;

/**Constantes tamaño de estructuras**/
const int DIM = sizeof(stPersonaje); /** para las funciones de archivo**/
const int DIMG = sizeof(stGuardado); /** para las funciones de archivo **/
const int DIMMAR = sizeof(stMarcador); /**para las funciones de archivo**/

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
void MuestraArchivoSave(); /**Muestra todo el archivo save para ver los registros (partidas)*/
void MuestraUnaPartida(stGuardado aux); /**Muestra las partidas, una parte de stPersonaje*/
void CargaDePersonaje(stPersonaje *player,int registroPartida); /**carga un registro para copiar a stPersonaje*/
stPersonaje CargarDatos(stGuardado aux);
void MuestraArchivoMarcadores(); /**abre el archivo y muestra una tabla de posiciones**/
void MuestraUnMarcador(stMarcador aux);
/**juego**/
void GuardarProgreso(stPersonaje *player); /**guarda progreso en el archivo*/
int CuentaRegistros(); /**para guardar la partida**/
void CrearNuevoSlot(stPersonaje * player); /**para guardar la partida*/
void GuardarSiguienteSlot(stPersonaje *player); /**para guardar la partida*/
stGuardado EscribirDatos(stPersonaje *player); /**para guardar la partida*/
int Jugar(stPersonaje *player); /**va a ser llamada ya por NuevoJuego o CargarJuego, contiene todos los
niveles, la tienda y todo el gameplay. Devuelve flag para saber si sigue jugando o murio*/
int CicloPelea(stPersonaje *aux, int hpMon, int danoMon, char nombreMon[]);
void RecompensaPelea(stPersonaje *aux, int mejoraAtrib, int dinero); /**sube atrib y el dinero*/
/**ataque pj**/
int AtaqueBasico(stPersonaje *aux);
int AtaqueCargado(stPersonaje *aux);
/**ataque bicho**/
int AtaqueBicho1(stPersonaje *aux,int danoMon);
int AtaqueBicho2(stPersonaje * aux,int danoMon);
int AtaqueBicho3(stPersonaje *aux,int danoMon);
int AtaqueBicho4(stPersonaje *aux,int danoMon);

/**para hacer*/
/**marcadores*/

int main()
{
    SetConsoleTitle("THE LEYEND OF C");
    srand(time(NULL));

    stPersonaje partida; /**estructura de la partida: se modifica segun la save o se crea
    una nueva si no hay save anterior*/

    int controlmenu = 0;

    printf("1:\tNUEVO JUEGO\n2:\tCARGAR JUEGO\n3:\tMARCADORES\n4:\tSALIR\n");
    scanf("%i",&controlmenu);

    switch(controlmenu)
    {
    case 1:
        printf("**ELEJISTE EMPEZAR UNA PARTIDA NUEVA.\n");
        NuevoJuego(&partida);
        break;
    case 2:
        printf("**ELEJISTE CARGAR UNA PARTIDA ANTERIOR.\n");
        CargaJuego(&partida);
        break;
    case 3:
        MuestraArchivoMarcadores();
        break;
    }


    return 0;
}

void InicioDePersonaje(stPersonaje *player)
{
    /**crea una nueva save**/

    FILE * archi = NULL;
    int cerroarchivo = 0;
    int escribioarchivo = 0;

    archi = fopen(SAVE,"wb");

    if (archi!=NULL)
    {
        printf("\nIngrese su nombre\n");
        fflush(stdin);
        gets(player->nombre);

        printf("Indica el genero con el cual te identificas\n");
        fflush(stdin);
        gets(player->genero);

        printf("Se le presentaran las siguientes clases las cual puede elegir\n\n");

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

        if(player->clase == 1 || player->clase == 3)
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

        escribioarchivo = fwrite(&player,DIM,1,archi);

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


    printf("1. Guerrero: Eres el elegido para ser el salvador de la aldea, llevas contigo la bendicion y la fuerza de la luz\n para combatir lo que acecha en las tinieblas. Clasico combatiente perfectamente ideado para el combate directo,\ncon sus propias capacidades de aguante que hacen trizas a los enemigos en un abrir y cerrar de ojos.\nSus armas afines son las espadas y las hachas. Toda la fe de la aldea esta puesta en este valeroso guerrero.\n\n");
    printf("2. Nigromante: Tras la invasion de las criaturas en la aldea, en lugar de combatirlas, aprendiste sus artes oscuras.\n Usas el poder de la mismisima muerte y destruccion para sacarle todo el provecho posible, causar estragos y \ntemores a los enemigos, incluso utilizas sus cadaveres para invocar en siervos en su lugar. \nSu arma afin es el baculo y el baston. La gente le teme a los nigromantes.\n\n");
    printf("3. Asesino: No defiendes a la aldea ni a las criaturas del mal. Haces tu propio camino y te preocupas por tus intereses\n.Expertos verdugos escurridizos que se tienen que exponer normalmente bastante, pero con una brutal con habilidad\n de deshacerse de los enemigos en un abrir y cerrar de ojos. Provocando muertes por doquier. Su arma afin es \nla daga yo el cuchillo.\n\n");
    printf("4. Hechicero: Aprendiste las artes de la hechiceria para defender a tu aldea. Siempre causaran estragos elementales entre\n las filas enemigas. Sus hechizos elementales van desde ataques devastadores a protectores e incluso curadores,\nsiempre dispuestos a entrar en pleno fragor de la batalla. El hechicero utiliza el poder de su inteligencia\n en carencia de su fuerza. Sus armas afines son el baculo y el baston.\n\n");

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
            /**todas las clases pueden comprar pociones**/
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
            /**todas las clases pueden comprar pociones*/
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
            if(player->clase == 1)
            {
                if(hayespada == 1)
                {
                    hayespada = CompraEspada(player,hayespada);
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
        case 4:
            if(player->clase == 4)
            {
                if(haybaston = 1)
                {
                    haybaston = CompraBaston(player,haybaston);
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
        case 5:
            if(player->clase == 2)
            {
                if(haycalavera = 1)
                {
                    haycalavera = CompraCalavera(player,haycalavera);
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
        case 6:
            if(player->clase == 1 || player->clase == 4)
            {
                if(hayescudo = 1)
                {
                    hayescudo = CompraEscudo(player,hayescudo);
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
        case 7:
            if(player->clase == 1)
            {
                if(hayhacha = 1)
                {
                    hayhacha = CompraHacha(player,hayhacha);
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
        case 8:
            if(player->clase == 3)
            {
                if(haydaga > 0 )
                {
                    haydaga = CompraDaga(player,haydaga);
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

void MostrarPersonaje(stPersonaje *player)
{
    printf("Tu personaje es:\n");

    printf("\n----------------------------------------------------------\n");
    printf("\nDatos del jugador");
    printf("\nNombre:..............:\t\t%s",player->nombre);
    printf("\nGenero:..............:\t\t%s",player->genero);
    printf("\nClase:...............:\t\t%s",player->tipoClase);
    printf("\nVida:................:\t\t%d",player->hp);
    printf("\nMana:................:\t\t%d",player->mp);
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
    printf("\nEscudo:..............:\t\t[%s]",player->inv.escudo);
    printf("\nPociones de vida:....:\t\t[%d]",player->inv.pocioneshp);
    printf("\nPociones de mana:....:\t\t[%d]",player->inv.pocionesmp);
    printf("\nDinero:..............:\t\t[%d]",player->inv.dinero);
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
    int golpePj = 0;
    int golpeMon = 0;

    printf("te encontraste con %s\n",nombreMon);
    printf("Su hp es de: %i\n",hpMon);
    printf("Su ataque es de: %i\n",danoMon);

    do
    {
        printf("HP PJ: %i\nMP PJ: %i\nHP MON: %i\nPocionesHP:%i\nPocionesMP: %i\n",aux->hp,aux->mp,hpMon,aux->inv.pocioneshp,aux->inv.pocionesmp);
        printf("1:\tAtaque normal\n2:\tAtaque Especial\n3:\tTomar pocion hp\n4:\tTomar pocion mp\n");
        scanf("%d",&opc);
        switch(opc)
        {
        case 1:
            ///ataque normal
            printf("Elejiste ataque normal.\n");
            golpePj = AtaqueBasico(aux);
            printf("tu golpe es de: %i\n",golpePj);
            hpMon = hpMon - golpePj;
            break;
        case 2:
            ///ataque especial
            printf("Elejiste ataque cargado.\n");
            golpePj = AtaqueCargado(aux);
            printf("tu golpe es de %i\n",golpePj);
            hpMon = hpMon - golpePj;
            aux->mp = aux-> mp-10;
            break;
        case 3:
            ///pocion de vida
            printf("Elejiste tomar una pocion HP.\n");
            aux->hp = aux->hp + 25;
            printf("Tu vida es: %i\n",aux->hp);
            aux->inv.pocioneshp = aux->inv.pocioneshp - 1;
            break;
        case 4:
            ///pocion de mana
            printf("Elejiste tomar una pocion MP.\n");
            aux->mp= aux->mp+ 25;
            printf("Tu mana es: %i\n",aux->mp);
            aux->inv.pocionesmp = aux->inv.pocionesmp - 1;
            break;
        }

        ///turno del monstruo
        switch(aux->clase)
        {
        case 1: /**caso guerrero*/
            printf("El bicho ataca guerrero:\n");
            golpeMon = AtaqueBicho1(aux,danoMon);
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        case 2: /**caso nigromante*/
            printf("El bicho ataca nigromante:\n");
            golpeMon = AtaqueBicho2(aux,danoMon);
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        case 3: /**caso asesino**/
            printf("El bicho ataca asesino:\n");
            golpeMon = AtaqueBicho3(aux,danoMon);
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        case 4: /**caso hechicero**/
            printf("El bicho ataca hechicero:n");
            golpeMon = AtaqueBicho4(aux,danoMon);
            aux->hp = aux->hp - golpeMon;
            printf("El bicho te infringe %i\n",golpeMon);
            break;
        }

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

int AtaqueBasico(stPersonaje *aux)
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

int AtaqueCargado(stPersonaje *aux)
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

void NuevoJuego(stPersonaje *player)
{
    int correjuego = 0;
    char decisionguarda = 0;
    printf("**CREAR PERSONAJE, SE INICIAN LAS STATS POR DEFECTO\n");
    InicioDePersonaje(player);
    printf("*ESTE ES SU PERSONAJE*");
    MostrarPersonaje(player);

    do
    {
        printf("*EMPIEZA EL JUEGO*\n");
        correjuego = Jugar(player);
    }
    while(correjuego == 1);

    printf("DESEA GUARDAR EL PROGRESO? s/n\n");
    scanf("\n%c",&decisionguarda);

    if(decisionguarda =='s')
    {
        printf("**SI\n");

        GuardarProgreso(player); /**escribe en el archivo**/

        /*printf("SE GUARDO EL PROGRESO, SE ESCRIBE EN MARCADORES.\n");

        *GuardarMarcadores(&player);*/
    }

}

void GuardarProgreso(stPersonaje *player)
{
    /**el maximo de slots para guardar la partida son 3*/
    int haypartidas = 0;

    haypartidas = CuentaRegistros(); /**segun el numero de registros del archivo save*/

    if(haypartidas == 0) /**si no hay registros abre el archivo save  en modo wb y guarda*/
    {
        printf("Se guardara en el primer slot.\n");
        CrearNuevoSlot(player);
    }
    else if(haypartidas > 0 && haypartidas < 3)
    {
        printf("Se guardara en el ultimo slot disponible.\n");
        GuardarSiguienteSlot(player); /**si ya hay registros abre el archivo en ab y guarda*/
    }
}

int Jugar(stPersonaje *player)
{
    int flagGame = 1; /**mientras esto sea 1, el juego sigue, cuando muere o quiere salir
    se vuelve 0**/
    int nivelJuego = 1; /**empieza desde el nivel 1*/
    int pasaNivel = 0; /**valor desde ciclo de pelea, si recibe 1 pasa de nivel*/
    char controlNivel = 0; /**cuando pierde, le pregunta si quiere volver a intentar*/
    char accedetienda = 0;

    while(flagGame == 1)
    {
        nivelJuego = 1;
        switch(nivelJuego)
        {
        case 1:
            do
            {
                printf("NIVEL 1\n");
                pasaNivel = CicloPelea(player,60,10,"Orco espadachin");

                if(pasaNivel == 1)
                {
                    nivelJuego = 2;
                    printf("\n\nTu recompensa por la batalla:\n");
                    RecompensaPelea(player,2,100);
                    printf("\n\nDeseas acceder a la tienda? s/n\n");
                    scanf("\n%c",&accedetienda);
                    if(accedetienda=='s')
                    {
                        Tienda(player);
                    }
                    accedetienda = 0;
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

        case 2:

            do
            {
                /**este bicho tiene menos vida y más ataque*/
                printf("NIVEL 2\n");
                pasaNivel = CicloPelea(player,50,20,"Hechicero Calavera");

                if(pasaNivel == 1)
                {
                    nivelJuego = 2;
                    printf("\n\nTu recompensa por la batalla:\n");
                    RecompensaPelea(player,2,100);
                    printf("\n\nDeseas acceder a la tienda? s/n\n");
                    scanf("\n%c",&accedetienda);
                    if(accedetienda=='s')
                    {
                        Tienda(player);
                    }
                    accedetienda = 0;
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

        case 3:

            do
            {
                printf("NIVEL 3\n");

                pasaNivel = CicloPelea(player,70,30,"Espectro");

                if(pasaNivel == 1)
                {
                    nivelJuego = 2;
                    printf("\n\nTu recompensa por la batalla:\n");
                    RecompensaPelea(player,2,100);
                    printf("\n\nDeseas acceder a la tienda? s/n\n");
                    scanf("\n%c",&accedetienda);
                    if(accedetienda=='s')
                    {
                        Tienda(player);
                    }
                    accedetienda = 0;
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

        case 4:
            do
            {
                printf("NIVEL 1\n");

                pasaNivel = CicloPelea(player,50,35,"Lobo del terror");

                if(pasaNivel == 1)
                {
                    nivelJuego = 2;
                    printf("\n\nTu recompensa por la batalla:\n");
                    RecompensaPelea(player,2,100);
                    printf("\n\nDeseas acceder a la tienda? s/n\n");
                    scanf("\n%c",&accedetienda);
                    if(accedetienda=='s')
                    {
                        Tienda(player);
                    }
                    accedetienda = 0;
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

        case 5:

            do
            {
                printf("NIVEL 1\n");

                pasaNivel = CicloPelea(player,100,100,"Dark Fact");

                if(pasaNivel == 1)
                {
                    nivelJuego = 2;
                    printf("\n\nHas derrotado al boss de este area! Tu recompensa por la batalla:\n");
                    RecompensaPelea(player,4,250);
                    printf("\n\nDeseas acceder a la tienda? s/n\n");
                    scanf("\n%c",&accedetienda);
                    if(accedetienda=='s')
                    {
                        Tienda(player);
                    }
                    accedetienda = 0;
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

        flagGame = 0;
    }

    return flagGame;
}

int AtaqueBicho1(stPersonaje *aux,int danoMon)
{
    /**caso guerrero**/
    int ataque = 0;
    int resistenciapj = (int) aux->atribPersonaje.defensa/2;

    ataque = (rand()%7 + danoMon) - resistenciapj;

    return ataque;
}
int AtaqueBicho2(stPersonaje *aux,int danoMon)
{
    int ataque = 0;
    int resistenciapj = (int)aux->atribPersonaje.defensa/2;

    ataque = (rand()%7 + danoMon) - resistenciapj;

    return ataque;
}
int AtaqueBicho3(stPersonaje * aux, int danoMon)
{
    int ataque = 0;
    int resistenciapj = (int)aux->atribPersonaje.destreza;

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

void RecompensaPelea(stPersonaje * aux,int mejoraAtrib,int dinero)
{
    printf("ANTES DE LA RECOMPENSA\n");
    MostrarPersonaje(aux);
    int elijeatrib = 0;
    int decisionpuntos = 0;
    printf("**Recibes %i de dinero.\n",dinero);
    aux->inv.dinero = aux->inv.dinero + dinero;
    printf("**Puedes mejorar atributos!!\n");
    printf("1:\tMejorar fuerza\n2:\tMejorar inteligencia\n3:\tMejorar destreza\n4:\tMejorar magia\n5:\tMejorar defensa\n");

    do
    {
        printf("**Tienes %i puntos para gastar:\n",mejoraAtrib);
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

    printf("DESPUES DE LA RECOMPENSA.\n");
    MostrarPersonaje(aux);
}

void CargaJuego(stPersonaje*player)
{
    int opcionpartida = 0;
    printf("Contenidos del archivo SAVE.\n");
    MuestraArchivoSave();
    printf("Selecciona la partida para cargar.\n");
    scanf("%i",&opcionpartida);
    CargaDePersonaje(player,opcionpartida);
    printf("TRAS LA LECTURA DEL ARCHIVO, ESTE ES TU PERSONAJE.\n");
    MostrarPersonaje(player);
}

void MuestraArchivoSave()
{
    FILE* pfile = NULL;
    stGuardado aux;
    int cerroarchivo = 0;
    pfile = fopen(SAVE,"rb");

    if(pfile!=NULL)
    {
        rewind(pfile);

        while(fread(&aux,DIMG,1,pfile) > 0  )
        {
            MuestraUnaPartida(aux);
        }

        cerroarchivo = fclose(pfile);

        if(cerroarchivo!=0)
        {
            printf("NO SE PUDO CERRAR EL ARCHIVO EN MuestraArchivo()\n");
        }

    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO SAVE EN MuestraArchivo().\n");
    }
}

void MuestraUnaPartida(stGuardado aux)
{
    printf("*********************************************************\n");
    printf("Nombre.................: %s\n",aux.nombre);
    printf("Clase..................: %s\n",aux.clase);
    printf("Nivel..................: %i\n",aux.nivelJuego);
    printf("*********************************************************\n");
}

int CuentaRegistros()
{
    FILE* pfile = NULL;
    int cerrarArchivo = 0;
    int bytesArchivo = 0; /**nos va a decir los bytes del archivo*/
    int numRegistros = 0; /**cantidad de registros**/

    pfile = fopen(SAVE,"rb");

    if(pfile!=NULL)
    {
        rewind(pfile);

        bytesArchivo = ftell(pfile);

        if(bytesArchivo > 0)
        {
            numRegistros = bytesArchivo/DIMG;
        }

        cerrarArchivo = fclose(pfile);

        if(cerrarArchivo!=0)
        {
            printf("NO SE PUDO CERRAR EL ARCHIVO EN CuentaRegistros()\n");
        }
    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO EN CuentaRegistros()\n");
    }

    return numRegistros;
}

void CrearNuevoSlot(stPersonaje *player)
{
    FILE* pfile = NULL;
    pfile = fopen(SAVE,"wb");
    int cerrarArchivo = 0;
    int escribirArchivo = 0;
    stGuardado aux;

    if(pfile!=NULL)
    {
        aux = EscribirDatos(player);

        escribirArchivo = fwrite(&aux,DIMG,1,pfile);

        if(escribirArchivo < 1)
        {
            printf("ERROR AL ESCRIBIR EN ARCHIVO EN CrearNuevoSlot()\n");
        }

        cerrarArchivo = fclose(pfile);

        if(cerrarArchivo!=0)
        {
            printf("NO SE PUDO ABRIR EL ARCHIVO EN CrearNuevoSlot()\n");
        }

    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO EN CrearNuevoSlot()\n");
    }
}

void GuardarSiguienteSlot(stPersonaje *player)
{
    FILE* pfile = NULL;
    int cerrarArchivo = 0;
    int escribirArchivo = 0;
    pfile = fopen(SAVE,"ab");
    stGuardado aux;

    if(pfile!=NULL)
    {
        aux = EscribirDatos(player);

        escribirArchivo = fwrite(&aux,DIMG,1,pfile);

        if(escribirArchivo < 1)
        {
            printf("NO SE PUDO ESCRIBIR EN EL ARCHIVO EN GuardarSiguienteSlot()\n");
        }

        cerrarArchivo = fclose(pfile);

        if(cerrarArchivo!=0)
        {
            printf("NO SE PUEDE CERRAR EL ARCHIVO EN GuardarSiguienteSlot()\n");
        }
    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO EN GuardarSiguienteSlot()\n");
    }
}

stGuardado EscribirDatos(stPersonaje *player)
{
    stGuardado aux;

    aux.pj = *player;

    strcpy(aux.nombre,player->nombre);

    strcpy(aux.clase,player->tipoClase);

    aux.nivelJuego = player->nivelDeJuego;

    aux.validez = 1;

    return aux;
}

void CargaDePersonaje(stPersonaje * player,int registroPartida)
{
    FILE* pfile = NULL;
    stGuardado aux;
    int cerrarArchivo = 0;
    int leerArchivo = 0;
    int limitesArchivo = 0; /** para fseek si devuelve distinto de cero, hubo error*/
    pfile = fopen(SAVE,"r+b");

    if(pfile!=NULL)
    {
        /**leer desde el principio del archivo**/
        rewind(pfile);
        /**bajar hasta la partida que indicó el jugador por parametro**/
        limitesArchivo = fseek(pfile,DIMG*(registroPartida - 1),SEEK_SET);
        /**leer los datos de ese registro stGuardado**/
        leerArchivo = fread(&aux,DIMG,1,pfile);
        /**copiar los datos de ese registro stGuardado a stPersonaje**/
        *player = CargarDatos(aux);

        if(leerArchivo < 1)
        {
            printf("NO SE PUDO LEER EL ARCHIVO EN CargaDePersonaje()\n");
        }

        cerrarArchivo = fclose(pfile);

        if(cerrarArchivo!= 0)
        {
            printf("NO SE PUDO CERRAR EL ARCHIVO EN CargaDePersonaje()\n");
        }
    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO EN CargaDePersonaje()\n");
    }
}

stPersonaje CargarDatos(stGuardado aux)
{
    stPersonaje aux2;

    aux2 = aux.pj;

    return aux2;
}

void MuestraArchivoMarcadores()
{
    FILE*  pfile = NULL;
    stMarcador aux;
    int cerrarArchivo = 0;
    int bytesArchivo = 0;
    pfile = fopen(MARCADORES,"rb");

    if(pfile!=NULL)
    {

        bytesArchivo = ftell(pfile);

        if(bytesArchivo == 0)
        {
            printf("NO HAY DATOS PARA LEER EN Marcadores.dat");
        }
        else
        {
            rewind(pfile);

            while( (fread(&aux,DIMMAR,1,pfile)) > 0 )
            {
                MuestraUnMarcador(aux);
            }

        }

        cerrarArchivo = fclose(pfile);

        if(cerrarArchivo!=0)
        {
            printf("NO SE PUDO CERRAR EL ARCHIVO EN MuestraMarcadores()\n");
        }
    }
    else
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO EN MuestraMarcadores()\n");
    }

}
void MuestraUnMarcador(stMarcador aux)
{
    /**no veo aun como relacionar el idPersonaje con esta funcion**/
    printf("Puntaje maximo: %l\n",aux.Puntaje);
    printf("Nivel maximo superado: %i\n",aux.nivelMaximo);
}
