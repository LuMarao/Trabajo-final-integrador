CC     = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2 -Idependencias_de_otros
LDLIBS = -lm
SRC    = propulsor.c dependencias_de_otros/validaciones.c

demo_propulsor: demo_propulsor.c $(SRC)
	$(CC) $(CFLAGS) -o $@ demo_propulsor.c $(SRC) $(LDLIBS)

test_propulsor: test_propulsor.c $(SRC)
	$(CC) $(CFLAGS) -o $@ test_propulsor.c $(SRC) $(LDLIBS)

test: test_propulsor
	./test_propulsor

clean:
	rm -f demo_propulsor test_propulsor *.o

.PHONY: test clean
