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

    for(int i=init; i<=fin; i++)
        result+=v[i];
    
    return result;
}


/**
 * @brief Funcion que calcula la subsecuencia maxima de un vector de enteros.
 * 
 * @param v  Vector de enteros
 * @param init  Posicion inicial
 * @param fin  Posicion final
 * @return pair<int,int>  Par de enteros que representan la posicion inicial y final de la subsecuencia maxima
 */
pair<int,int> subsecMax (const vector<int> &v, int init, int fin){
        
    pair<int,int> result;
    int suma_result;
    
    // Caso base. n=1
    if(fin <= init){
        result.first=result.second=init;
        return result;
    }

    else{
        int middle=init + (fin-init)/2;

        pair<int,int> result_izq  = subsecMax(v,init,middle);   // Subsecuencia maxima de la izquierda
        pair<int,int> result_dcha = subsecMax(v,middle+1,fin);  // Subsecuencia maxima de la derecha

        int suma_izq  = suma(v,result_izq.first,result_izq.second);
        int suma_dcha = suma(v,result_dcha.first,result_dcha.second);


        // Combinacion del centro
		// Bucamos la subsecMax que contenga a middle y a middle+1
        pair <int,int> result_central (middle,middle+1);
		int sum_max = suma(v, result_central.first, result_central.second);

		int sum_tmp = sum_max;		
		// Actualizo el borde por la izquierda.
		// Voy desplazandome a la izquierda (hasta encontrarme con result_izq) y actualizando la suma
        int lim_izq_tmp;
		for (lim_izq_tmp=result_central.first-1; lim_izq_tmp>result_izq.second; lim_izq_tmp--){
			sum_tmp += v[lim_izq_tmp];
			if (sum_tmp>=sum_max){
				result_central.first = lim_izq_tmp;
				sum_max = sum_tmp;
			}
		}
        // Si se ha llegado a result_izq, se aprovecha que sabemos que la suma de result_izq es la mayor
        if (lim_izq_tmp==result_izq.second){
            sum_tmp += suma_izq;
            if (sum_tmp>=sum_max){
                result_central.first = result_izq.first;
                sum_max = sum_tmp;
            }
        }
		
		// Actualizo el borde por la dcha.
		// Voy desplazandome a la dcha (hasta encontrarme con result_dcha) y actualizando la suma
		sum_tmp = sum_max;
        int lim_dcha_tmp;
		for (lim_dcha_tmp=result_central.second+1; lim_dcha_tmp<result_dcha.first; lim_dcha_tmp++){
			sum_tmp += v[lim_dcha_tmp];
			if (sum_tmp>=sum_max){
				result_central.second = lim_dcha_tmp;
				sum_max = sum_tmp;
			}
		}
        // Si se ha llegado a result_dcha, se aprovecha que sabemos que la suma de result_dcha es la mayor
        if (lim_dcha_tmp==result_dcha.first){
            sum_tmp += suma_dcha;
            if (sum_tmp>=sum_max){
                result_central.second = result_dcha.second;
                sum_max = sum_tmp;
            }
        }

        // Se actualiza la suma de la subsecuencia central
        int suma_central = sum_max;


        //Combinacion de casos. Me quedo con el caso mejor.
        if(suma_izq>=suma_dcha){
            result = result_izq;
            suma_result=suma_izq;
        }
        else{
            result = result_dcha;
            suma_result=suma_dcha;
        }

        if(suma_central>=suma_result){
            result = result_central;
            suma_result=suma_central;
        }

        #ifdef DEBUG
        // Imprimo los resultados parciales, para comprobar que el algoritmo funciona correctamente
        cout << "Init: " << init << " Fin: " << fin << endl;
        cout << "Secuencia: ";
        for(int i=init; i<=fin; i++)
            cout << v[i] << " ";
        cout << endl;
        cout << "Izq: " << result_izq.first << " " << result_izq.second << " " << suma_izq << endl;
        cout << "Dcha: " << result_dcha.first << " " << result_dcha.second << " " << suma_dcha << endl;
        cout << "Central: " << result_central.first << " " << result_central.second << " " << suma_central << endl;
        cout << "Mejor: " << result.first << " " << result.second << " " << suma_result << endl;
        cout << endl;
        #endif

        return result;
    }
}



int main (int argc,char **argv){
    
    if(argc!=4){
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

        result = subsecMax(v,0,v.size()-1);

        tdespues = chrono::high_resolution_clock::now();

        transcurrido = chrono::duration_cast <chrono::duration<double>> (tdespues-tantes);
    }
    input.close();

    //Salida
    file=argv[2];
    ofstream output(file);

    if(!output){
        cout << "File" + file + "not found. Couldn't be opened.";
    }
    else{
        output << result.first << " " << result.second;
    }
    output.close();
    file=argv[3];
    output.open(file);

    if(!output){
        cout << "File" + file + "not found. Couldn't be opened.";
    }
    else{
        output << tam << " " << transcurrido.count() << endl;
    }
    output.close();
    return 0;

}
