/*
 * demo_propulsor.c - Ejecuta SOLO el modulo del propulsor (Punto D), sin el resto del proyecto.
 * La orbita se arma aqui con la formula v = sqrt(G*MT/r), que en el proyecto completo
 * calcula el integrante 2 (artemis.c).
 */
#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

int main(void)
{
    ResultadoOrbital   orb = {0};
    EventoPropulsor    ev;
    ResultadoPropulsor prop;

    orb.r = R_TIERRA + H_DEFECTO;                    /* h = 192 km */
    orb.v = sqrt(G_CONST * M_TIERRA / orb.r);        /* velocidad orbital */

    propulsor_leer_evento(&ev);
    propulsor_calcular(&orb, &ev, &prop);
    propulsor_mostrar(&orb, &ev, &prop);
    return 0;
}
