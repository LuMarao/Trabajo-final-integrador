#ifndef ARTEMIS_H
#define ARTEMIS_H

#define G            6.674e-11   /* N·m²/kg² */
#define MASA_TIERRA  5.972e24    /* kg */
#define RADIO_TIERRA 6371000.0   /* m */
#define PI           3.14159265358979

typedef struct {
    double masa_nave;      /* kg */
    double altitud;        /* m */
    double radio_orbital;  /* m (RT + h) */
} Orbita;

typedef struct {
    double fuerza_grav;       /* N */
    double fuerza_centripeta; /* N */
    double velocidad;         /* m/s */
    double periodo_s;         /* s */
    double periodo_min;       /* min */
    double aceleracion_c;     /* m/s² */
} ResultadosOrbita;

double calcular_radio_orbital(double altitud);
double calcular_fuerza_gravitatoria(double masa, double radio);
double calcular_velocidad_orbital(double radio);
double calcular_fuerza_centripeta(double masa, double velocidad, double radio);
double calcular_periodo(double radio, double velocidad);
double calcular_aceleracion_centripeta(double velocidad, double radio);
void   calcular_orbita(const Orbita *o, ResultadosOrbita *res);
void   mostrar_demostracion(const ResultadosOrbita *res);

#endif
