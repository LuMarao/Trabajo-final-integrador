#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "propulsor.h"
#include "reportes.h"
#include "validaciones.h"

void propulsor_valores_defecto(DatosPropulsor *prop)
{
    prop->aceleracion = ACEL_PULSO_DEF;
    prop->duracion    = DT_PULSO_DEF;
}

void propulsor_leer_datos(DatosPropulsor *prop)
{
    char mensaje[120];

    ui_titulo("EVENTO DEL PROPULSOR LATERAL");
    snprintf(mensaje, sizeof(mensaje),
             "  Usar los datos de la consigna (a = %g m/s^2, dt = %g s)? (S/N): ",
             ACEL_PULSO_DEF, DT_PULSO_DEF);
    if (leer_si_no(mensaje)) {
        propulsor_valores_defecto(prop);
        return;
    }

    snprintf(mensaje, sizeof(mensaje), "  Aceleracion del propulsor a [m/s^2] (%g - %g): ",
             ACEL_PULSO_MIN, ACEL_PULSO_MAX);
    prop->aceleracion = leer_real(mensaje, ACEL_PULSO_MIN, ACEL_PULSO_MAX);

    snprintf(mensaje, sizeof(mensaje), "  Duracion del pulso dt [s] (%g - %g): ",
             DT_PULSO_MIN, DT_PULSO_MAX);
    prop->duracion = leer_real(mensaje, DT_PULSO_MIN, DT_PULSO_MAX);
}

void calcular_pulso_lateral(const ResultadosOrbita *orbita, const DatosPropulsor *prop,
                            ResultadosPropulsor *res)
{
    double v = orbita->velocidad_orbital;
    double r = orbita->radio_orbital;
    double cociente;

    res->velocidad_lateral  = prop->aceleracion * prop->duracion;
    res->desviacion_lateral = 0.5 * prop->aceleracion * prop->duracion * prop->duracion;
    res->velocidad_nueva    = sqrt(v * v + res->velocidad_lateral * res->velocidad_lateral);

    cociente                = res->velocidad_lateral / v;
    res->giro_grados        = atan(cociente) * 180.0 / PI;
    res->excentricidad      = cociente * cociente;       /* e = (v'^2 - v^2)/v^2 */
    res->separacion_maxima  = r * sin(atan(cociente));

    if (res->excentricidad < 1.0) {
        res->aumento_apoapsis = r * (1.0 + res->excentricidad) / (1.0 - res->excentricidad) - r;
    } else {
        res->aumento_apoapsis = INFINITY;  /* la nave escaparia de la orbita cerrada */
    }
}
