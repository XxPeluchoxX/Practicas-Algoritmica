#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

// Si el exponente para el problema es mayor, no generamos archivos con la solucion
const int MAX_EXP_GENERA_ARCHIVO = 8;

const string CARPETA_INSTANCIAS = "./instancias/";
const string FICHERO_INSTANCIAS = "programa2";
const string CARPETA_OUTPUT = "./salidas/";
const string FICHERO_OUTPUT = "programa2";
const string CARPETA_TIEMPOS = "./tiempos/";
const string FICHERO_TIEMPOS = "programa2";

/**
 * @brief Borra el contenido de una carpeta
 * 
 * @param carpeta ruta de la carpeta a borrar
 * @return true La carpeta se ha borrado correctamente
 * @return false Ha habido un error al borrar la carpeta
 */
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

/**
 * @brief Devuelve un número aleatorio en [0, rango]
 * 
 * @param rango valor que acota el rango de valores aleatorios
 * @return int número aleatorio en [0, rango]
 */
int aleatorioRango(int rango){
    int num_valores = rango + 1;
    return rand() % num_valores;
}

/**
 * @brief Dada una carpeta, un fichero y un índice, crea la ruta corresondiente
 * 
 * @param carpeta Ruta de la carpeta
 * @param fichero Nombre del fichero
 * @param n Índice
 * @return string Ruta formateada. Ejemplo: "./carpeta/fichero_n.txt" 
 */
string formateaRuta(string carpeta, string fichero, int n){
    return carpeta + fichero + "_" + to_string(n) + ".txt";
}

/**
 * Generador de casos, ejecuta y genera gráficas
 *
 * ej : Programa ejecutable
 * min: Minimo valor de n
 * max: Maximo valor de n
 * [estado]: {1, 2, 3} Si no es especifica, 3 por defecto
 *      1: Sólo genera instancias
 *      2: Sólo genera instancias y ejecuta midiendo tiempos
 *      3: Hace 2 y genera gráfica y regresión
*/
int main(int argc, char** argv){
    if(argc < 4 || argc > 5){
        cout << "Uso: ./ejecutable ./ej min max" << endl;
        cout << "Uso: ./ejecutable ./ej min max estado" << endl;
        exit(-1);
    }

    // Lectura de parámetros

    //string ruta_ejecutable = argv[1];
    int min = strtol(argv[2], NULL, 10);
    int max = strtol(argv[3], NULL, 10);

    int estado;
    const int DEFAULT_ESTADO = 3;
    if(argc == 4){
        estado = DEFAULT_ESTADO;
    }else{
        estado = strtol(argv[4], NULL, 10);
    }

    // instanciar generador de números aleatorios

    time_t t;
    srand(time(&t));

    // Ejecución

    if(estado >= 1){ // Estado = 1, 2, ó 3
        int n, m;
        borrarContenidoCarpeta(CARPETA_INSTANCIAS);

        // generamos todas las instancias de n
        for(n = min; n <= max; n++){
            m = 1 << n; // 2^n

            ofstream flujo(formateaRuta(CARPETA_INSTANCIAS, FICHERO_INSTANCIAS, n));
            if(!flujo){
                cout << "Error al crear flujo";
            }

            flujo << n << "\n";
            flujo << aleatorioRango(m) << " " << aleatorioRango(m);
            flujo.close();
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
        for(int n = min; n <= max; n++){

            orden = argv[1]; // ruta al ejecutable
            orden += " " + formateaRuta(CARPETA_INSTANCIAS, FICHERO_INSTANCIAS, n);

            // Si el exponente es pequeno, generamos la salida en un archivo
            if(n <= MAX_EXP_GENERA_ARCHIVO){
                orden += " " + formateaRuta(CARPETA_OUTPUT, FICHERO_OUTPUT, n);
            }

            orden += " | head -n1 >> " + CARPETA_TIEMPOS + FICHERO_TIEMPOS;

            if(system(orden.c_str()) != 0){
                cout << "Error en el tamaño " << n << endl;
            }
        }
    }

    if(estado >= 3){ // estado = 3
        string archivoTiempos = CARPETA_TIEMPOS + FICHERO_TIEMPOS;

        string orden = "cd " + CARPETA_TIEMPOS + ";";
        orden += "gnuplot script_programa2.gp";

        if(system(orden.c_str()) != 0){
            cout << "Error en gnuplot" << endl;
        }
    }
            
    return 0;
}
