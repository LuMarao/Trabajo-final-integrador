#ifndef ARTEMIS_H
#define ARTEMIS_H

#include <stdio.h>
#include <math.h>

#define PI              3.14159265358979323846
#define G_GRAVITACION   6.674e-11      /* N*m^2/kg^2 */

#define MASA_TIERRA_DEF 5.972e24       /* kg */
#define RADIO_TIERRA_DEF 6371000.0     /* m  */
#define ALTITUD_DEF     192000.0       /* m  */
#define MASA_ORION_DEF  33000.0        /* kg */
#define ACEL_PULSO_DEF  0.50           /* m/s^2 */
#define DT_PULSO_DEF    10.0           /* s */


#define ALTITUD_MIN     160000.0       /* m
#define ALTITUD_MAX     2000000.0      /* m
#define MASA_NAVE_MIN   1.0            /* kg */
#define MASA_NAVE_MAX   1.0e6          /* kg */
#define ACEL_PULSO_MIN  0.0            /* m/s^2 */
#define ACEL_PULSO_MAX  10.0           /* m/s^2 */
#define DT_PULSO_MIN    0.0            /* s */
#define DT_PULSO_MAX    600.0          /* s */

/* Datos de entrada de la orbita: */
typedef struct {
    double masa_tierra;    /* MT, kg */
    double radio_tierra;   /* RT, m  */
    double altitud;        /* h,  m  */
    double masa_nave;      /* m,  kg */
} DatosOrbita;

/* Resultados de los puntos A, B y C. */
typedef struct {
    double radio_orbital;          /* r = RT + h, m */
    double fuerza_gravitatoria;    /* Fg, N */
    double velocidad_orbital;      /* v, m/s */
    double periodo_s;              /* T, segundos */
    double periodo_min;            /* T, minutos */
    double aceleracion_centripeta; /* ac, m/s^2 */
} ResultadosOrbita;

/* Datos de entrada del pulso lateral. */
typedef struct {
    double aceleracion;    /* a,  m/s^2 */
    double duracion;       /* dt, s */
} DatosPropulsor;

/* Resultados del pulso lateral. */
typedef struct {
    double velocidad_lateral;   /* vl = a*dt, m/s */
    double desviacion_lateral;  /* d = 1/2*a*dt^2, m */
} ResultadosPropulsor;

void   cargar_valores_originales(DatosOrbita *orbita, DatosPropulsor *propulsor);
double calcular_radio_orbital(const DatosOrbita *orbita);
double calcular_fuerza_gravitatoria(const DatosOrbita *orbita);
double calcular_velocidad_orbital(const DatosOrbita *orbita);
double calcular_periodo_orbital(const DatosOrbita *orbita);
double calcular_aceleracion_centripeta(const DatosOrbita *orbita);
void   calcular_orbita(const DatosOrbita *orbita, ResultadosOrbita *res);
void   mostrar_demostracion_centripeta(const DatosOrbita *orbita);
void   mostrar_resultados_orbita(const DatosOrbita *orbita, const ResultadosOrbita *res);

void calcular_pulso_lateral(const DatosPropulsor *prop, ResultadosPropulsor *res);
void mostrar_resultados_propulsor(const DatosPropulsor *prop, const ResultadosPropulsor *res);

#endif /* ARTEMIS_H */
