#include <stdio.h>
#include <math.h>
#include "artemis.h"

void ui_titulo(const char *titulo)
{
    printf("\n==== %s ====\n", titulo);
}

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
    double radio;

    radio = orbita->radio_tierra + orbita->altitud;
    return radio;
}

double calcular_fuerza_gravitatoria(const DatosOrbita *orbita)
{
    double radio;
    double fuerza;

    radio  = calcular_radio_orbital(orbita);
    fuerza = G_GRAVITACION * orbita->masa_tierra * orbita->masa_nave / (radio * radio);
    return fuerza;
}

double calcular_velocidad_orbital(const DatosOrbita *orbita)
{
    double radio;
    double velocidad;

    radio     = calcular_radio_orbital(orbita);
    velocidad = sqrt(G_GRAVITACION * orbita->masa_tierra / radio);
    return velocidad;
}

double calcular_periodo_orbital(const DatosOrbita *orbita)
{
    double radio;
    double velocidad;
    double periodo;

    radio     = calcular_radio_orbital(orbita);
    velocidad = calcular_velocidad_orbital(orbita);
    periodo   = 2.0 * PI * radio / velocidad;
    return periodo;
}

double calcular_aceleracion_centripeta(const DatosOrbita *orbita)
{
    double radio;
    double velocidad;
    double aceleracion;

    radio       = calcular_radio_orbital(orbita);
    velocidad   = calcular_velocidad_orbital(orbita);
    aceleracion = velocidad * velocidad / radio;
    return aceleracion;
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

void mostrar_demostracion_centripeta(const DatosOrbita *orbita)
{
    ResultadosOrbita res;
    double fc;

    calcular_orbita(orbita, &res);
    fc = orbita->masa_nave * res.velocidad_orbital * res.velocidad_orbital / res.radio_orbital;

    ui_titulo("PUNTO A - Fg ACTUA COMO FUERZA CENTRIPETA");
    printf("  Fg = G*MT*m/r^2   = %.2f N\n", res.fuerza_gravitatoria);
    printf("  Fc = m*v^2/r      = %.2f N\n", fc);
    printf("  Igualando: G*MT*m/r^2 = m*v^2/r  =>  v = sqrt(G*MT/r)\n");
    printf("  Velocidad orbital v = %.2f m/s\n", res.velocidad_orbital);
}

void mostrar_resultados_orbita(const DatosOrbita *orbita, const ResultadosOrbita *res)
{
    ui_titulo("DATOS DE ENTRADA");
    printf("  Masa de la Tierra MT = %.3e kg\n", orbita->masa_tierra);
    printf("  Radio terrestre RT   = %.1f m\n", orbita->radio_tierra);
    printf("  Altitud h            = %.1f m\n", orbita->altitud);
    printf("  Masa de Orion m      = %.1f kg\n", orbita->masa_nave);
    printf("  Radio orbital r      = %.1f m\n", res->radio_orbital);

    ui_titulo("RESULTADOS A, B Y C");
    printf("  Fuerza gravitatoria Fg    = %.2f N\n", res->fuerza_gravitatoria);
    printf("  Velocidad orbital v       = %.2f m/s\n", res->velocidad_orbital);
    printf("  Periodo orbital T         = %.2f s = %.2f min\n", res->periodo_s, res->periodo_min);
    printf("  Aceleracion centripeta ac = %.3f m/s^2\n", res->aceleracion_centripeta);
}
