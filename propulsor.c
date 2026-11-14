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
    ui_titulo("EVENTO DEL PROPULSOR LATERAL");
    if (leer_si_no("  Usar los datos de la consigna (a = 0.50 m/s^2, dt = 10 s)? (S/N): ")) {
        propulsor_valores_defecto(prop);
        return;
    }
    prop->aceleracion = leer_real("  Aceleracion del propulsor a [m/s^2] (0 - 10): ",
                                  ACEL_PULSO_MIN, ACEL_PULSO_MAX);
    prop->duracion    = leer_real("  Duracion del pulso dt [s] (0 - 600): ",
                                  DT_PULSO_MIN, DT_PULSO_MAX);
}

void calcular_pulso_lateral(const ResultadosOrbita *orbita, const DatosPropulsor *prop,
                            ResultadosPropulsor *res)
{
    double v = orbita->velocidad_orbital;
    double r = orbita->radio_orbital;
    double cociente;                       /* dv / v */

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
