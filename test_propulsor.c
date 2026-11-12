/*
 * test_propulsor.c - Prueba automatica del propulsor lateral (Integrante 3)
 * No interactiva: llama directamente a propulsor_calcular().
 */
#include <stdio.h>
#include <math.h>
#include "artemis.h"

static int fallos = 0;

static void check(const char *nombre, double obtenido, double esperado, double tol)
{
    double err = (esperado != 0.0) ? fabs(obtenido - esperado) / fabs(esperado) : fabs(obtenido);
    int ok = (err <= tol);
    printf("  [%s] %-30s obtenido = %-14.8g esperado = %-14.8g\n",
           ok ? "PASS" : "FAIL", nombre, obtenido, esperado);
    if (!ok)
        fallos++;
}

int main(void)
{
    /* orbita de la consigna ya resuelta (se arma a mano para no depender del integrante 2) */
    ResultadoOrbital o = { 6563000.0, 0, 0, 7792.9500, 0, 0, 0 };
    EventoPropulsor e;
    ResultadoPropulsor p;

    printf("== Datos de la consigna (a = 0.5 m/s^2, dt = 10 s)\n");
    propulsor_valores_defecto(&e);
    check("a por defecto", e.a, 0.5, 1e-12);
    check("dt por defecto", e.dt, 10.0, 1e-12);
    propulsor_calcular(&o, &e, &p);
    check("velocidad lateral dv [m/s]", p.dv, 5.0, 1e-12);
    check("desviacion 1/2 a dt^2 [m]", p.desv, 25.0, 1e-12);
    check("nueva rapidez [m/s]", p.v_nueva, sqrt(7792.95 * 7792.95 + 25.0), 1e-12);
    check("giro del plano [grados]", p.giro_deg, 0.03676, 1e-3);

    printf("== Otro evento (a = 2 m/s^2, dt = 30 s)\n");
    e.a = 2.0;
    e.dt = 30.0;
    propulsor_calcular(&o, &e, &p);
    check("dv [m/s]", p.dv, 60.0, 1e-12);
    check("desv [m]", p.desv, 900.0, 1e-12);

    printf("== Sin pulso (a = 0): no hay efecto\n");
    e.a = 0.0;
    propulsor_calcular(&o, &e, &p);
    check("dv", p.dv, 0.0, 1e-12);
    check("giro", p.giro_deg, 0.0, 1e-12);

    printf("\n%s (%d fallos)\n", fallos == 0 ? "TODAS LAS PRUEBAS PASARON" : "HAY PRUEBAS FALLIDAS", fallos);
    return fallos != 0;
}
