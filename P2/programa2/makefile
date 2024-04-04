# Makefile para compilar los programas
# Enlosar.cpp y generadorPrograma2.cpp

CXX = g++				# Compilador a utilizar
CXXFLAGS = -Wall -O2	# Opciones de compilación

# Lista de todos los programas a compilar
PROGRAMS = Enlosar generadorPrograma2

# Regla para construir todos los programas
all: $(PROGRAMS)

# Regla para compilar cada programa
Enlosar:
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

generadorPrograma2:
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

# Regla para limpiar los archivos generados
clean:
	rm -f $(PROGRAMS)
	rm ./instancias/*
	rm ./salidas/programa2*
	rm ./tiempos/programa2*