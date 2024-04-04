# Makefile para compilar los programas Subsecuencia_Iterativo.cpp,
# Subsecuencia_DyV.cpp y generadorPrograma1.cpp

CXX = g++				# Compilador a utilizar
CXXFLAGS = -Wall -O2	# Opciones de compilación

# Lista de todos los programas a compilar
PROGRAMS = Subsecuencia_Iterativo Subsecuencia_DyV generadorPrograma1

# Regla para construir todos los programas
all: $(PROGRAMS)

# Regla para compilar cada programa
Subsecuencia_Iterativo:
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

Subsecuencia_DyV:
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

generadorPrograma1:
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

# Regla para limpiar los archivos generados
clean:
	rm -f $(PROGRAMS)
	rm ./instancias/*
	rm ./salidas/programa1*
	rm ./tiempos/programa1*