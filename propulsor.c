/*
 * propulsor.c - Punto D: propulsor lateral (entradas, velocidad lateral,
 *               desviacion y descripcion de la trayectoria)
 * RESPONSABLE: Integrante 3
 *
 * Modelo: el propulsor empuja de forma lateral (perpendicular a la velocidad orbital,
 * fuera del plano de la orbita). Como el pulso es corto frente al periodo, se trata
 * como instantaneo:
 *     dv   = a*dt                  velocidad lateral ganada
 *     desv = 1/2*a*dt^2            desviacion lateral durante el pulso
 *     v'   = sqrt(v^2 + dv^2)      nueva rapidez (componentes perpendiculares)
 *
 * Restricciones de dominio (ver artemis.h): a y dt SIEMPRE mayores que 0 y dentro de
 * un rango fisicamente razonable. La validacion se hace dos veces: al leer del usuario
 * (leer_real) y dentro del calculo (por si otro modulo llama con datos invalidos).
 */
#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

/* Devuelve 1 si los datos del pulso estan dentro del dominio valido, 0 si no. */
int validar_datos_propulsor(const DatosPropulsor *prop)
{
    if (prop == NULL)
        return 0;
    if (!isfinite(prop->aceleracion) || !isfinite(prop->duracion))
        return 0;
    return prop->aceleracion >= ACEL_PULSO_MIN && prop->aceleracion <= ACEL_PULSO_MAX &&
           prop->duracion    >= DT_PULSO_MIN   && prop->duracion    <= DT_PULSO_MAX;
}

/* Entradas del evento: lectura segura con validacion de tipo y rango. */
void leer_datos_propulsor(DatosPropulsor *prop)
{
    char msj_a[LONGITUD_MAX_ENTRADA + 60];
    char msj_dt[LONGITUD_MAX_ENTRADA + 60];

    printf("\n=== EVENTO DEL PROPULSOR LATERAL ===\n");
    if (leer_si_no("  Usar los datos de la consigna (a = 0.50 m/s^2, dt = 10 s)? (S/N): ")) {
        prop->aceleracion = ACEL_PULSO_DEF;
        prop->duracion    = DT_PULSO_DEF;
        return;
    }

    snprintf(msj_a, sizeof msj_a,
             "  Aceleracion del propulsor a [m/s^2] (%g - %g): ", ACEL_PULSO_MIN, ACEL_PULSO_MAX);
    snprintf(msj_dt, sizeof msj_dt,
             "  Duracion del pulso dt [s] (%g - %g): ", DT_PULSO_MIN, DT_PULSO_MAX);

    prop->aceleracion = leer_real(msj_a,  ACEL_PULSO_MIN, ACEL_PULSO_MAX);
    prop->duracion    = leer_real(msj_dt, DT_PULSO_MIN,   DT_PULSO_MAX);
}

/* Calcula el efecto del pulso. Devuelve 0 si fue OK, -1 si los datos no son validos. */
int calcular_pulso_lateral(const ResultadosOrbita *orb, const DatosPropulsor *prop,
                           ResultadosPropulsor *res)
{
    double v, r, cociente, exc;

    if (orb == NULL || res == NULL || !validar_datos_propulsor(prop))
        return -1;

    v = orb->velocidad_orbital;
    r = orb->radio_orbital;
    if (!(v > 0.0) || !(r > 0.0))          /* sin orbita valida no hay modelo */
        return -1;

    cociente = (prop->aceleracion * prop->duracion) / v;      /* dv / v */
    exc      = cociente * cociente;                           /* e = (v'^2 - v^2)/v^2 */
    if (exc >= 1.0)                                           /* dejaria de ser orbita cerrada */
        return -1;

    res->velocidad_lateral  = prop->aceleracion * prop->duracion;
    res->desviacion_lateral = 0.5 * prop->aceleracion * prop->duracion * prop->duracion;
    res->velocidad_nueva    = sqrt(v * v + res->velocidad_lateral * res->velocidad_lateral);
    res->giro_grados        = atan(cociente) * 180.0 / PI;
    res->excentricidad      = exc;
    res->aumento_apoapsis   = r * (1.0 + exc) / (1.0 - exc) - r;
    res->separacion_maxima  = r * sin(atan(cociente));
    return 0;
}

void mostrar_resultados_propulsor(const ResultadosOrbita *orb, const DatosPropulsor *prop,
                                  const ResultadosPropulsor *res)
{
    printf("\n=== PUNTO D - PROPULSOR LATERAL ===\n");
    printf("  Entradas del evento: a = %.3f m/s^2 (%.4f g) | dt = %.1f s\n",
           prop->aceleracion, prop->aceleracion / G_TERRESTRE, prop->duracion);
    printf("  Velocidad orbital antes del pulso: v = %.2f m/s\n", orb->velocidad_orbital);
    printf("  El pulso dura %.2f %% del periodo orbital (modelo de pulso instantaneo valido\n"
           "  mientras sea mucho menor que 100 %%).\n\n",
           100.0 * prop->duracion / orb->periodo_s);

    printf("  1) Velocidad lateral adquirida:   dv   = a*dt           = %.3f m/s\n", res->velocidad_lateral);
    printf("  2) Desviacion lateral del pulso:  desv = 1/2*a*dt^2     = %.3f m\n", res->desviacion_lateral);
    printf("  3) Nueva rapidez total:           v'   = sqrt(v^2+dv^2) = %.4f m/s\n", res->velocidad_nueva);
    printf("  4) Giro del plano orbital:        atan(dv/v)            = %.5f grados\n", res->giro_grados);

    printf("\n  Descripcion de la trayectoria:\n");
    printf("   - La nave NO sale en linea recta: la gravedad sigue actuando como fuerza\n");
    printf("     centripeta, asi que continua en orbita.\n");
    printf("   - La velocidad lateral se suma (perpendicularmente) a la orbital: el plano de la\n");
    printf("     orbita se inclina %.5f grados.\n", res->giro_grados);
    printf("   - La rapidez aumenta apenas, por lo que la orbita deja de ser perfectamente\n");
    printf("     circular: excentricidad %.3e, el apoapsis sube %.2f m.\n",
           res->excentricidad, res->aumento_apoapsis);
    printf("   - Un cuarto de vuelta despues la nave queda separada ~%.0f m (lateralmente) de\n",
           res->separacion_maxima);
    printf("     su orbita original; a la media vuelta las dos orbitas se cruzan de nuevo.\n");
    printf("   (Se interpreta 'perpendicular a la velocidad' como lateral, fuera del plano orbital.)\n");
}
