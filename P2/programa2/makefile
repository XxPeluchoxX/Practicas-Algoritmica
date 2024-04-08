# Makefile para compilar los programas
# Enlosar.cpp y generadorPrograma2.cpp

# Hay 3 reglas importantes:
# make: compila los 2 cpp
# make clean: elimina todos los archivos basura, que no queremos subir al repositorio
# make save: guarda los resultados en una carpeta, conclusiones, y se llama a clean


CXX = g++				# Compilador a utilizar
CXXFLAGS = -O2	# Opciones de compilación

# Lista de todos los programas a compilar
PROGRAMS = Enlosar generadorPrograma2

# Regla para construir todos los programas
all: $(PROGRAMS)

# Regla para compilar cada programa
Enlosar: Enlosar.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

generadorPrograma2: generadorPrograma2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para limpiar los archivos generados
clean:
	rm -f $(PROGRAMS)
	rm ./instancias/*
	rm ./salidas/programa2*
	rm ./tiempos/programa2*

# Regla para crear una carpeta conclusiones, y copiar ahí las instancias, los tiempos y las salidas
save:
	mkdir -p conclusiones conclusiones/instancias conclusiones/tiempos conclusiones/salidas
	mv ./instancias/* ./conclusiones/instancias
	mv ./tiempos/programa2* ./conclusiones/tiempos
	mv ./salidas/programa2* ./conclusiones/salidas
	rm -f $(PROGRAMS)
