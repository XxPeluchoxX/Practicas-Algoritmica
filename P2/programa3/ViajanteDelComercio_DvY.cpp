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
#include <unistd.h>

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

// resul : saca ac
// a1, a2: izqda
// b1, b2: dcha
void funcion(pair<int, int>* resul, pair<int, int> a1, pair<int, int> a2, pair<int, int> b1, pair<int, int> b2, pair<int, int> p){
    // calculo 4 posibilidades
    double d1 = dist(a1, p) + dist(p, b1) + dist(a2, b2);
    double d2 = dist(a1, p) + dist(p, b2) + dist(a2, b1);
    double d3 = dist(a2, p) + dist(p, b1) + dist(a1, b2);
    double d4 = dist(a2, p) + dist(p, b2) + dist(a1, b1);

    if(d1 <= d2 && d1 <= d3 && d1 <= d4){
        resul[0] = a1;
        resul[1] = b2;
    }else if(d2 <= d1 && d2 <= d3 && d2 <= d4){
        resul[0] = a1;
        resul[1] = b1;
    }else if(d3 <= d1 && d3 <= d2 && d3 <= d4){
        resul[0] = a2;
        resul[1] = b2;
    }else{
        resul[0] = a2;
        resul[1] = b1;
    }
}

void intercambia(vector<pair<int, int>>& v, pair<int, int>* resul, int inf, int sup, int k){
    if(v[inf] == resul[0]){ // i == a
        reverse(v.begin() + inf, v.begin() + k);  // revertir [inf, k[
    }
    if(v[k+1] == resul[1]){ // k+1 == c
        reverse(v.begin() + k+1, v.begin() + sup + 1);   // revertir [k+1, s]
    }
}

bool functor(const pair<int, int> &a, const pair<int, int> &b) {
    return a.second < b.second;
}

void ordena(vector<pair<int,int>> &v, int inf, int sup, bool sort_by_x){
    if(sort_by_x){
        sort(v.begin() + inf, v.begin() + sup + 1); // [inf, sup]
    }else{
        sort(v.begin() + inf, v.begin() + sup + 1, functor);    // [inf, sup]
    }
}

// Rota k posiciones el vector entre [inf, sup]
void rota(vector<pair<int, int>> &v, int inf, int sup, int k){
    vector<pair<int, int>> temp(k);
    
    // Guarda los k primeros
    for(int i = inf; i < inf + k; i++){
        temp[i-inf] = v[i];
    }

    // Rota todos k posiciones adelante
    for(int i = inf + k; i <= sup; i++){
        v[i-k] = v[i];
    }

    // Pone los k primeros en las k ultimas posiciones
    for(int i = sup - k + 1; i <= sup; i++){
        v[i] = temp[i - sup + k - 1];
    }
}

// Busca buscado en v, entre [inf, sup] y devuelve su indice - inf
int busca(vector<pair<int, int>> &v, int inf, int sup, pair<int, int> buscado){
    for(int i = inf; i <= sup; i++){
        if(v[i] == buscado)
            return i-inf;
    }

    return inf;
}

// Calcular distancias en brute force
void mejor_caminoDyV(vector<pair<int,int>> &v, int inf, int sup, bool sort_by_x){
    if(sup - inf + 1 <= 8){
        mejor_camino(v, inf, sup);
    }else{
        // Guardo anterior vector
        pair<int, int> old_pivote = v[inf];

        // ordenamos y cogemos la mediana
        ordena(v, inf, sup, sort_by_x);
        int k = inf + (sup - inf+1)/2;

        // [k, sup]
        mejor_caminoDyV(v, k, sup, !sort_by_x);

        // [inf, k] con el pivote al inicio
        swap(v[inf], v[k]);
        mejor_caminoDyV(v, inf, k, !sort_by_x);
        reverse(v.begin() + inf, v.begin() + k + 1); // ponemos el pivote en k

        // Obtenemos el array bien ordenado
        pair<int, int> vect[2];
        funcion(vect, v[inf], v[k-1], v[k+1], v[sup], v[k]);
        intercambia(v, vect, inf, sup, k);

        // Devuelvo la solucion con el pivote antiguo en inf
        int desfase = busca(v, inf, sup, old_pivote);
        rota(v, inf, sup, desfase);
    }
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
    int n, x, y;
    entrada >> n;
    vector<pair<int,int>> ciudades(n); 

    for(int i=0; i < n; i++){
        entrada >> ciudades[i].first >> ciudades[i].second;
    }

    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;

    // Busco el mejor camino
    //sort(ciudades.begin(), ciudades.end());

    tantes = high_resolution_clock::now();
    mejor_caminoDyV(ciudades, 0, n-1, true);
    tdespues = high_resolution_clock::now();
    
    transcurrido = duration_cast<duration<double>>(tdespues - tantes);

    // Doy la salida de tiempo
    cout << n << " " << transcurrido.count() << endl;


    // Añadimos la primera ciudad (volver a ella)
    ciudades.push_back(ciudades[0]);
    print_v(ciudades, salida);

    return 0;
}
