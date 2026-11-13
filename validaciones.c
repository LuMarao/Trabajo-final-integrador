#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include "validaciones.h"

static int leer_linea(char *buffer, int tamano)
{
    size_t largo;
    int caracter;

    if (fgets(buffer, tamano, stdin) == NULL)
    {
        printf("\nSe cerro la entrada de datos. Fin del programa.\n");
        exit(EXIT_FAILURE);
    }

    largo = strlen(buffer);

    if (largo > 0 && buffer[largo - 1] == '\n')
    {
        return 1;
    }

    if (largo == (size_t)(tamano - 1))
    {
        while ((caracter = getchar()) != '\n' && caracter != EOF)
        {
        }
        return 0;
    }

    return 1;
}

static int solo_espacios(const char *texto)
{
    while (*texto != '\0')
    {
        if (!isspace((unsigned char)*texto))
        {
            return 0;
        }
        texto++;
    }
    return 1;
}

int leer_entero(const char *mensaje, int minimo, int maximo)
{
    char buffer[LONGITUD_MAX_ENTRADA];
    char *fin;
    long valor = 0;
    int valido = 0;

    do
    {
        printf("%s", mensaje);

        if (leer_linea(buffer, LONGITUD_MAX_ENTRADA))
        {
            errno = 0;
            valor = strtol(buffer, &fin, 10);

            if (fin == buffer || !solo_espacios(fin))
            {
                printf("Error: ingrese un numero entero valido.\n");
            }
            else if (errno == ERANGE || valor < INT_MIN || valor > INT_MAX ||
                     valor < minimo || valor > maximo)
            {
                printf("Error: el valor debe estar entre %d y %d.\n", minimo, maximo);
            }
            else
            {
                valido = 1;
            }
        }
        else
        {
            printf("Error: la entrada es demasiado larga.\n");
        }
    } while (!valido);

    return (int)valor;
}

static int solo_caracteres_numericos(const char *texto)
{
    while (*texto != '\0')
    {
        if (!isspace((unsigned char)*texto) && strchr("0123456789+-.eE", *texto) == NULL)
        {
            return 0;
        }
        texto++;
    }
    return 1;
}
 
int convertir_real(const char *texto, double minimo, double maximo, double *resultado)
{
    char buffer[LONGITUD_MAX_ENTRADA];
    char *fin;
    char *coma;
    double valor;
 
    if (strlen(texto) >= LONGITUD_MAX_ENTRADA)
    {
        return VALIDACION_FORMATO;
    }
    strcpy(buffer, texto);
 
    while ((coma = strchr(buffer, ',')) != NULL)
    {
        *coma = '.';
    }
 
    errno = 0;
    valor = strtod(buffer, &fin);
 
    if (fin == buffer || !solo_espacios(fin) || !isfinite(valor) || !solo_caracteres_numericos(buffer))
    {
        return VALIDACION_FORMATO;
    }
    if (errno == ERANGE || valor < minimo || valor > maximo)
    {
        return VALIDACION_RANGO;
    }
 
    *resultado = valor;
    return VALIDACION_OK;
}

double leer_real(const char *mensaje, double minimo, double maximo)
{
    char buffer[LONGITUD_MAX_ENTRADA];
    double valor = 0.0;
    int valido = 0;
    int estado;

    do
    {
        printf("%s", mensaje);

        if (leer_linea(buffer, LONGITUD_MAX_ENTRADA))
        {
            estado = convertir_real(buffer, minimo, maximo, &valor);

            if (estado == VALIDACION_FORMATO)
            {
                printf("Error: ingrese un numero real valido.\n");
            }
            else if (estado == VALIDACION_RANGO)
            {
                printf("Error: el valor debe estar entre %g y %g.\n", minimo, maximo);
            }
            else
            {
                valido = 1;
            }
        }
        else
        {
            printf("Error: la entrada es demasiado larga.\n");
        }
    } while (!valido);

    return valor;
}

int leer_opcion_menu(const char *mensaje, int opcion_minima, int opcion_maxima)
{
    return leer_entero(mensaje, opcion_minima, opcion_maxima);
}

int leer_si_no(const char *mensaje)
{
    char buffer[LONGITUD_MAX_ENTRADA];
    char *p;
    int respuesta = -1;

    do
    {
        printf("%s", mensaje);

        if (leer_linea(buffer, LONGITUD_MAX_ENTRADA))
        {
            p = buffer;
            while (isspace((unsigned char)*p))
            {
                p++;
            }

            if ((tolower((unsigned char)*p) == 's' || tolower((unsigned char)*p) == 'n') &&
                solo_espacios(p + 1))
            {
                respuesta = (tolower((unsigned char)*p) == 's') ? 1 : 0;
            }
            else
            {
                printf("Error: responda con 's' o 'n'.\n");
            }
        }
        else
        {
            printf("Error: la entrada es demasiado larga.\n");
        }
    } while (respuesta == -1);

    return respuesta;
}
