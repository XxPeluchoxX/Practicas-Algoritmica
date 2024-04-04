#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <utility>
#include <ctime> 
using namespace std;
using namespace std::chrono;


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
 * @brief Funcion que calcula la subsecuencia maxima de un vector de enteros. Algoritmo Divide y Venceras.
 * 
 * @param v  Vector de enteros
 * @param init  Posicion inicial
 * @param fin  Posicion final
 * @return pair<int,int>  Par de enteros que representan la posicion inicial y final de la subsecuencia maxima
 */
pair<int,int> subsecMax_DyV (const vector<int> &v, int init, int fin){
        
    pair<int,int> bounds_max;
    int  suma_max;
    
    // Caso base. n=1
    if(fin <= init){
        bounds_max.first=bounds_max.second=init;
        suma_max=v[init];
    }
        

    else{
        int middle=init + (fin-init)/2;

        pair<int,int> bounds_max_izq  = subsecMax_DyV(v,init,middle);   // Subsecuencia maxima de la izquierda
        pair<int,int> bounds_max_dcha = subsecMax_DyV(v,middle+1,fin);  // Subsecuencia maxima de la derecha

        int suma_max_izq  = suma(v,bounds_max_izq.first,bounds_max_izq.second);
        int suma_max_dcha = suma(v,bounds_max_dcha.first,bounds_max_dcha.second);


        // Combinacion del centro
		// Bucamos la subsecMax_DyV que contenga a middle y a middle+1
        pair <int,int> bounds_max_central (middle,middle+1);
		int suma_max_central = suma(v, bounds_max_central.first, bounds_max_central.second);

		int suma_central_tmp = suma_max_central;

        // Si hay subsencuencia máxima a la izq, actualizo el borde por la izquierda
        if (bounds_max_izq.second >= 0){
            // Actualizo el borde por la izquierda.
            // Voy desplazandome a la izquierda (hasta encontrarme con bounds_max_izq) y actualizando la suma
            int lim_izq_tmp = bounds_max_central.first;

            while (lim_izq_tmp>bounds_max_izq.second){
                lim_izq_tmp--;
                suma_central_tmp += v[lim_izq_tmp];
                if (suma_central_tmp>=suma_max_central){
                    bounds_max_central.first = lim_izq_tmp;
                    suma_max_central = suma_central_tmp;
                }
            }

            // Si se ha llegado a bounds_max_izq, se aprovecha que sabemos que la suma de bounds_max_izq es la mayor
            // por lo que se contabiliza de golpe.
            // Se resta el valor de v[bounds_max_izq.second] porque ha sido el último que ya se ha sumado antes
            if (lim_izq_tmp==bounds_max_izq.second){
                suma_central_tmp += suma_max_izq - v[lim_izq_tmp];

                if (suma_central_tmp>=suma_max_central){
                    bounds_max_central.first = bounds_max_izq.first;
                    suma_max_central = suma_central_tmp;
                }
            }
    
        } // Fin de la actualización del borde por la izquierda

        	

        // Si hay subsencuencia máxima a la dcha, actualizo el borde por la dcha
        if (bounds_max_dcha.first >= 0){
            // Actualizo el borde por la dcha.
            // Voy desplazandome a la dcha (hasta encontrarme con bounds_max_dcha) y actualizando la suma
            suma_central_tmp = suma_max_central;
            int lim_dcha_tmp = bounds_max_central.second;

            while (lim_dcha_tmp<bounds_max_dcha.first){
                lim_dcha_tmp++;
                suma_central_tmp += v[lim_dcha_tmp];
                if (suma_central_tmp>=suma_max_central){
                    bounds_max_central.second = lim_dcha_tmp;
                    suma_max_central = suma_central_tmp;
                }
            }

            // Si se ha llegado a bounds_max_dcha, se aprovecha que sabemos que la suma de bounds_max_dcha es la mayor
            // por lo que se contabiliza de golpe.
            // Se resta el valor de v[bounds_max_dcha.first] porque ha sido el último que ya se ha sumado antes
            if (lim_dcha_tmp==bounds_max_dcha.first){
                suma_central_tmp += suma_max_dcha - v[lim_dcha_tmp];

                if (suma_central_tmp>=suma_max_central){
                    bounds_max_central.second = bounds_max_dcha.second;
                    suma_max_central = suma_central_tmp;
                }
            }
        } // Fin de la actualización del borde por la dcha
		
		
		


        //Combinacion de casos. Me quedo con el caso mejor.
        if(suma_max_izq>=suma_max_dcha){
            bounds_max = bounds_max_izq;
            suma_max = suma_max_izq;
        }
        else{
            bounds_max = bounds_max_dcha;
            suma_max = suma_max_dcha;
        }

        if(suma_max_central>= suma_max){
            bounds_max = bounds_max_central;
            suma_max = suma_max_central;
        }

        #ifdef DEBUG
        // Imprimo los resultados parciales, para comprobar que el algoritmo funciona correctamente
        cout << "Init: " << init << " Fin: " << fin << endl;
        cout << "Secuencia: ";
        for(int i=init; i<=fin; ++i)
            cout << v[i] << " ";
        cout << endl;
        cout << "Izq: " << bounds_max_izq.first << " " << bounds_max_izq.second << " " << suma_max_izq << endl;
        cout << "Dcha: " << bounds_max_dcha.first << " " << bounds_max_dcha.second << " " << suma_max_dcha << endl;
        cout << "Central: " << bounds_max_central.first << " " << bounds_max_central.second << " " << suma_max_central << endl;
        cout << "Mejor: " << bounds_max.first << " " << bounds_max.second << " " <<  suma_max << endl;
        cout << endl;
        #endif
    }

    // Si la suma maxima es negativa, devolvemos un par de indices invalidos
    if (suma_max<0){
        const int INVALID_INDEX = -1;
        return pair<int,int>(INVALID_INDEX,INVALID_INDEX);
    }
    else
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
        cout << " ./ejecutable <entrada> [<salida>]";
        return 1;
    }

    pair <int,int> result;
    int tam;

    // Apertura del archivo de entrada
    ifstream input(argv[1]);    
    if(!input){
        cout << "No se pudo abrir el archivo de entrada\n";
        return 1;
    }

    // Apertura del archivo de salida
    ofstream output;
    if (argc == 3){
        output.open(argv[2]);
        if (!output.is_open()){
            cout << "No se pudo abrir el archivo de salida\n";
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


    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;

    tantes= chrono::high_resolution_clock::now();
    result = subsecMax_DyV(v,0,v.size()-1);
    tdespues = chrono::high_resolution_clock::now();

    transcurrido = duration_cast <duration<double>> (tdespues-tantes);
    

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
