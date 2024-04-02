#include <iostream> 
#include <string> 
#include <fstream> 
#include <cmath> 
#include <algorithm> 
#include <vector> 
#include <limits> 
#include <cfloat> 

#include <unistd.h>

using namespace std;

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

// resul : saca abcd
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

void rota(vector<pair<int, int>> &v, int inf, int sup){
    pair<int, int> primero = v[inf];
    for(int i = inf; i < sup; i++){
        v[i] = v[i+1];
    }

    v[sup] = primero;
}

// Calcular distancias en brute force
void mejor_caminoDyV(vector<pair<int,int>> &v, int inf, int sup, bool sort_by_x){
    if(sup - inf + 1 <= 8){
        mejor_camino(v, inf, sup);
    }else{
        pair<int, int> old_pivote = v[inf];
        ordena(v, inf, sup, sort_by_x);
        int k = inf + (sup - inf+1)/2;

        //
        //cout << "Imprimo vector" << endl;
        //for(int i = inf; i <= sup; i++){
            //cout << v[i].first << " " << v[i].second << endl;
        //}
        //cout << "Pivote: " << v[k].first << " " << v[k].second << endl;
        //

        mejor_caminoDyV(v, k, sup, !sort_by_x);

        swap(v[inf], v[k]);
        mejor_caminoDyV(v, inf, k, !sort_by_x);
        reverse(v.begin() + inf, v.begin() + k + 1);

        //
        //cout << "Mejor camino dcha:" << endl;
        //for(int i = k; i <= sup; i++){
            //cout << v[i].first << " " << v[i].second << endl;
        //}
        //cout << "hola" << endl;
        //cout << "Mejor camino izqda:" << endl;
        //for(int i = inf; i <= k; i++){
            //cout << v[i].first << " " << v[i].second << endl;
        //}
        //cout << "hola" << endl;
        //

        pair<int, int> vect[2];
        funcion(vect, v[inf], v[k-1], v[k+1], v[sup], v[k]);
        intercambia(v, vect, inf, sup, k);

        //
        //cout << "Mejor camino con pivote centrado" << endl;
        //for(int i = inf; i <= sup; i++){
            //cout << v[i].first << " " << v[i].second << endl;
        //}
        //cout << "hola" << endl;
        //

        // vuelvo a old_pivote en posición 0
        while(v[inf] != old_pivote){
            rota(v, inf, sup);
        }

        //
        //cout << "Mejor camino con pivote inicial" << endl;
        //for(int i = inf; i <= sup; i++){
            //cout << v[i].first << " " << v[i].second << endl;
        //}
        //cout << "hola" << endl;
        //
    }
}

/**
 * Imprime el contenido de v
*/
void print_v(vector<pair<int,int>> v){
    vector<pair<int,int>>::iterator it;
    for(it = v.begin(); it != v.end(); ++it){
        cout << it->first << " " << it->second << endl;
    }
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
    vector<pair<int,int>> ciudades(n); 

    for(int i=0; i < n; i++){
        archivo >> ciudades[i].first >> ciudades[i].second;
    }

    // Busco el mejor camino
    sort(ciudades.begin(), ciudades.end());
    mejor_caminoDyV(ciudades, 0, n-1, true);
    
    ciudades.push_back(ciudades[0]);
    print_v(ciudades);

    return 0;
}
