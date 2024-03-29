#include <iostream> 
#include <string> 
#include <fstream> 
#include <cmath> 
#include <algorithm> 
#include <vector> 
#include <limits> 
#include <cfloat> 
using namespace std;



/**
 * Reserva memoria para una matriz.
 *
 * @param m La matriz en la que queremos reservar memoria.
 * @param n El tamaño de la matriz.
 */
void allocateM(double **m, int n){
    m = new double*[n];
    for(int i=0; i < n; i++){
        m[i] = new double[n];
    }
}




/**
 * Borra la memoria dinamica de la matriz pasada como parametro.
 * 
 * @param m La matriz a borrar.
 * @param n El tamaño de la matriz.
 */
void deallocateM(double **m, int n){
    for(int i=0; i < n; i++){
        delete[] m[i];
    }
    delete[] m;
}


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
 * Dadas n ciudades calcula la distancia entre cada par de ellas guardando
 * las distancias en una matriz nxn.
 * @param v El vector de puntos con las ciudades.
 * @param m La matriz donde guardamos las distancias.
 * @param n El numero de ciudades.
 */
void calc_distancias(pair<int,int> *v, double **m, int n){
    // Llenamos la diagonal de 0
    // en la diagonal se situa la distancia a la misma ciudad   

    for(int i=0; i < n; i++){
        m[i][i] = 0;
    }
    
    // Calculando solo una vez la distancia llenamos el m[i][j], m[j][i]

    for(int i = 1; i < n ; i++){
        for(int j = 0; j < i; j++){
            m[i][j] = dist(v[i], v[j]);
            m[j][i] = m[i][j];
        }
    }
}


/**
 * Dadas sup - inf + 1 ciudades calcula el camino mas economico en cuanto a 
 * distancias. 
 *
 * @param m La matriz con las distancias entre las distintas ciudades.
 * @param inf La primera ciudad de la lista que tenemos en cuenta.
 * @param sup La ultima ciudad de la lista que tenemos en cuenta.
 *
 * @return El vector con la permutacion del camino mas economico.
 */
vector<int> mejor_camino(double **m, int inf, int sup){
    vector<int> perm;
    for(int i = inf; i <= sup; i++){
        perm.push_back(i); 
    }
    
    double min = DBL_MAX;
    vector<int> mejor_camino;
    
    // Para cada permutacion posible calculo la suma de todas las distancias    
    do{
        double loc_sum=0;   
        for(int i = 0; i < perm.size() -1; i++){
            loc_sum += m[perm[i]][perm[i+1]];
        }
        loc_sum += m[perm[0]][perm[perm.size()-1]];

        if (loc_sum < min){
            min = loc_sum;
            mejor_camino = perm;
        }
    }while (next_permutation(perm.begin(), perm.end())); 

    //cout << "min " << min << endl;

    return mejor_camino;
}



/**
 * Programa principal
 *
 * @param argc Numero de argumentos
 * @param argv Array con los argumentos
 *
 * Uso: programa <archivo>
 */
int main(int argc, char **argv){
    // Comprobacion de parametros
    if (argc != 2){
        cout << "Uso: ./mejor_camino <archivo>\n";
        return 1;
    }    

    // Abrimos el archivo
    string ruta = argv[1];

    ifstream archivo(ruta);
    if (!archivo.is_open()){
        cout << "Error al abrir el archivo\n";
        return 1;
    }

    // Lectura del archivo
    int n, x, y;

    archivo >> n;

    pair<int,int> *ciudades = new pair<int,int>[n];

    for(int i=0; i < n; i++){
        archivo >> ciudades[i].first >> ciudades[i].second;
    }

    // Calculo las distancias
    double **distancias = nullptr;
    
    //allocateM(distancias, n);
    distancias = new double*[n];
    for(int i=0; i < n; i++){
        distancias[i] = new double[n];
    }
    
    calc_distancias(ciudades,distancias,n);

    // Busco el mejor camino
    vector<int> mejor = mejor_camino(distancias, 0, n-1);
    //cout << min << endl;

    // Borramos la memoria dinamica
    deallocateM(distancias, n);
    delete[] ciudades;

    return 0;
}
