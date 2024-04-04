#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <random>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

const string CARPETA_INSTANCIAS = "../instancias/";
const string FICHERO_INSTANCIAS = "programa3";
const string CARPETA_OUTPUT = "./salidas/";
const string FICHERO_OUTPUT = "programa3";
const string CARPETA_TIEMPOS = "./tiempos/";
const string FICHERO_TIEMPOS = "programa3";

// Borra el contenido de carpeta;
// Si falla, devuelve false
bool borrarContenidoCarpeta(const std::string& carpeta) {
    DIR* dir = opendir(carpeta.c_str());
    if (dir == nullptr) {
        std::cerr << "Error al abrir la carpeta: " << carpeta << std::endl;
        return false;
    }

    dirent* entrada;
    while ((entrada = readdir(dir)) != nullptr) {
        // Ignora las entradas "." y ".."
        if (std::string(entrada->d_name) == "." || std::string(entrada->d_name) == "..") {
            continue;
        }

        // Construye la ruta completa del archivo/directorio
        std::string rutaCompleta = carpeta + "/" + entrada->d_name;

        // Elimina el archivo o directorio
        if (remove(rutaCompleta.c_str()) != 0) {
            std::cerr << "Error al eliminar: " << rutaCompleta << std::endl;
            closedir(dir);
            return false;
        }
    }

    closedir(dir);
    return true;
}

// Genera un número aleatorio en [0, rango]
int aleatorioRango(int rango){
    int num_valores = rango + 1;
    return rand() % num_valores;
}

// Dada una carpeta, un fichero y un índice, crea la ruta corresondiente
string formateaRuta(string carpeta, string fichero, int n){
    return carpeta + fichero + "_" + to_string(n) + ".txt";
}

/**
 * Generador de casos, ejecuta y genera gráficas
 *
 * ej : Programa ejecutable
 * min: Minimo valor de n
 * max: Maximo valor de n
 * salto: Saltos entre n
 * [estado]: {1, 2, 3} Si no es especifica, 3 por defecto
 *      1: Sólo genera instancias
 *      2: Sólo genera instancias y ejecuta midiendo tiempos
 *      3: Hace 2 y genera gráfica y regresión
*/
int main(int argc, char** argv){
    if(argc < 5 || argc > 6){
        cout << "Uso: ./ejecutable ./ej min max salto" << endl;
        cout << "Uso: ./ejecutable ./ej min max salto estado" << endl;
        exit(-1);
    }

    // Lectura de parámetros

    int min = strtol(argv[2], NULL, 10);
    int max = strtol(argv[3], NULL, 10);
    int salto = strtol(argv[4], NULL, 10);

    int estado;
    if(argc == 5){
        estado = 3;
    }else{
        estado = strtol(argv[5], NULL, 10);
    }

    // instanciar generador de números aleatorios

    time_t t;
    srand(time(&t));

    // Ejecución

    if(estado >= 1){ // Estado = 1, 2, ó 3
        int n;
        borrarContenidoCarpeta(CARPETA_INSTANCIAS);

        set<pair<int, int> > ciudades;

        // generamos todas las instancias de n
        for(n = min; n <= max; n += salto){
            ofstream flujo(formateaRuta(CARPETA_INSTANCIAS, FICHERO_INSTANCIAS, n));
            if(!flujo){
                cout << "Error al crear flujo";
            }

            flujo << n << "\n";

            // genero n ciudades no repetidas
            pair<int, int> elemento;

            while(ciudades.size() != n){
                elemento = pair<int, int>(aleatorioRango(2*n), aleatorioRango(2*n));
                ciudades.insert(elemento);
            }

            // Las paso a un vector y hago shuffle

            vector<pair<int, int>> ciudades_instancias(ciudades.begin(), ciudades.end());
            random_device rd;
            mt19937 g(rd()); 
            shuffle(ciudades_instancias.begin(), ciudades_instancias.end(), g);

            // pongo las n ciudades en el archivo
            for(int i = 0; i < n; i++){
                flujo << ciudades_instancias[i].first << " " << ciudades_instancias[i].second << endl;
            }

            flujo.close();
            ciudades.clear();
        }
    }

    if(estado >= 2){ // estado = 2 ó 3 
        // Elimino archivo anterior
        
        string archivoTiempos = CARPETA_TIEMPOS + FICHERO_TIEMPOS;
        if(remove(archivoTiempos.c_str()) != 0){
            cout << "Error al borrar archivo de tiempos antiguo" << endl;
        }

        // borro contenido de carpeta de salidas

        borrarContenidoCarpeta(CARPETA_OUTPUT);

        // Por cada instancia
        string orden;
        for(int n = min; n <= max; n += salto){
            orden = argv[1]; // ruta al ejecutable
            orden += " " + formateaRuta(CARPETA_INSTANCIAS, FICHERO_INSTANCIAS, n);
            orden += " " + formateaRuta(CARPETA_OUTPUT, FICHERO_OUTPUT, n);
            orden += " | head -n1 >> " + CARPETA_TIEMPOS + FICHERO_TIEMPOS;

            if(system(orden.c_str()) != 0){
                cout << "Error en el tamaño " << n << endl;
            }
        }
    }

    if(estado >= 3){ // estado = 3
        string archivoTiempos = CARPETA_TIEMPOS + FICHERO_TIEMPOS;

        string orden = "cd " + CARPETA_TIEMPOS + ";";
        orden += "gnuplot script_programa3.gp";

        if(system(orden.c_str()) != 0){
            cout << "Error en gnuplot" << endl;
        }
    }
            
    return 0;
}
