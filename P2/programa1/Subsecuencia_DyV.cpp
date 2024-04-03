#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <utility>

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
		// Actualizo el borde por la izquierda.
		// Voy desplazandome a la izquierda (hasta encontrarme con bounds_max_izq) y actualizando la suma
        int lim_izq_tmp;
		for (lim_izq_tmp=bounds_max_central.first-1; lim_izq_tmp>bounds_max_izq.second; lim_izq_tmp--){
			suma_central_tmp += v[lim_izq_tmp];
			if (suma_central_tmp>=suma_max_central){
				bounds_max_central.first = lim_izq_tmp;
				suma_max_central = suma_central_tmp;
			}
		}
        // Si se ha llegado a bounds_max_izq, se aprovecha que sabemos que la suma de bounds_max_izq es la mayor
        if (lim_izq_tmp==bounds_max_izq.second){
            suma_central_tmp += suma_max_izq;
            if (suma_central_tmp>=suma_max_central){
                bounds_max_central.first = bounds_max_izq.first;
                suma_max_central = suma_central_tmp;
            }
        }
		
		// Actualizo el borde por la dcha.
		// Voy desplazandome a la dcha (hasta encontrarme con bounds_max_dcha) y actualizando la suma
		suma_central_tmp = suma_max_central;
        int lim_dcha_tmp;
		for (lim_dcha_tmp=bounds_max_central.second+1; lim_dcha_tmp<bounds_max_dcha.first; lim_dcha_tmp++){
			suma_central_tmp += v[lim_dcha_tmp];
			if (suma_central_tmp>=suma_max_central){
				bounds_max_central.second = lim_dcha_tmp;
				suma_max_central = suma_central_tmp;
			}
		}
        // Si se ha llegado a bounds_max_dcha, se aprovecha que sabemos que la suma de bounds_max_dcha es la mayor
        if (lim_dcha_tmp==bounds_max_dcha.first){
            suma_central_tmp += suma_max_dcha;
            if (suma_central_tmp>=suma_max_central){
                bounds_max_central.second = bounds_max_dcha.second;
                suma_max_central = suma_central_tmp;
            }
        }


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
        cout << "Central: " << bounds_max_central.first << " " << bounds_max_central.second << " " << suma_central << endl;
        cout << "Mejor: " << result.first << " " << result.second << " " <<  suma_max << endl;
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



int main (int argc,char **argv){
    
    if(argc!=2){
        cout << " ./ejecutable <FILE>";
        return 1;
    }

    pair <int,int> result;
    srand(time(0));

    string file = argv[1];
    
    chrono::high_resolution_clock::time_point tantes, tdespues;
    chrono::duration<double> transcurrido;
    int tam;

    ifstream input(file);
    
    if(!input){
        cout << "File" + file + "not found. Couldn't be opened.";
    }

    else {
        input >> tam;
        vector <int> v;
        int dato;
        for (int i=0; i<tam; ++i){
            input>>dato;
            v.push_back(dato);
        }

        tantes= chrono::high_resolution_clock::now();

        result = subsecMax_DyV(v,0,v.size()-1);

        tdespues = chrono::high_resolution_clock::now();

        transcurrido = chrono::duration_cast <chrono::duration<double>> (tdespues-tantes);
    }
    input.close();

    //Salida
    //file=argv[2];
    //ofstream output(file);
//
    //if(!output){
        //cout << "File" + file + "not found. Couldn't be opened.";
    //}
    //else{
        //output << result.first << " " << result.second;
    //}
    //output.close();
    //file=argv[3];
    //output.open(file);
//
    //if(!output){
        //cout << "File" + file + "not found. Couldn't be opened.";
    //}
    //else{
        //output << tam << " " << transcurrido.count() << endl;
    //}
    //output.close();
    //
    cout << tam << " " << transcurrido.count() << endl;
    cout << result.first << " " << result.second;
    return 0;

}
