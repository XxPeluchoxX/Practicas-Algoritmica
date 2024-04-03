#include <iostream> 
#include <string> 
#include <fstream> 
#include <cmath> 
#include <algorithm> 
#include <vector> 
#include <limits> 
#include <cfloat> 
#include <ctime> 
#include <chrono> 

using namespace std;
using namespace std::chrono;





/**
 * Calcula la distancia euclidea entre dos puntos en el plano.
 *
 * @param p1 El primer punto.
 * @param p2 El segundo punto.
 */
double dist(pair<int,int> p1, pair<int,int> p2){
    return sqrt((p1.first - p2.first)*(p1.first - p2.first) + (p1.second - p2.second)*(p1.second - p2.second));
}




/**
 * Dadas sup - inf + 1 ciudades calcula el camino mas economico en cuanto a 
 * distancias. La primera ciudad se queda la primera siempre.
 *
 * @param m La matriz con las distancias entre las distintas ciudades.
 * @param v Las ciudades. Parámetro de entrada y salida, se devuelve con las 
 * ciudades ya ordenadas. 
 * @param inf La primera ciudad de la lista que tenemos en cuenta.
 * @param sup La ultima ciudad de la lista que tenemos en cuenta.
 */
void mejor_camino(vector<pair<int,int>> &v, int inf, int sup){
    if (inf == sup || inf+1 == sup){
        return;
    }

    int n = sup - inf + 1;
    double min = DBL_MAX;
    vector<pair<int, int>> perm;
    for(int i = 0; i < sup - inf + 1; i++){
        perm.push_back(v[inf + i]);
    }

    double sum = 0;
    do{
        sum = 0;

        for(int i = 0; i < n-1; i++){
            sum += dist(perm[i], perm[i+1]);
        }

        sum += dist(perm[n-1], perm[0]);

        if(sum < min){
            min = sum;
            for(int i = inf; i <= sup; i++){
                v[i] = perm[i-inf];
            }
        }
        
    }while(next_permutation(perm.begin()+1, perm.end()));

}

/**
 * Imprime el contenido de v
*/
void print_v(vector<pair<int,int>> v, ofstream &os){
    vector<pair<int,int>>::iterator it;
    for(it = v.begin(); it != v.end(); ++it){
        os << it->first << " " << it->second << endl;
    }
}

/**
 * Programa principal
 *
 * @param argc Numero de argumentos
 * @param argv Array con los argumentos
 *
 * Uso: programa <entrada> <salida>
 * Formato de entrada
 * n
 * x y
 * x y
 * .
 * .
 * .
 */
int main(int argc, char **argv){
    // Comprobacion de parametros
    if (argc != 3){
        cout << "Uso: ejecutable <entrada> <salida>\n";
        return 1;
    }    


    ifstream entrada(argv[1]);
    if (!entrada.is_open()){
        cout << "Error al abrir el archivo de entrada\n";
        return 1;
    }
    
    ofstream salida(argv[2]);
    if (!salida.is_open()){
        cout << "No se puede abrir el archivo de salida\n"; 
        return 1;
    }

    // Lectura del archivo
    int n;
    entrada >> n;
    vector<pair<int,int>> ciudades(n); 

    for(int i=0; i < n; i++){
        entrada >> ciudades[i].first >> ciudades[i].second;
    }

    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;

    // Busco el mejor camino

    tantes = high_resolution_clock::now();
    mejor_camino(ciudades, 0, n-1);
    tdespues = high_resolution_clock::now();
    
    transcurrido = duration_cast<duration<double>>(tdespues - tantes);

    // Doy la salida de tiempo
    cout << n << " " << transcurrido.count() << endl;


    // Añadimos la primera ciudad (volver a ella)
    ciudades.push_back(ciudades[0]);
    print_v(ciudades, salida);

    return 0;
}
