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

const string CARPETA_INSTANCIAS = "./instancias/";
const string FICHERO_INSTANCIAS = "programa3";

const string CARPETA_OUTPUT = "./salidas/";
const string FICHERO_OUTPUT = "programa3";

const string NOMBRE_CARPETA_CAMINOS = "dibujos";
const string CARPETA_CAMINOS = "./" + NOMBRE_CARPETA_CAMINOS + "/"; // relativa a CARPETA_OUTPUT
const string SCRIPT_CAMINOS = "dibuja_caminos.gp"; // se guardará temporalmente en CARPETA_OUTPUT

const string CARPETA_TIEMPOS = "./tiempos/";
const string FICHERO_TIEMPOS = "programa3";

/**
 * @brief Borra el contenido de una carpeta
 * 
 * @param carpeta ruta de la carpeta a borrar
 * @return true La carpeta se ha borrado correctamente
 * @return false Ha habido un error al borrar la carpeta
 */
bool borrarContenidoCarpeta(const string& carpeta) {
    DIR* dir = opendir(carpeta.c_str());
    if (dir == nullptr) {
        cerr << "Error al abrir la carpeta: " << carpeta << endl;
        return false;
    }

    dirent* entrada;
    while ((entrada = readdir(dir)) != nullptr) {
        // Ignora las entradas ".", ".." y CARPETA_CAMINOS
        if (string(entrada->d_name) == "." || string(entrada->d_name) == ".."
                || string(entrada->d_name) == NOMBRE_CARPETA_CAMINOS) {
            continue;
        }

        // Construye la ruta completa del archivo/directorio
        string rutaCompleta = carpeta + "/" + entrada->d_name;

        // Elimina el archivo o directorio
        if (remove(rutaCompleta.c_str()) != 0) {
            cerr << "Error al eliminar: " << rutaCompleta << endl;
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
 * @brief Genera un script de gnuplot para graficar los caminos
 * 
 * @param n  Número de ciudades
 * @param datos  Archivo de datos
 * @param ruta  Ruta de salida
 * @param script  Nombre del script
 */
void generaScriptGnuplot(int n, const string& datos, const string& ruta, const string& script){
    ofstream flujo(script);

    flujo << "set xrange [0:" + to_string(2*n) + "]\n"
            + "set yrange [0:" + to_string(2*n) + "]\n"
            + "set pointsize 2\n"
            + "unset key\n"
            + "plot '" + datos + "' with points, '" + datos + "' with lines\n"
            + "set terminal pngcairo\n"
            + "set output '" + ruta + "camino_" + to_string(n) + ".png'\n"
            + "replot\n";

    flujo.close();
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
 *      3: Hace 2 y genera visualmente los caminos computados
 *      4: Hace 3 y genera gráfica y regresión
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
    const int DEFAULT_ESTADO = 4;
    if(argc == 5){
        estado = DEFAULT_ESTADO;
    }else{
        estado = strtol(argv[5], NULL, 10);
    }

    // instanciar generador de números aleatorios

    time_t t;
    srand(time(&t));

    // Ejecución

    if(estado >= 1){ // Estado = 1, 2, 3 ó 4
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

    if(estado >= 2){ // estado = 2, 3 ó 4
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

    if(estado >= 3){ // estado = 3 ó 4
        string archivoGnuplot = CARPETA_CAMINOS + SCRIPT_CAMINOS;
        string datos, orden;

        // Por cada instancia generada
        for(int n = min; n <= max; n += salto){
            // genera el script gnuplot
            datos = formateaRuta("", FICHERO_OUTPUT, n);
            generaScriptGnuplot(n, datos, CARPETA_CAMINOS, CARPETA_OUTPUT + SCRIPT_CAMINOS);

            // ejecuta el script gnuplot
            orden = "cd " + CARPETA_OUTPUT + ";";
            orden += "gnuplot " + SCRIPT_CAMINOS;
            if(system(orden.c_str()) != 0){
                cout << "Error al graficar tamano " << n << endl;
            }
        }
    }

    if(estado >= 4){ // estado = 4
        string archivoTiempos = CARPETA_TIEMPOS + FICHERO_TIEMPOS;

        string orden = "cd " + CARPETA_TIEMPOS + ";";
        orden += "gnuplot script_programa3.gp";

        if(system(orden.c_str()) != 0){
            cout << "Error en gnuplot" << endl;
        }
    }
            
    return 0;
}
