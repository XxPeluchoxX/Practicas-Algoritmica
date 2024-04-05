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
 * @brief Calcula la distancia euclidea entre dos puntos en el plano.
 *
 * @param p1 El primer punto.
 * @param p2 El segundo punto.
 */
double dist(pair<int,int> p1, pair<int,int> p2){
    int diff_x = p1.first - p2.first;
    int diff_y = p1.second - p2.second;
    return sqrt(diff_x*diff_x + diff_y*diff_y);
}




/**
 * @brief Dadas sup - inf + 1 ciudades calcula el camino mas economico en cuanto a 
 * distancias. La primera ciudad se queda la primera siempre.
 *
 * @param v Las ciudades. Parámetro de entrada y salida, se devuelve con las 
 * ciudades ya ordenadas. 
 * @param inf La primera ciudad de la lista que tenemos en cuenta.
 * @param sup La ultima ciudad de la lista que tenemos en cuenta.
 */
void mejor_camino_Iterativo(vector<pair<int,int>> &v, int inf, int sup){
    if (inf >= sup){ // Caso base, no hay ciudades que considerar
        return;
    }
    sort(v.begin()+inf+1, v.begin()+sup+1); // El next_permutation necesita que este el vector en orden ascendente.

    int n = sup - inf + 1;  // Número de ciudades que consideramos
    double min = DBL_MAX;   // Distancia mínima inicial. La inicializamos a un valor muy grande para que cualquier distancia sea menor

    vector<pair<int, int>> perm(n); // Lista de las ciudades que vamos a considerar

    // Almacena en perm las ciudades que vamos a considerar. Permutación inicial
    for(int i = 0; i < n; i++){
        perm[i] = v[inf + i];
    }

    double sum = 0; // Suma de las distancias de la permutación actual
    do{
        sum = 0;

        // Contabilizamos todas las distancias de la permutación actual
        for(int i = 0; i < n-1; i++){
            sum += dist(perm[i], perm[i+1]);
        }
        sum += dist(perm[n-1], perm[0]); // Cierre del camino

        // Si la suma de distancias de la permutación actual es menor que la mínima encontrada hasta ahora, se actualiza
        if(sum < min){
            min = sum;      // Actualiza la distancia mínima
            for(int i = inf; i <= sup; i++){ // Actualiza el vector de ciudades para ponerlo en el orden de la permutación actual
                v[i] = perm[i-inf];
            }
        }
        
    }while(next_permutation(perm.begin()+1, perm.end()));   // Genera la siguiente permutación, sin contabilizar la primera ciudad

}


/**
 * @brief Sobrecarga del operador << para imprimir un vector de pares de enteros
 * 
 * @param os Flujo de salida
 * @param v Vector de pares de enteros
 * @return ostream& Referencia al flujo de salida
 */
ostream& operator<<(ostream &os, vector<pair<int,int>> v){
    vector<pair<int,int>>::iterator it;
    for(it = v.begin(); it != v.end(); ++it){
        os << it->first << " " << it->second << endl;
    }
    return os;
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


    // Apertura del archivo de entrada
    ifstream entrada(argv[1]);
    if (!entrada.is_open()){
        cout << "Error al abrir el archivo de entrada\n";
        return 1;
    }
    
    // Apertura del archivo de salida
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
    entrada.close();

    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;

    // Busco el mejor camino
    tantes = high_resolution_clock::now();
    mejor_camino_Iterativo(ciudades, 0, n-1);
    tdespues = high_resolution_clock::now();
    
    transcurrido = duration_cast<duration<double>>(tdespues - tantes);

    // Añadimos la primera ciudad (volver a ella)
    ciudades.push_back(ciudades[0]);

    // Salida. Imprimimos el tiempo y las ciudades por orden
    cout << n << " " << transcurrido.count() << endl;
    salida << ciudades;

    salida.close();

    return 0;
}
