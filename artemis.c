#include <stdio.h>
#include <math.h>
#include "artemis.h"

/* r = RT + h
   altitud: m | resultado: m */
double calcular_radio_orbital(double altitud) {
    return RADIO_TIERRA + altitud;
}

/* Fg = G * MT * m / r²
   masa: kg | radio: m | resultado: N */
double calcular_fuerza_gravitatoria(double masa, double radio) {
    return G * MASA_TIERRA * masa / (radio * radio);
}

/* v = sqrt(G * MT / r)
   radio: m | resultado: m/s */
double calcular_velocidad_orbital(double radio) {
    return sqrt(G * MASA_TIERRA / radio);
}

/* Fc = m * v² / r
   masa: kg | velocidad: m/s | radio: m | resultado: N */
double calcular_fuerza_centripeta(double masa, double velocidad, double radio) {
    return masa * velocidad * velocidad / radio;
}

/* T = 2 * pi * r / v
   radio: m | velocidad: m/s | resultado: s */
double calcular_periodo(double radio, double velocidad) {
    return 2.0 * PI * radio / velocidad;
}

/* ac = v² / r
   velocidad: m/s | radio: m | resultado: m/s² */
double calcular_aceleracion_centripeta(double velocidad, double radio) {
    return velocidad * velocidad / radio;
}

/* Calcula todo y lo guarda en res (uso de punteros) */
void calcular_orbita(const Orbita *o, ResultadosOrbita *res) {
    res->fuerza_grav        = calcular_fuerza_gravitatoria(o->masa_nave, o->radio_orbital);
    res->velocidad          = calcular_velocidad_orbital(o->radio_orbital);
    res->fuerza_centripeta  = calcular_fuerza_centripeta(o->masa_nave, res->velocidad, o->radio_orbital);
    res->periodo_s          = calcular_periodo(o->radio_orbital, res->velocidad);
    res->periodo_min        = res->periodo_s / 60.0;
    res->aceleracion_c      = calcular_aceleracion_centripeta(res->velocidad, o->radio_orbital);
}

void mostrar_demostracion(const ResultadosOrbita *res) {
    printf("\n--- Punto A: Fg actua como fuerza centripeta ---\n");
    printf("Fg = G*MT*m/r^2   = %.2f N\n", res->fuerza_grav);
    printf("Fc = m*v^2/r      = %.2f N\n", res->fuerza_centripeta);
    printf("Igualando: G*MT*m/r^2 = m*v^2/r  =>  v = sqrt(G*MT/r)\n");
    printf("Velocidad orbital v = %.2f m/s\n", res->velocidad);
    printf("\n--- Punto B: Periodo ---\n");
    printf("T = %.2f s = %.2f min\n", res->periodo_s, res->periodo_min);
    printf("\n--- Punto C: Aceleracion centripeta ---\n");
    printf("ac = %.3f m/s^2\n", res->aceleracion_c);
}
