# Simulador de Misiones Artemis – Escenario 2: Orion en órbita baja terrestre (LEO)

## Descripción del proyecto

Este proyecto consiste en desarrollar un programa en lenguaje C que permita simular algunos aspectos físicos de la nave Orion mientras se encuentra en una órbita baja alrededor de la Tierra (LEO).

El programa permite realizar cálculos relacionados con la órbita de la nave y simular el efecto de un pulso de propulsión lateral. Los datos iniciales pueden ser modificados por el usuario para realizar diferentes pruebas.

El trabajo corresponde a la materia Algoritmos y Estructuras de Datos de la carrera Ingeniería en Sistemas de Información de la UTN Facultad Regional Resistencia.

## Integrantes

- Luciana Marao – Menú e integración.
- Uriel – Cálculos de física orbital.
- Luciano – Propulsor lateral.
- Alegre Sanchez Ruben Bautista – Validaciones.
- Aranda Silvero Maximiliano Uriel – Estructuras, documentación y pruebas.

**Comisión:** K 1.2

## Escenario de trabajo

La simulación se basa en una nave Orion que, luego de apagar sus motores de ascenso, permanece en una órbita circular alrededor de la Tierra.

Para realizar los cálculos se utilizan los siguientes datos iniciales:

- Masa de la Tierra: 5,972 × 10²⁴ kg.
- Radio de la Tierra: 6.371.000 m.
- Altitud de la órbita: 192.000 m.
- Radio orbital: 6.563.000 m.
- Constante gravitatoria: 6,674 × 10⁻¹¹ N·m²/kg².
- Masa de Orion: 33.000 kg.
- Aceleración del pulso lateral: 0,50 m/s².
- Duración del pulso: 10 segundos.

## Cálculos que realiza el programa

El programa contempla los siguientes cálculos:

- Fuerza gravitatoria y velocidad orbital de la nave.
- Período orbital, expresado en segundos y minutos.
- Aceleración centrípeta.
- Velocidad lateral y desviación producida por el pulso del propulsor.

Para el pulso lateral se considera una aceleración constante durante el tiempo indicado.

Como referencia, utilizando los datos iniciales se obtienen aproximadamente los siguientes resultados:

- Fuerza gravitatoria: 305.362 N.
- Velocidad orbital: 7.792,95 m/s.
- Período orbital: 5.291,52 segundos (88,19 minutos).
- Aceleración centrípeta: 9,253 m/s².
- Velocidad lateral: 5 m/s.
- Desviación lateral: 25 m.

## Funcionamiento del programa

Al iniciar, el programa carga los valores iniciales del escenario y muestra un menú con las operaciones disponibles.

Desde el menú, el usuario podrá consultar o modificar los datos, realizar los cálculos de la órbita, simular el pulso lateral o salir del programa.

Los datos ingresados serán validados para evitar valores incorrectos. Luego de cada operación se mostrarán los resultados correspondientes y se podrá volver al menú principal.

## Organización de los archivos

El proyecto se organiza en distintos archivos para separar las tareas del programa:

- `main.c`: contiene la función principal y el menú.
- `artemis.h`: reúne las constantes, estructuras y declaraciones de funciones compartidas.
- `artemis.c`: contiene los cálculos relacionados con la órbita.
- `propulsor.c` y `propulsor.h`: contienen las operaciones relacionadas con el propulsor lateral, si se utiliza este módulo.
- `validaciones.c` y `validaciones.h`: se encargan de validar los datos ingresados.
- `Makefile`: permite automatizar la compilación del programa.

## Compilación y ejecución

Una vez que estén integrados los archivos del proyecto, se podrá compilar desde una terminal utilizando GCC.

El comando previsto es:

```bash
gcc -Wall -Wextra -std=c99 main.c artemis.c propulsor.c validaciones.c -o artemis -lm
```

Para ejecutar el programa:

```bash
./artemis
```

El comando de compilación podrá modificarse según los archivos que finalmente formen parte del proyecto. La opción `-lm` permite utilizar las funciones matemáticas de la biblioteca correspondiente.

## Uso de inteligencia artificial

Durante el desarrollo del proyecto se registrarán las herramientas de inteligencia artificial utilizadas, las consultas realizadas y los cambios o comprobaciones efectuados por los integrantes.

Este registro permitirá dejar constancia de cómo se utilizaron estas herramientas y qué partes fueron revisadas y comprendidas por el equipo.