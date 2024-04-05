# Makefile para compilar los programas ViajanteDelComercio_Iterativo.cpp,
# ViajanteDelComercio_DyV.cpp y generadorPrograma3.cpp

# Hay 4 reglas importantes:
# make: compila los 3 cpp
# make clean: elimina todos los archivos basura, que no queremos subir al repositorio
# make save_It: guarda los resultados del iterativo en una carpeta, conclusiones_It, y se llama a clean
# make save_DyV: guarda los resultados del iterativo en una carpeta, conclusiones_DyV, y se llama a clean


CXX = g++				# Compilador a utilizar
CXXFLAGS = -Wall -O2	# Opciones de compilación

# Lista de todos los programas a compilar
PROGRAMS = ViajanteDelComercio_Iterativo ViajanteDelComercio_DyV generadorPrograma3 calculaDist

# Regla para construir todos los programas
all: $(PROGRAMS)

# Regla para compilar cada programa
ViajanteDelComercio_Iterativo: ViajanteDelComercio_Iterativo.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

ViajanteDelComercio_DyV: ViajanteDelComercio_DyV.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

generadorPrograma3: generadorPrograma3.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

calculaDist: calculaDist.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para limpiar los archivos generados
clean:
	rm -f $(PROGRAMS)
	rm ./instancias/*
	rm ./salidas/programa3*
	rm ./salidas/dibujos/*
	rm ./tiempos/programa3*
	
# Regla para crear una carpeta conclusiones, y copiar ahí las instancias y los tiempos
save_It:
	mkdir -p conclusiones_It conclusiones_It/instancias conclusiones_It/tiempos conclusiones_It/salidas conclusiones_It/salidas/dibujos
	mv ./instancias/* ./conclusiones_It/instancias
	mv ./tiempos/programa3* ./conclusiones_It/tiempos
	mv ./salidas/programa3* ./conclusiones_It/salidas
	mv ./salidas/dibujos/* ./conclusiones_It/salidas/dibujos

	rm -f $(PROGRAMS)

save_DyV:
	mkdir -p conclusiones_DyV conclusiones_DyV/instancias conclusiones_DyV/tiempos conclusiones_DyV/salidas conclusiones_DyV/salidas/dibujos
	mv ./instancias/* ./conclusiones_DyV/instancias
	mv ./tiempos/programa3* ./conclusiones_DyV/tiempos
	mv ./salidas/programa3* ./conclusiones_DyV/salidas
	mv ./salidas/dibujos/* ./conclusiones_DyV/salidas/dibujos

	rm -f $(PROGRAMS)