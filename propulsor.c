/*
 * propulsor.c - Punto D: propulsor lateral (entradas, velocidad lateral,
 *               desviacion y descripcion de la trayectoria)
 * RESPONSABLE: Integrante 3
 *
 * Modelo: el propulsor empuja de forma lateral (perpendicular a la velocidad orbital,
 * fuera del plano de la orbita). Como el pulso es muy corto frente al periodo, se trata
 * como instantaneo:
 *     dv   = a*dt                  velocidad lateral ganada
 *     desv = 1/2*a*dt^2            desviacion lateral durante el pulso
 *     v'   = sqrt(v^2 + dv^2)      nueva rapidez (componentes perpendiculares)
 */
#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

void propulsor_valores_defecto(EventoPropulsor *e)
{
    e->a  = A_DEFECTO;
    e->dt = DT_DEFECTO;
}

/* Entradas del evento: usa la lectura segura del integrante 4 */
void propulsor_leer_evento(EventoPropulsor *e)
{
    ui_titulo("EVENTO DEL PROPULSOR LATERAL");
    if (leer_si_no("  Usar los datos de la consigna (a = 0.50 m/s^2, dt = 10 s)? (S/N): ")) {
        propulsor_valores_defecto(e);
        return;
    }
    e->a  = leer_real("  Aceleracion del propulsor a [m/s^2] (0.01 - 10): ", 0.01, 10.0);
    e->dt = leer_real("  Duracion del pulso dt [s] (0.1 - 300): ", 0.1, 300.0);
}

void propulsor_calcular(const ResultadoOrbital *o, const EventoPropulsor *e, ResultadoPropulsor *p)
{
    double cociente = 0.0;                 /* dv / v */

    p->dv      = e->a * e->dt;
    p->desv    = 0.5 * e->a * e->dt * e->dt;
    p->v_nueva = sqrt(o->v * o->v + p->dv * p->dv);

    cociente     = p->dv / o->v;
    p->giro_deg  = atan(cociente) * 180.0 / PI;
    p->exc       = cociente * cociente;                       /* e = (v'^2 - v^2)/v^2 */
    p->dr_apo    = o->r * (1.0 + p->exc) / (1.0 - p->exc) - o->r;
    p->sep_max   = o->r * sin(atan(cociente));
}

void propulsor_mostrar(const ResultadoOrbital *o, const EventoPropulsor *e, const ResultadoPropulsor *p)
{
    ui_titulo("PUNTO D - PROPULSOR LATERAL");
    printf("  Entradas del evento: a = %.3f m/s^2 | dt = %.1f s\n", e->a, e->dt);
    printf("  Velocidad orbital antes del pulso: v = %.2f m/s\n\n", o->v);

    printf("  1) Velocidad lateral adquirida:   dv   = a*dt        = %.3f m/s\n", p->dv);
    printf("  2) Desviacion lateral del pulso:  desv = 1/2*a*dt^2  = %.3f m\n", p->desv);
    printf("  3) Nueva rapidez total:           v'   = sqrt(v^2+dv^2) = %.4f m/s\n", p->v_nueva);
    printf("  4) Giro del plano orbital:        atan(dv/v)         = %.5f grados\n", p->giro_deg);

    printf("\n  Descripcion de la trayectoria:\n");
    printf("   - La nave NO sale en linea recta: la gravedad sigue actuando como fuerza\n");
    printf("     centripeta, asi que continua en orbita.\n");
    printf("   - La velocidad lateral se suma (perpendicularmente) a la orbital: el plano de la\n");
    printf("     orbita se inclina %.5f grados.\n", p->giro_deg);
    printf("   - La rapidez aumenta apenas, por lo que la orbita deja de ser perfectamente\n");
    printf("     circular: excentricidad %.3e, el apoapsis sube %.2f m.\n", p->exc, p->dr_apo);
    printf("   - Un cuarto de vuelta despues la nave queda separada ~%.0f m (lateralmente) de\n", p->sep_max);
    printf("     su orbita original; a la media vuelta las dos orbitas se cruzan de nuevo.\n");
    printf("   (Se interpreta 'perpendicular a la velocidad' como lateral, fuera del plano orbital.)\n");
}
