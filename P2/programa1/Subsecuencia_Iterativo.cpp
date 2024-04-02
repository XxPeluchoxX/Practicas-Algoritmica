#include <iostream>
#include <random>
#include <utility>
#include <vector>

using namespace std;


/**
 * @brief Funcion que calcula la suma de los elementos de un vector en un rango determinado.
 * 
 * @param v  Vector de enteros
 * @param init Posicion inicial
 * @param fin  Posicion final
 * @return int Suma de los elementos del vector en el rango [init,fin]
 */
int suma(const vector<int> & v, int init, int fin){
    int result=0;

    bool index_ok = 0<=init && init<=fin && fin<v.size();

    // Tan solo se calcula la suma si los indices son correctos
    // Evitamos así las subsecuencias vacias
    if(index_ok)
        for(int i=init; i<=fin; ++i)
            result+=v[i];
    
    return result;
}


/**
 * @brief Funcion que calcula la suma maxima de una subsecuencia de un vector de enteros. Algoritmo Iterativo.
 * 
 * @param v Vector de enteros
 * @param init  Posicion inicial
 * @param fin  Posicion final
 * @return pair<int,int>  Par de enteros que representan la posicion inicial y final de la subsecuencia maxima
 */
pair<int,int> subsecMax_Itertivo (const vector<int> &v, int init, int fin){

    // Variables para almacenar la subsecuencia maxima local
    int localini=0, localfin=0, localmax=0;

    int max=0; // Variable para almacenar la suma de la subsecuencia maxima por ahora
    const int INVALID_INDEX = -1; // Valor para indicar que no se ha encontrado una subsecuencia valida
    pair<int,int> bounds_max(INVALID_INDEX,INVALID_INDEX); // Limites de la subsecuencia maxima

    
    for(int i=init; i<fin; ++i){
        
        // Si la suma local es negativa, no la consideramos porque nos quedaremos con la subsecuencia vacia
        if(localmax >= 0){
            // Se actualiza la suma local y el límite de la derecha avanza
            localmax += v[i];
            localfin = i;
        } else{
            // La subsecuencia anterior no era válida. La reseteamos a la subsecuencia que empieza en i 
            localmax = v[i];
            localini = localfin = i;
        }


        // Actualizamos la subsecuencia maxima si la suma local es mayor
                //Si se quiere la subsecuencia mas pequeña posible, añadir:
                // || (localmax == max && localfin - localini < bounds_max.second - bounds_max.first)
        if (localmax > max){ 
            max = localmax;
            bounds_max.first = localini;
            bounds_max.second = localfin;
        }
        
    }
    return bounds_max;

}

int main(){
    const vector<int> v = {1,2,40,-50,7,-10,4,20,  30,-11,-1,0,0,0,-200,40};


    for(int i=0; i<v.size(); ++i){
        cout << v[i] << " ";
    }
    cout << endl;

    pair<int,int> bounds_max = subsecMax_Itertivo(v,0, v.size()-1);
    cout << suma(v, bounds_max.first, bounds_max.second) << "   " << bounds_max.first << " " << bounds_max.second << endl;
    return 0;
}