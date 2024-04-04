#include <iostream>
#include <random>
#include <utility>
#include <vector>
#include <chrono>
#include <ctime> 
#include <fstream> 

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
pair<int,int> subsecMax_Iterativo (const vector<int> &v, int init, int fin){

    // Variables para almacenar la subsecuencia maxima local
    int localini=0, localfin=0, localmax=0;

    int max=0; // Variable para almacenar la suma de la subsecuencia maxima por ahora
    const int INVALID_INDEX = -1; // Valor para indicar que no se ha encontrado una subsecuencia valida
    pair<int,int> bounds_max(INVALID_INDEX,INVALID_INDEX); // Limites de la subsecuencia maxima

    
    for(int i=init; i<=fin; ++i){
        
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


/**
 * Programa principal
 *
 * Uso: ejecutable <entrada> [<salida>]
 * Formato de la entrada:
 * n 
 * x x x x ...
 */
int main (int argc,char **argv){
    
    if(argc!=3 && argc != 2){
        cerr << " Error: ./ejecutable <entrada> [<salida>]" << endl;
        return 1;
    }

    pair <int,int> result;
    int tam;

    // Apertura del archivo de entrada
    ifstream input(argv[1]);
    if(!input){
        cerr << "No se pudo abrir el archivo de entrada\n";
        return 1;
    }

    // Apertura del archivo de salida
    ofstream output;
    if (argc == 3){
        output.open(argv[2]);
        if (!output.is_open()){
            cerr << "No se pudo abrir el archivo de salida\n";
            return 1;
        }
    }

    // Lectura de los datos de entrada
    input >> tam;
    vector <int> v(tam);
    for (int i=0; i<tam; ++i){
        input>>v[i];
    }
    input.close();


    chrono::high_resolution_clock::time_point tantes, tdespues;
    chrono::duration<double> transcurrido;

    tantes= chrono::high_resolution_clock::now();
    result = subsecMax_Iterativo(v,0,v.size()-1);
    tdespues = chrono::high_resolution_clock::now();

    transcurrido = chrono::duration_cast <chrono::duration<double>> (tdespues-tantes);
    

    // Salida.
    // En pantalla se muestra el tamaño del problema y el tiempo de ejecución
    // En el archivo de salida (si se proporciona) se muestra el resultado
    cout << tam << " " << transcurrido.count() << endl;
    if (argc == 3){
        output << result.first << " " << result.second << endl; 
        output.close();
    } else{
        cout << result.first << " " << result.second << endl;
    }
    

    return 0;
}
