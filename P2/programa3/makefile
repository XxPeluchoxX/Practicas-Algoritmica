# Makefile para compilar los programas ViajanteDelComercio_Iterativo.cpp,
# ViajanteDelComercio_DyV.cpp y generadorPrograma3.cpp

CXX = g++				# Compilador a utilizar
CXXFLAGS = -Wall -O2	# Opciones de compilación

# Lista de todos los programas a compilar
PROGRAMS = ViajanteDelComercio_Iterativo ViajanteDelComercio_DyV generadorPrograma3 calculaDist

# Regla para construir todos los programas
all: $(PROGRAMS)

# Regla para compilar cada programa
ViajanteDelComercio_Iterativo:
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

ViajanteDelComercio_DyV:
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

generadorPrograma3:
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

calculaDist:
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp

# Regla para limpiar los archivos generados
clean:
	rm -f $(PROGRAMS)
	rm ./instancias/*
	rm ./salidas/programa3*
	rm ./tiempos/programa3*