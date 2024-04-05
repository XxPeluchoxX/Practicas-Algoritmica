# Makefile para compilar los programas Subsecuencia_Iterativo.cpp,
# Subsecuencia_DyV.cpp y generadorPrograma1.cpp

# Hay 4 reglas importantes:
# make: compila los 3 cpp
# make clean: elimina todos los archivos basura, que no queremos subir al repositorio
# make save_It: guarda los resultados del iterativo en una carpeta, conclusiones_It, y se llama a clean
# make save_DyV: guarda los resultados del iterativo en una carpeta, conclusiones_DyV, y se llama a clean

CXX = g++				# Compilador a utilizar
CXXFLAGS = -Wall -O2	# Opciones de compilación

# Lista de todos los programas a compilar
PROGRAMS = Subsecuencia_Iterativo Subsecuencia_DyV generadorPrograma1

# Regla para construir todos los programas
all: $(PROGRAMS)

# Regla para compilar cada programa
Subsecuencia_Iterativo: Subsecuencia_Iterativo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

Subsecuencia_DyV: Subsecuencia_DyV.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

generadorPrograma1: generadorPrograma1.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para limpiar los archivos generados
clean:
	rm -f $(PROGRAMS)
	rm ./instancias/*
	rm ./tiempos/programa1*

# Regla para crear una carpeta conclusiones, y copiar ahí las instancias y los tiempos
save_It:
	mkdir -p conclusiones_It conclusiones_It/instancias conclusiones_It/tiempos
	mv ./instancias/* ./conclusiones_It/instancias
	mv ./tiempos/programa1* ./conclusiones_It/tiempos
	rm -f $(PROGRAMS)

save_DyV:
	mkdir -p conclusiones_DyV conclusiones_DyV/instancias conclusiones_DyV/tiempos
	mv ./instancias/* ./conclusiones_DyV/instancias
	mv ./tiempos/programa1* ./conclusiones_DyV/tiempos
	rm -f $(PROGRAMS)