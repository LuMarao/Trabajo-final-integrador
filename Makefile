# Compatible con MSYS2 (terminal UCRT64) y Linux.
CC     = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2
LDLIBS = -lm
SRC    = propulsor.c validaciones.c
 
# En Windows gcc genera archivos .exe
ifeq ($(OS),Windows_NT)
EXE = .exe
endif
 
# Banderas de SDL3 (pkg-config las provee MSYS2: paquete mingw-w64-ucrt-x86_64-sdl3)
SDL_FLAGS = $(shell pkg-config --cflags --libs sdl3)
 
all: demo_propulsor$(EXE) test_propulsor$(EXE)
 
demo_propulsor$(EXE): demo_propulsor.c $(SRC) artemis.h validaciones.h
	$(CC) $(CFLAGS) -o $@ demo_propulsor.c $(SRC) $(LDLIBS)
 
test_propulsor$(EXE): test_propulsor.c $(SRC) artemis.h validaciones.h
	$(CC) $(CFLAGS) -o $@ test_propulsor.c $(SRC) $(LDLIBS)
 
test: test_propulsor$(EXE)
	./test_propulsor$(EXE)
 
# Programa completo (ventana con SDL3, requiere SDL 3.2.0 o mas nuevo).
# main.c no usa -std=c99/-pedantic porque los headers de SDL3 usan extensiones.
simulador_sdl$(EXE): main.c artemis.c propulsor.c validaciones.c artemis.h validaciones.h
	@pkg-config --exists sdl3 || { echo "ERROR: no se encontro SDL3. En MSYS2 UCRT64: pacman -S mingw-w64-ucrt-x86_64-sdl3"; exit 1; }
	$(CC) -Wall -Wextra -O2 -o $@ main.c artemis.c propulsor.c validaciones.c $(SDL_FLAGS) $(LDLIBS)
 
ifneq ($(EXE),)
# Permite escribir "make simulador_sdl" tambien en Windows
simulador_sdl: simulador_sdl$(EXE)
endif
 
clean:
	rm -f demo_propulsor demo_propulsor.exe test_propulsor test_propulsor.exe \
	      simulador_sdl simulador_sdl.exe *.o
 
.PHONY: all test clean simulador_sdl
 
