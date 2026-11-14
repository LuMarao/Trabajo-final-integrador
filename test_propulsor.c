/*
 * test_propulsor.c - Prueba automatica del propulsor lateral (Integrante 3)
 * No interactiva: llama directamente a las funciones del modulo.
 */
#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

static int fallos = 0;

static void check(const char *nombre, double obtenido, double esperado, double tol)
{
    double err = (esperado != 0.0) ? fabs(obtenido - esperado) / fabs(esperado) : fabs(obtenido);
    int ok = (err <= tol);
    printf("  [%s] %-34s obtenido = %-14.8g esperado = %-14.8g\n",
           ok ? "PASS" : "FAIL", nombre, obtenido, esperado);
    if (!ok)
        fallos++;
}

static void check_int(const char *nombre, int obtenido, int esperado)
{
    int ok = (obtenido == esperado);
    printf("  [%s] %-34s obtenido = %-14d esperado = %-14d\n",
           ok ? "PASS" : "FAIL", nombre, obtenido, esperado);
    if (!ok)
        fallos++;
}

static int pulso(const ResultadosOrbita *o, double a, double dt, ResultadosPropulsor *r)
{
    DatosPropulsor p;
    p.aceleracion = a;
    p.duracion = dt;
    return calcular_pulso_lateral(o, &p, r);
}

int main(void)
{
    DatosOrbita orbita;
    DatosPropulsor prop;
    ResultadosOrbita o;
    ResultadosPropulsor r;
    double dummy;

    cargar_valores_originales(&orbita, &prop);
    calcular_orbita(&orbita, &o);

    printf("== Datos de la consigna (a = 0.5 m/s^2, dt = 10 s)\n");
    check("a por defecto", prop.aceleracion, 0.5, 1e-12);
    check("dt por defecto", prop.duracion, 10.0, 1e-12);
    check_int("calculo OK", pulso(&o, 0.5, 10.0, &r), 0);
    check("velocidad lateral dv [m/s]", r.velocidad_lateral, 5.0, 1e-12);
    check("desviacion 1/2 a dt^2 [m]", r.desviacion_lateral, 25.0, 1e-12);
    check("nueva rapidez [m/s]", r.velocidad_nueva,
          sqrt(o.velocidad_orbital * o.velocidad_orbital + 25.0), 1e-12);
    check("giro del plano [grados]", r.giro_grados, 0.03676, 1e-3);

    printf("== Otro evento valido (a = 1.5 m/s^2, dt = 60 s)\n");
    check_int("calculo OK", pulso(&o, 1.5, 60.0, &r), 0);
    check("dv [m/s]", r.velocidad_lateral, 90.0, 1e-12);
    check("desv [m]", r.desviacion_lateral, 2700.0, 1e-12);

    printf("== Limites exactos validos\n");
    check_int("a min, dt min", pulso(&o, ACEL_PULSO_MIN, DT_PULSO_MIN, &r), 0);
    check_int("a max, dt max", pulso(&o, ACEL_PULSO_MAX, DT_PULSO_MAX, &r), 0);

    printf("== Rangos del propulsor (definidos en artemis.h)\n");
    check_int("a minimo > 0", ACEL_PULSO_MIN > 0.0, 1);
    check_int("dt minimo > 0", DT_PULSO_MIN > 0.0, 1);
    check_int("a min < a max", ACEL_PULSO_MIN < ACEL_PULSO_MAX, 1);
    check_int("dt min < dt max", DT_PULSO_MIN < DT_PULSO_MAX, 1);
    check_int("datos de la consigna en rango", validar_datos_propulsor(&prop), 1);
    check_int("justo debajo de a min", pulso(&o, ACEL_PULSO_MIN * 0.99, 10.0, &r), -1);
    check_int("justo debajo de dt min", pulso(&o, 0.5, DT_PULSO_MIN * 0.99, &r), -1);
    check_int("justo encima de a max", pulso(&o, ACEL_PULSO_MAX * 1.01, 10.0, &r), -1);
    check_int("justo encima de dt max", pulso(&o, 0.5, DT_PULSO_MAX * 1.01, &r), -1);

    printf("== Valores maximos: la orbita sigue casi circular\n");
    check_int("calculo OK con a y dt max", pulso(&o, ACEL_PULSO_MAX, DT_PULSO_MAX, &r), 0);
    check_int("excentricidad < 0.01", r.excentricidad < 0.01, 1);
    check_int("giro del plano < 5 grados", r.giro_grados < 5.0, 1);
    check_int("dv < 5% de la v orbital", r.velocidad_lateral < 0.05 * o.velocidad_orbital, 1);
    check_int("dt max < 5% del periodo", DT_PULSO_MAX < 0.05 * o.periodo_s, 1);

    printf("== Datos NO realistas: deben rechazarse (-1)\n");
    check_int("a = 0", pulso(&o, 0.0, 10.0, &r), -1);
    check_int("dt = 0", pulso(&o, 0.5, 0.0, &r), -1);
    check_int("a = 0 y dt = 0", pulso(&o, 0.0, 0.0, &r), -1);
    check_int("a negativa", pulso(&o, -0.5, 10.0, &r), -1);
    check_int("dt negativo", pulso(&o, 0.5, -10.0, &r), -1);
    check_int("a sobre el maximo", pulso(&o, ACEL_PULSO_MAX + 0.1, 10.0, &r), -1);
    check_int("dt sobre el maximo", pulso(&o, 0.5, DT_PULSO_MAX + 1.0, &r), -1);
    check_int("a = NaN", pulso(&o, NAN, 10.0, &r), -1);
    check_int("dt = infinito", pulso(&o, 0.5, INFINITY, &r), -1);

    printf("== Orbita invalida (v = 0): no debe dividir por cero\n");
    o.velocidad_orbital = 0.0;
    check_int("v = 0", pulso(&o, 0.5, 10.0, &r), -1);

    printf("== Validacion de entrada (convertir_real, rango de a)\n");
    check_int("texto '0'", convertir_real("0", ACEL_PULSO_MIN, ACEL_PULSO_MAX, &dummy), VALIDACION_RANGO);
    check_int("texto '-1'", convertir_real("-1", ACEL_PULSO_MIN, ACEL_PULSO_MAX, &dummy), VALIDACION_RANGO);
    check_int("texto 'abc'", convertir_real("abc", ACEL_PULSO_MIN, ACEL_PULSO_MAX, &dummy), VALIDACION_FORMATO);
    check_int("texto '0,5' (coma)", convertir_real("0,5", ACEL_PULSO_MIN, ACEL_PULSO_MAX, &dummy), VALIDACION_OK);

    printf("\n%s (%d fallos)\n", fallos == 0 ? "TODAS LAS PRUEBAS PASARON" : "HAY PRUEBAS FALLIDAS", fallos);
    return fallos != 0;
}
