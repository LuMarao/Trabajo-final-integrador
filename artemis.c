#include <math.h>
#include "artemis.h"

void cargar_valores_originales(DatosOrbita *orbita, DatosPropulsor *propulsor)
{
    orbita->masa_tierra  = MASA_TIERRA_DEF;
    orbita->radio_tierra = RADIO_TIERRA_DEF;
    orbita->altitud      = ALTITUD_DEF;
    orbita->masa_nave    = MASA_ORION_DEF;

    propulsor->aceleracion = ACEL_PULSO_DEF;
    propulsor->duracion    = DT_PULSO_DEF;
}

double calcular_radio_orbital(const DatosOrbita *orbita)
{
    return orbita->radio_tierra + orbita->altitud;
}

double calcular_fuerza_gravitatoria(const DatosOrbita *orbita)
{
    double radio = calcular_radio_orbital(orbita);

    return G_GRAVITACION * orbita->masa_tierra * orbita->masa_nave / (radio * radio);
}

double calcular_velocidad_orbital(const DatosOrbita *orbita)
{
    double radio = calcular_radio_orbital(orbita);

    return sqrt(G_GRAVITACION * orbita->masa_tierra / radio);
}

double calcular_periodo_orbital(const DatosOrbita *orbita)
{
    double radio     = calcular_radio_orbital(orbita);
    double velocidad = calcular_velocidad_orbital(orbita);

    return 2.0 * PI * radio / velocidad;
}

double calcular_aceleracion_centripeta(const DatosOrbita *orbita)
{
    double radio     = calcular_radio_orbital(orbita);
    double velocidad = calcular_velocidad_orbital(orbita);

    return velocidad * velocidad / radio;
}

double calcular_fuerza_centripeta(const DatosOrbita *orbita)
{
    double radio     = calcular_radio_orbital(orbita);
    double velocidad = calcular_velocidad_orbital(orbita);

    return orbita->masa_nave * velocidad * velocidad / radio;
}

void calcular_orbita(const DatosOrbita *orbita, ResultadosOrbita *res)
{
    res->radio_orbital          = calcular_radio_orbital(orbita);
    res->fuerza_gravitatoria    = calcular_fuerza_gravitatoria(orbita);
    res->velocidad_orbital      = calcular_velocidad_orbital(orbita);
    res->periodo_s              = calcular_periodo_orbital(orbita);
    res->periodo_min            = res->periodo_s / 60.0;
    res->aceleracion_centripeta = calcular_aceleracion_centripeta(orbita);
}
