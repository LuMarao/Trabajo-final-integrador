#include <stdio.h>
#include "artemis.h"

/* 1 = colores ANSI, 0 = sin colores (ponelo en 0 si ves simbolos raros) */
#define USAR_COLOR 1

#if USAR_COLOR
    #define C_RESET   "\033[0m"
    #define C_TITULO  "\033[1;36m"
    #define C_MARCO   "\033[0;34m"
    #define C_OK      "\033[1;32m"
    #define C_AVISO   "\033[1;33m"
    #define C_ERROR   "\033[1;31m"
    #define C_TEXTO   "\033[0;37m"
#else
    #define C_RESET   ""
    #define C_TITULO  ""
    #define C_MARCO   ""
    #define C_OK      ""
    #define C_AVISO   ""
    #define C_ERROR   ""
    #define C_TEXTO   ""
#endif

enum {
    OPC_PARAMETROS = 1,
    OPC_ORBITA,
    OPC_PULSO,
    OPC_HISTORIAL,
    OPC_REPORTE,
    OPC_SALIR
};

static void limpiar_pantalla(void)
{
#if USAR_COLOR
    printf("\033[2J\033[H");
#else
    printf("\n\n\n");
#endif
}

static void pausa(void)
{
    int c;
    printf(C_AVISO "\nPresione ENTER para continuar..." C_RESET);
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static void pantalla_presentacion(void)
{
    limpiar_pantalla();
    printf(C_TITULO);
    printf("\n");
    printf("                   /\\\n");
    printf("                  /  \\\n");
    printf("                 | () |\n");
    printf("                 |ORION|\n");
    printf("                /|    |\\\n");
    printf("               /_|____|_\\\n");
    printf("                 /_||_\\\n");
    printf("                  ****\n");
    printf("                   **\n");
    printf(C_MARCO);
    printf("\n  +==============================================+\n");
    printf("  |" C_TITULO "          M I S I O N   A R T E M I S         " C_MARCO "|\n");
    printf("  |" C_TEXTO "      Orion en Orbita Terrestre Baja (LEO)    " C_MARCO "|\n");
    printf("  +==============================================+\n");
    printf("  |" C_TEXTO "   UTN FRRe - Algoritmos y Estructuras de     " C_MARCO "|\n");
    printf("  |" C_TEXTO "   Datos - Ingenieria en Sistemas de Inform.  " C_MARCO "|\n");
    printf("  +==============================================+\n" C_RESET);
    pausa();
}

static void mostrar_estado(const Orbita *o, int orbita_calculada)
{
    printf(C_MARCO "  +----------------- ESTADO ACTUAL -----------------+\n");
    printf("  |" C_TEXTO " Masa de Orion  : %12.1f kg              " C_MARCO "|\n", o->masa_nave);
    printf("  |" C_TEXTO " Altitud        : %12.1f m               " C_MARCO "|\n", o->altitud);
    printf("  |" C_TEXTO " Radio orbital  : %12.1f m               " C_MARCO "|\n", o->radio_orbital);
    printf("  |" C_TEXTO " Orbita calculada: %s                        " C_MARCO "|\n",
           orbita_calculada ? C_OK "SI" C_TEXTO : C_ERROR "NO" C_TEXTO);
    printf("  +-------------------------------------------------+\n" C_RESET);
}

static void mostrar_menu(void)
{
    printf(C_MARCO "\n  +==============  MENU PRINCIPAL  ==============+\n");
    printf("  |" C_TEXTO "  1 - Configurar parametros                   " C_MARCO "|\n");
    printf("  |" C_TEXTO "  2 - Calcular la orbita (puntos A, B y C)    " C_MARCO "|\n");
    printf("  |" C_TEXTO "  3 - Simular pulso lateral (punto D)         " C_MARCO "|\n");
    printf("  |" C_TEXTO "  4 - Ver historial de la sesion              " C_MARCO "|\n");
    printf("  |" C_TEXTO "  5 - Ver reporte final                       " C_MARCO "|\n");
    printf("  |" C_TEXTO "  6 - Salir                                   " C_MARCO "|\n");
    printf("  +==============================================+\n" C_RESET);
    printf(C_AVISO "  Elija una opcion: " C_RESET);
}

/* Lectura basica; despues se reemplaza por la funcion de validaciones.c */
static int leer_opcion(void)
{
    int opcion, c;
    if (scanf("%d", &opcion) != 1) {
        opcion = 0;
    }
    while ((c = getchar()) != '\n' && c != EOF) { }
    return opcion;
}

static void en_construccion(const char *modulo)
{
    printf(C_AVISO "\n  [ En construccion ] %s\n" C_RESET, modulo);
}

int main(void)
{
    Orbita o;
    ResultadosOrbita r;
    int opcion;
    int orbita_calculada = 0;

    /* Valores iniciales del escenario */
    o.masa_nave = 33000.0;
    o.altitud = 192000.0;
    o.radio_orbital = calcular_radio_orbital(o.altitud);

    pantalla_presentacion();

    do {
        limpiar_pantalla();
        mostrar_estado(&o, orbita_calculada);
        mostrar_menu();
        opcion = leer_opcion();

        limpiar_pantalla();
        switch (opcion) {
            case OPC_PARAMETROS:
                en_construccion("Configurar parametros (validaciones)");
                pausa();
                break;
            case OPC_ORBITA:
                calcular_orbita(&o, &r);
                orbita_calculada = 1;
                printf(C_OK "\n  Orbita calculada correctamente\n" C_RESET);
                mostrar_demostracion(&r);
                pausa();
                break;
            case OPC_PULSO:
                en_construccion("Pulso lateral (propulsor)");
                pausa();
                break;
            case OPC_HISTORIAL:
                en_construccion("Historial de la sesion");
                pausa();
                break;
            case OPC_REPORTE:
                en_construccion("Reporte final");
                pausa();
                break;
            case OPC_SALIR:
                printf(C_TITULO "\n  Fin de la mision. Hasta la proxima!\n\n" C_RESET);
                break;
            default:
                printf(C_ERROR "\n  Opcion invalida. Elija un numero del 1 al 6.\n" C_RESET);
                pausa();
                break;
        }
    } while (opcion != OPC_SALIR);

    return 0;
}
